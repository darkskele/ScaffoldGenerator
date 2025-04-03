#include "NamespaceParser.h"
#include "ParserUtilities.h"
#include "ClassParser.h"
#include "CallableParser.h"
#include <stdexcept>
#include <deque>
#include <optional>

namespace NamespaceParser
{
    ScaffoldModels::NamespaceModel parseNamespaceBlock(const std::optional<std::string>& nsName,
                                                         std::deque<std::string_view>& lines)
    {
        std::string description;
        std::vector<ScaffoldModels::ClassModel> classes;
        std::vector<ScaffoldModels::FunctionModel> functions;
        std::vector<ScaffoldModels::NamespaceModel> nestedNamespaces;
        bool validContentFound = false;

        while (!lines.empty())
        {
            std::string_view line = ParserUtilities::trim(lines.front());
            lines.pop_front(); // Consume the line

            if (line.empty())
                continue;

            if (line == "_")
            {
                // End marker: end of this namespace block.
                validContentFound = true;
                break;
            }
            else if (!line.empty() && line.front() == '-') 
            {
                // Process a header line.
                line.remove_prefix(1); // Remove the '-' (ignoring any spaces)
                line = ParserUtilities::trim(line);
                if (!line.empty() && line.back() == ':')
                {
                    line.remove_suffix(1);
                    line = ParserUtilities::trim(line);
                }
                // Split the header into keyword and optional identifier.
                size_t spacePos = line.find(' ');
                std::string keyword, identifier;
                if (spacePos == std::string_view::npos)
                {
                    keyword = std::string(line);
                }
                else
                {
                    keyword = std::string(ParserUtilities::trim(line.substr(0, spacePos)));
                    identifier = std::string(ParserUtilities::trim(line.substr(spacePos + 1)));
                }

                if (keyword == "namespace")
                {
                    // Recursively parse nested namespace.
                    auto nestedNS = parseNamespaceBlock(identifier.empty() ? std::nullopt : std::make_optional(identifier), lines);
                    nestedNamespaces.push_back(nestedNS);
                }
                else if (keyword == "class")
                {
                    if (identifier.empty())
                        throw std::runtime_error("Class block must have an identifier.");
                    auto cls = ClassParser::parseClassBlock(identifier, lines);
                    classes.push_back(cls);
                }
                else if (keyword == "function")
                {
                    if (identifier.empty())
                        throw std::runtime_error("Function block must have an identifier.");
                    auto fn = CallableParser::parseFunctionProperties(identifier, lines);
                    functions.push_back(fn);
                }
                else if (keyword == "method")
                {
                    // Methods cannot be declared at the namespace level.
                    throw std::runtime_error("Methods cannot be declared directly in a namespace: " + identifier);
                }
                else
                {
                    throw std::runtime_error("Unknown nested block keyword in namespace: " + keyword);
                }
                validContentFound = true;
            }
            else if (line.starts_with("|"))
            {
                // Process top-level property.
                line.remove_prefix(1); // Remove '|'
                line = ParserUtilities::trim(line);
                size_t eqPos = line.find('=');
                if (eqPos == std::string_view::npos)
                    continue;
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
                else
                {
                    throw std::runtime_error("Unknown namespace property: " + std::string(key));
                }
                validContentFound = true;
            }
            else
            {
                if (!validContentFound)
                    throw std::runtime_error("Malformed DSL file in namespace block: unexpected line '" + std::string(line) + "'");
                // Otherwise, ignore trailing garbage.
            }
        } // end while

        if (!validContentFound)
            throw std::runtime_error("Malformed DSL file: no valid DSL content found in namespace block");

        return ScaffoldModels::NamespaceModel{ nsName.value_or(""), description, classes, functions, nestedNamespaces };
    }

    ScaffoldModels::NamespaceModel parseNamespaceBlock(const std::optional<std::string>& nsName,
                                                         const std::vector<std::string_view>& lines)
    {
        std::deque<std::string_view> dq(lines.begin(), lines.end());
        return parseNamespaceBlock(nsName, dq);
    }
} // namespace NamespaceParser
