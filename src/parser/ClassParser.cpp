#include "ClassParser.h"
#include "ParserUtilities.h"
#include "PropertiesParser.h"
#include "MethodParser.h"
#include "SpecialMemberFunctionParser.h"

#include <stdexcept>
#include <unordered_set>
#include <optional>

namespace ClassParser
{
    using namespace ScaffoldModels;
    using namespace ScaffoldProperties;

    /**
     * @brief Parses the `assignment=` flag into copy/move assignment booleans.
     *
     * Splits the value string by commas and sets the corresponding boolean flags.
     */
    void parseAssignmentFlags(std::string_view value, bool &copy, bool &move)
    {
        auto tokens = ParserUtilities::split(value, ',');
        for (auto tok : tokens)
        {
            tok = ParserUtilities::trim(tok);
            if (tok == "copy")
                copy = true;
            else if (tok == "move")
                move = true;
            else
                throw std::runtime_error("Unknown assignment type: " + std::string(tok));
        }
    }

    // This function reads the DSL lines that define a class according to the scaffolder schema.
    // It processes top-level properties (like description, constructors, assignment, and members)
    // as well as nested blocks (methods, constructors, destructor) and access specifiers.
    ClassModel parseClassBlock(const std::string &className, const std::vector<std::string_view> &lines)
    {
        std::string description;
        bool hasCopyAssignment = false;
        bool hasMoveAssignment = false;
        std::vector<Constructor> constructors;
        std::optional<Destructor> destructor;

        std::vector<MethodModel> publicMethods, privateMethods, protectedMethods;
        std::vector<Parameter> publicMembers, privateMembers, protectedMembers;

        // Enum for tracking the current access specifier. Defaults to Private.
        enum class Access
        {
            Private,
            Public,
            Protected
        };
        Access currentAccess = Access::Private;

        // Variables for tracking nested block state
        std::string currentKeyword;                // e.g., "method", "constructor", "destructor"
        std::string blockIdentifier;               // identifier for the nested block (like method name)
        std::vector<std::string_view> blockBuffer; // buffer to hold properties within a nested block

        // Lambda to flush the current block buffer and add the parsed element to the corresponding container.
        auto flush = [&](const std::vector<std::string_view> &buffer)
        {
            if (currentKeyword == "method")
            {
                // Delegate method properties parsing to MethodParser.
                MethodModel method = MethodParser::parseMethodProperties(blockIdentifier, buffer);
                // Add method to the correct access group.
                switch (currentAccess)
                {
                case Access::Public:
                    publicMethods.push_back(method);
                    break;
                case Access::Private:
                    privateMethods.push_back(method);
                    break;
                case Access::Protected:
                    protectedMethods.push_back(method);
                    break;
                }
            }
            else if (currentKeyword == "constructor")
            {
                if (blockIdentifier.empty())
                    throw std::runtime_error("Missing constructor identifier.");
                // Delegate custom constructor parsing.
                constructors.emplace_back(
                    SpecialMemberFunctionParser::parseConstructorProperties(blockIdentifier, buffer));
            }
            else if (currentKeyword == "destructor")
            {
                // Only one destructor is allowed.
                if (destructor.has_value())
                    throw std::runtime_error("Only one destructor is allowed per class.");
                destructor = SpecialMemberFunctionParser::parseDestructorProperties(buffer);
            }

            // Clear state for the next block.
            currentKeyword.clear();
            blockIdentifier.clear();
        };

        // Flag to indicate whether any valid DSL content was encountered.
        bool validContentFound = false;

        // Process each line of the DSL input.
        for (auto line : lines)
        {
            // Trim leading and trailing whitespace.
            line = ParserUtilities::trim(line);
            if (line.empty())
                continue;

            if (line.starts_with("- "))
            {
                // Mark that we've seen valid DSL content.
                validContentFound = true;

                // Flush any pending nested block before starting a new block.
                ParserUtilities::flushBlock(blockBuffer, flush);

                // Remove the DSL block marker "- " and trim any extra spaces.
                line.remove_prefix(2);
                line = ParserUtilities::trim(line); // Extra trim to handle extra whitespace

                // Handle header lines which may indicate access specifiers or nested blocks.
                // Check for a trailing colon and remove it.
                auto colonPos = line.find(':');
                if (colonPos != std::string_view::npos)
                    line = line.substr(0, colonPos); // Remove trailing colon

                // Determine if this is a simple token (like "public") or a nested block (like "method doSomething").
                auto spacePos = line.find(' ');
                if (spacePos == std::string_view::npos)
                {
                    std::string token = std::string(ParserUtilities::trim(line));

                    // Check if the token represents an access specifier.
                    if (token == "public")
                        currentAccess = Access::Public;
                    else if (token == "private")
                        currentAccess = Access::Private;
                    else if (token == "protected")
                        currentAccess = Access::Protected;
                    // Check if it is a valid block type such as "destructor".
                    else if (token == "destructor")
                    {
                        currentKeyword = token;
                        blockIdentifier.clear();
                    }
                    else
                    {
                        // Unrecognized token in a header should throw an error.
                        throw std::runtime_error("Unknown access or block type: " + token);
                    }

                    // Reset nested block state for access specifiers.
                    if (token == "public" || token == "private" || token == "protected")
                    {
                        currentKeyword.clear();
                        blockIdentifier.clear();
                    }
                }
                else
                {
                    // This is a nested block header (e.g., "method doSomething" or "constructor custom").
                    currentKeyword = std::string(ParserUtilities::trim(line.substr(0, spacePos)));
                    blockIdentifier = std::string(ParserUtilities::trim(line.substr(spacePos + 1)));

                    // Remove any trailing colon from the identifier.
                    if (!blockIdentifier.empty() && blockIdentifier.back() == ':')
                        blockIdentifier.pop_back();
                }
            }
            else if (line == "_")
            {
                // Mark valid content and flush the current nested block.
                validContentFound = true;
                ParserUtilities::flushBlock(blockBuffer, flush);
            }
            else if (line.starts_with("|"))
            {
                // Mark valid content and process a property line.
                validContentFound = true;
                if (currentKeyword.empty())
                {
                    // Remove the leading '|' and trim whitespace.
                    line.remove_prefix(1);
                    line = ParserUtilities::trim(line);

                    // Process top-level class properties.
                    size_t eqPos = line.find('=');
                    if (eqPos == std::string_view::npos)
                        continue; // Ignore if not a valid key=value pair

                    std::string_view key = ParserUtilities::trim(line.substr(0, eqPos));
                    std::string_view value = ParserUtilities::trim(line.substr(eqPos + 1));

                    if (key == "description")
                    {
                        // Remove surrounding quotes from description.
                        if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                        {
                            value.remove_prefix(1);
                            value.remove_suffix(1);
                        }
                        description = std::string(ParserUtilities::trim(value));
                    }
                    else if (key == "constructors")
                    {
                        // Parse comma-separated list of constructor types.
                        for (auto typeStr : ParserUtilities::split(value, ','))
                        {
                            typeStr = ParserUtilities::trim(typeStr);
                            ConstructorType type;
                            if (typeStr == "default")
                                type = ConstructorType::DEFAULT;
                            else if (typeStr == "copy")
                                type = ConstructorType::COPY;
                            else if (typeStr == "move")
                                type = ConstructorType::MOVE;
                            else
                                throw std::runtime_error("Unknown constructor type: " + std::string(typeStr));

                            constructors.emplace_back(type, std::vector<Parameter>{}, "");
                        }
                    }
                    else if (key == "assignment")
                    {
                        // Parse assignment operators.
                        parseAssignmentFlags(value, hasCopyAssignment, hasMoveAssignment);
                    }
                    else if (key == "members")
                    {
                        // Parse members into the current access group.
                        std::vector<Parameter> parsed = PropertiesParser::parseParameters(value);
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
                        }
                    }
                    else
                    {
                        // Unknown top-level property leads to an error.
                        throw std::runtime_error("Unknown class-level property: " + std::string(key));
                    }
                }
                else
                {
                    // Inside a nested block: queue the property line for later processing.
                    blockBuffer.push_back(line);
                }
            }
            else
            {
                // For lines that do not match any expected DSL marker:
                // If no valid DSL content has been seen yet, throw an error.
                // Otherwise, ignore trailing garbage.
                if (!validContentFound)
                {
                    throw std::runtime_error("Malformed DSL file: unexpected line '" + std::string(line) + "'");
                }
                // Trailing garbage is ignored.
            }
        }

        // Flush any remaining buffered nested block properties.
        ParserUtilities::flushBlock(blockBuffer, flush);

        // Return a fully constructed ClassModel from the parsed data.
        return ClassModel(
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
