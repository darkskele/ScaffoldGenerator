#include "ClassParser.h"
#include "ParserUtilities.h"
#include "PropertiesParser.h"
#include "CallableParser.h"
#include "SpecialMemberFunctionParser.h"
#include "CallableModels.h"

#include <stdexcept>
#include <unordered_set>
#include <optional>
#include <deque>

namespace ClassParser
{
    /**
     * @brief Processes a top-level property line in a class DSL block.
     *
     * This static helper function parses a property line (which is expected to start with
     * the '|' character) by splitting it into a key-value pair. It updates the provided
     * class-level properties—such as description, assignment flags, or member lists—based
     * on the current access specifier.
     *
     * @param line The DSL property line (starting with '|') to be processed.
     * @param description A reference to the class description string to update.
     * @param hasCopyAssignment A reference to the flag indicating whether copy assignment is enabled.
     * @param hasMoveAssignment A reference to the flag indicating whether move assignment is enabled.
     * @param constructors A reference to the vector of constructors to be updated.
     * @param publicMembers A reference to the vector of public data members.
     * @param privateMembers A reference to the vector of private data members.
     * @param protectedMembers A reference to the vector of protected data members.
     * @param currentAccess The current access specifier in effect for subsequent declarations.
     *
     * @throws std::runtime_error if an unrecognized property key is encountered.
     */
    static void processTopLevelProperty(std::string_view line,
                                        std::string &description,
                                        bool &hasCopyAssignment,
                                        bool &hasMoveAssignment,
                                        std::vector<ClassModels::Constructor> &constructors,
                                        std::vector<PropertiesModels::Parameter> &publicMembers,
                                        std::vector<PropertiesModels::Parameter> &privateMembers,
                                        std::vector<PropertiesModels::Parameter> &protectedMembers,
                                        Access currentAccess)
    {
        // Remove leading '|' if present and trim.
        if (!line.empty() && line.front() == '|')
        {
            line.remove_prefix(1);
            line = ParserUtilities::trim(line);
        }
        size_t eqPos = line.find('=');
        if (eqPos == std::string_view::npos)
            return; // Ignore if not a valid key=value pair.

        std::string_view key = ParserUtilities::trim(line.substr(0, eqPos));
        std::string_view value = ParserUtilities::trim(line.substr(eqPos + 1));

        if (key == "description")
        {
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
            {
                value.remove_prefix(1);
                value.remove_suffix(1);
            }
            description = std::string(ParserUtilities::trim(value));
        }
        else if (key == "constructors")
        {
            // Expect comma-separated constructor types.
            for (auto typeStr : ParserUtilities::split(value, ','))
            {
                typeStr = ParserUtilities::trim(typeStr);
                ClassModels::ConstructorType type;
                if (typeStr == "default")
                    type = ClassModels::ConstructorType::DEFAULT;
                else if (typeStr == "copy")
                    type = ClassModels::ConstructorType::COPY;
                else if (typeStr == "move")
                    type = ClassModels::ConstructorType::MOVE;
                else
                    throw std::runtime_error("Unknown constructor type: " + std::string(typeStr));
                constructors.emplace_back(type, std::vector<PropertiesModels::Parameter>{}, "");
            }
        }
        else if (key == "assignment")
        {
            auto tokens = ParserUtilities::split(value, ',');
            for (auto tok : tokens)
            {
                tok = ParserUtilities::trim(tok);
                if (tok == "copy")
                    hasCopyAssignment = true;
                else if (tok == "move")
                    hasMoveAssignment = true;
                else
                    throw std::runtime_error("Unknown assignment type: " + std::string(tok));
            }
        }
        else if (key == "members")
        {
            std::vector<PropertiesModels::Parameter> parsed = PropertiesParser::parseParameters(value);
            switch (currentAccess)
            {
            case Access::Public:
                publicMembers.insert(publicMembers.end(), parsed.begin(), parsed.end());
                break;
            case Access::Private:
                privateMembers.insert(privateMembers.end(), parsed.begin(), parsed.end());
                break;
            case Access::Protected:
                protectedMembers.insert(protectedMembers.end(), parsed.begin(), parsed.end());
                break;
            default:
                // If no access is set, default to private.
                privateMembers.insert(privateMembers.end(), parsed.begin(), parsed.end());
                break;
            }
        }
        else
        {
            throw std::runtime_error("Unknown class-level property: " + std::string(key));
        }
    }

    /**
     * @brief Parses a class block from the DSL using a consume-as-you-go approach.
     *
     * This function processes DSL lines from the provided deque, consuming each line as it is handled.
     * It supports top-level properties, nested blocks for methods, constructors, and destructors,
     * as well as access specifier sections. If no valid DSL content is found, an error is thrown.
     */
    ClassModels::ClassModel parseClassBlock(const std::string &className, std::deque<std::string_view> &lines)
    {
        std::string description;
        bool hasCopyAssignment = false;
        bool hasMoveAssignment = false;
        std::vector<ClassModels::Constructor> constructors;
        std::optional<ClassModels::Destructor> destructor;

        std::vector<CallableModels::MethodModel> publicMethods, privateMethods, protectedMethods;
        std::vector<PropertiesModels::Parameter> publicMembers, privateMembers, protectedMembers;

        // Start with no access specifier; defaults will be private.
        Access currentAccess = Access::None;

        bool validContentFound = false;

        while (!lines.empty())
        {
            std::string_view line = ParserUtilities::trim(lines.front());
            lines.pop_front(); // Consume the line

            if (line.empty())
                continue;

            if (line == "_")
            {
                // If we are inside an access specifier, an "_" terminates that section.
                if (currentAccess != Access::None)
                {
                    currentAccess = Access::None;
                    continue; // Do not break out of the class block.
                }
                else
                {
                    // Otherwise, this is the end of the class block.
                    validContentFound = true;
                    break;
                }
            }
            else if (line.starts_with("- "))
            {
                validContentFound = true;
                // Process header line.
                line.remove_prefix(2); // Remove "- " marker.
                line = ParserUtilities::trim(line);
                // Remove trailing colon if present.
                if (!line.empty() && line.back() == ':')
                {
                    line.remove_suffix(1);
                    line = ParserUtilities::trim(line);
                }
                // Determine if header is a simple token (access specifier) or a nested block.
                size_t spacePos = line.find(' ');
                if (spacePos == std::string_view::npos)
                {
                    std::string token = std::string(ParserUtilities::trim(line));
                    if (token == "public")
                        currentAccess = Access::Public;
                    else if (token == "private")
                        currentAccess = Access::Private;
                    else if (token == "protected")
                        currentAccess = Access::Protected;
                    else if (token == "destructor")
                    {
                        if (destructor.has_value())
                            throw std::runtime_error("Only one destructor is allowed per class.");
                        // Delegate to SpecialMemberFunctionParser; it will consume lines until its end marker.
                        destructor = SpecialMemberFunctionParser::parseDestructorProperties(lines);
                    }
                    else
                    {
                        throw std::runtime_error("Unknown access or block type: " + token);
                    }
                }
                else
                {
                    // Extract keyword and identifier.
                    std::string keyword = std::string(ParserUtilities::trim(line.substr(0, spacePos)));
                    std::string identifier = std::string(ParserUtilities::trim(line.substr(spacePos + 1)));
                    // Remove trailing colon from identifier if present.
                    if (!identifier.empty() && identifier.back() == ':')
                        identifier.pop_back();

                    if (keyword == "method")
                    {
                        // Delegate to CallableParser to parse the method block.
                        CallableModels::MethodModel method = CallableParser::parseMethodProperties(identifier, lines);
                        switch (currentAccess)
                        {
                        case Access::Public:
                            publicMethods.push_back(method);
                            break;
                        case Access::Protected:
                            protectedMethods.push_back(method);
                            break;
                        default:
                            // If no access specifier is currently set, default to private.
                            privateMethods.push_back(method);
                            break;
                        }
                    }
                    else if (keyword == "constructor")
                    {
                        if (identifier.empty())
                            throw std::runtime_error("Missing constructor identifier.");
                        ClassModels::Constructor ctor = SpecialMemberFunctionParser::parseConstructorProperties(identifier, lines);
                        constructors.push_back(ctor);
                    }
                    else
                    {
                        throw std::runtime_error("Unknown nested block keyword in class: " + keyword);
                    }
                }
            }
            else if (line.starts_with("|"))
            {
                validContentFound = true;
                // Process a top-level property.
                processTopLevelProperty(line, description, hasCopyAssignment, hasMoveAssignment,
                                        constructors, publicMembers, privateMembers, protectedMembers,
                                        currentAccess);
            }
            else
            {
                // If no DSL marker is recognized and no valid content has been seen yet, throw an error.
                if (!validContentFound)
                {
                    throw std::runtime_error("Malformed DSL file: unexpected line '" + std::string(line) + "'");
                }
                // Otherwise, ignore trailing garbage.
            }
        } // end while

        if (!validContentFound)
            throw std::runtime_error("Malformed DSL file: no valid DSL content found.");

        return ClassModels::ClassModel(
            className,
            description,
            constructors,
            destructor,
            publicMethods,
            privateMethods,
            protectedMethods,
            publicMembers,
            privateMembers,
            protectedMembers,
            hasCopyAssignment,
            hasMoveAssignment);
    }

} // namespace ClassParser
