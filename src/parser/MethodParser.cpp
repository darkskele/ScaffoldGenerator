#include "MethodParser.h"
#include "ScaffoldProperties.h"
#include "ParserUtilities.h"
#include "PropertiesParser.h"
#include <stdexcept>

namespace MethodParser
{
    ScaffoldModels::MethodModel parseMethodProperties(const std::string &methodName, const std::vector<std::string_view> &propertyLines)
    {
        // Initialize default properties:
        // Return type defaults to 'void'
        ScaffoldProperties::DataType returnType = PropertiesParser::parseDataType("void");
        // Empty parameter list
        std::vector<ScaffoldProperties::Parameter> params;
        // Default method description (empty string)
        std::string description;
        // Default declaration specifier (all false)
        ScaffoldProperties::DeclartionSpecifier declSpec;

        // Iterate over each property line in the method block.
        for (auto line : propertyLines)
        {
            // Remove any leading/trailing whitespace.
            line = ParserUtilities::trim(line);
            // Skip empty lines or those that don't start with the expected '|' prefix.
            if (line.empty() || line.front() != '|')
                continue;

            // Remove the '|' prefix.
            line.remove_prefix(1);
            // Trim again after removing the prefix.
            line = ParserUtilities::trim(line);

            // Find the '=' separator to split the key and value.
            size_t equalPos = line.find('=');
            if (equalPos == std::string_view::npos)
                continue;  // Skip lines without a key-value pair

            // Extract and trim the key and value from the line.
            std::string_view key = ParserUtilities::trim(line.substr(0, equalPos));
            std::string_view value = ParserUtilities::trim(line.substr(equalPos + 1));

            // Process recognized keys and update properties accordingly.
            if (key == "return")
            {
                // Parse the return type from the value.
                returnType = PropertiesParser::parseDataType(value);
            }
            else if (key == "parameters")
            {
                // Parse the parameter list from the value.
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
                // Convert the trimmed string_view into a std::string.
                description = std::string(ParserUtilities::trim(value));
            }
            else if(key == "declaration")
            {
                // Parse the declaration specifier from the value.
                declSpec = PropertiesParser::parseDeclarationSpecifier(value);
            }
            else
            {
                // If an unrecognized property key is found, throw an error.
                throw std::runtime_error("Unrecognised property in Method block!");
            }
        }

        // Construct and return a MethodModel with the parsed properties.
        return ScaffoldModels::MethodModel(returnType, methodName, params, declSpec, description);
    }
} // namespace MethodParser
