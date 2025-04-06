#include "CallableParser.h"
#include "PropertiesModels.h"
#include "ParserUtilities.h"
#include "PropertiesParser.h"

#include <stdexcept>

namespace CallableParser
{
    // This function consumes property lines from the front of the deque until it is empty.
    // When a line is processed, it is popped from the deque.
    CallableModels::CallableModel parseCallableProperties(const std::string &callableName, std::deque<std::string_view> &propertyLines)
    {
        // Initialize default properties:
        // Default return type is 'void'.
        PropertiesModels::DataType returnType = PropertiesParser::parseDataType("void");
        // Default parameter list is empty.
        std::vector<PropertiesModels::Parameter> params;
        // Default description is empty.
        std::string description;
        // Default declaration specifier (no modifiers).
        PropertiesModels::DeclartionSpecifier declSpec;

        // Process each property line until the deque is empty.
        while (!propertyLines.empty())
        {
            std::string_view line = ParserUtilities::trim(propertyLines.front());
            propertyLines.pop_front(); // Consume the line

            // If we reach an end-of-scope marker, break out.
            if (line == "_")
                break;

            // Process only lines that start with the '|' character.
            if (line.empty() || line.front() != '|')
                continue;

            // Remove the '|' prefix and trim.
            line.remove_prefix(1);
            line = ParserUtilities::trim(line);

            // Find the '=' separator to split key and value.
            size_t equalPos = line.find('=');
            if (equalPos == std::string_view::npos)
                continue; // Skip lines without a key-value pair.

            // Extract and trim the key and value.
            std::string_view key = ParserUtilities::trim(line.substr(0, equalPos));
            std::string_view value = ParserUtilities::trim(line.substr(equalPos + 1));

            // Process recognized keys.
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
                // Remove surrounding quotes if present.
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
                throw std::runtime_error("Unrecognised property in callable block: " + std::string(key));
            }
        }

        return CallableModels::CallableModel(returnType, callableName, params, declSpec, description);
    }

} // namespace CallableParser
