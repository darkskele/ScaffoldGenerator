#include "MethodParser.h"
#include "ScaffoldProperties.h"
#include "ParserUtilities.h"
#include "PropertiesParser.h"
#include <stdexcept>

namespace MethodParser
{
    ScaffoldModels::MethodModel parseMethodProperties(const std::string &methodName, const std::vector<std::string_view> &propertyLines)
    {
        // Default property values
        ScaffoldProperties::DataType returnType = PropertiesParser::parseDataType("void");
        std::vector<ScaffoldProperties::Parameter> params;
        std::string description;
        ScaffoldProperties::DeclartionSpecifier declSpec;

        // Parse method block
        for (auto line : propertyLines)
        {
            line = ParserUtilities::trim(line);
            if (line.empty() || line.front() != '|')
                continue;

            // Remove '|' prefix
            line.remove_prefix(1);
            line = ParserUtilities::trim(line);
            // Expect key value pairs with precsense of '='
            size_t equalPos = line.find('=');
            if (equalPos == std::string_view::npos)
                continue;
            std::string_view key = ParserUtilities::trim(line.substr(0, equalPos));
            std::string_view value = ParserUtilities::trim(line.substr(equalPos + 1));

            // Parse relevant properties
            if (key == "return")
            {
                returnType = PropertiesParser::parseDataType(value);
            }
            else if (key == "parameters")
            {
                params = PropertiesParser::parseParameters(value);
            }
            else if (key == "description")
            {
                if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                {
                    value.remove_prefix(1);
                    value.remove_suffix(1);
                }
                // Trim the description after removing quotes.
                description = std::string(ParserUtilities::trim(value));
            }
            else if(key == "declaration")
            {
                declSpec = PropertiesParser::parseDeclarationSpecifier(value);
            }
            else
            {
                throw std::runtime_error("Unrecognised property in Method block!");
            }
        }

        return ScaffoldModels::MethodModel(returnType, methodName, params, declSpec, description);
    }
} // namespace MethodParser
