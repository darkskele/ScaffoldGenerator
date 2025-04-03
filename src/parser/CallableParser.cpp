#include "CallableParser.h"
#include "ScaffoldProperties.h"
#include "ParserUtilities.h"
#include "PropertiesParser.h"
#include <stdexcept>

namespace CallableParser
{
    ScaffoldModels::CallableModel parseCallableProperties(const std::string &callableName, const std::vector<std::string_view> &propertyLines)
    {
        // Initialize default properties:
        // Default return type is 'void'.
        ScaffoldProperties::DataType returnType = PropertiesParser::parseDataType("void");
        // Default parameter list is empty.
        std::vector<ScaffoldProperties::Parameter> params;
        // Default description is empty.
        std::string description;
        // Default declaration specifier (no modifiers).
        ScaffoldProperties::DeclartionSpecifier declSpec;

        // Iterate over each property line in the callable block.
        for (auto line : propertyLines)
        {
            // Trim leading and trailing whitespace.
            line = ParserUtilities::trim(line);
            // Skip empty lines or lines not starting with the expected '|' character.
            if (line.empty() || line.front() != '|')
                continue;

            // Remove the '|' prefix.
            line.remove_prefix(1);
            // Trim again after removing the prefix.
            line = ParserUtilities::trim(line);

            // Find the '=' separator to split the key and value.
            size_t equalPos = line.find('=');
            if (equalPos == std::string_view::npos)
                continue;  // Skip lines without a key-value pair.

            // Extract and trim the key and value.
            std::string_view key = ParserUtilities::trim(line.substr(0, equalPos));
            std::string_view value = ParserUtilities::trim(line.substr(equalPos + 1));

            // Process recognized keys and update properties accordingly.
            if (key == "return")
            {
                // Parse the return type.
                returnType = PropertiesParser::parseDataType(value);
            }
            else if (key == "parameters")
            {
                // Parse the parameter list.
                params = PropertiesParser::parseParameters(value);
            }
            else if (key == "description")
            {
                // If the description is enclosed in quotes, remove them.
                if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                {
                    value.remove_prefix(1);
                    value.remove_suffix(1);
                }
                description = std::string(ParserUtilities::trim(value));
            }
            else if (key == "declaration")
            {
                // Parse the declaration specifier.
                declSpec = PropertiesParser::parseDeclarationSpecifier(value);
            }
            else
            {
                // If an unrecognized property key is encountered, throw an error.
                throw std::runtime_error("Unrecognised property in callable block!");
            }
        }

        // Construct and return a CallableModel with the parsed properties.
        return ScaffoldModels::CallableModel(returnType, callableName, params, declSpec, description);
    }
} // namespace CallableParser
