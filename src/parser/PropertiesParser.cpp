#include "PropertiesParser.h"
#include "ParserUtilities.h"
#include <stdexcept>

namespace PropertiesParser
{
    // Parse a data type from the provided string view.
    // This function trims the input and then compares it against known type strings.
    ScaffoldProperties::DataType parseDataType(std::string_view typeStr)
    {
        // Remove any leading and trailing whitespace.
        typeStr = ParserUtilities::trim(typeStr);

        // Compare the trimmed string against known types.
        if (typeStr == "void")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::VOID);
        else if (typeStr == "int")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::INT);
        else if (typeStr == "uint")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::UINT);
        else if (typeStr == "long")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::LONG);
        else if (typeStr == "ulong")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::ULONG);
        else if (typeStr == "longlong")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::LONGLONG);
        else if (typeStr == "ulonglong")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::ULONGLONG);
        else if (typeStr == "float")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::FLOAT);
        else if (typeStr == "double")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::DOUBLE);
        else if (typeStr == "bool")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::BOOL);
        else if (typeStr == "string")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::STRING);
        else if (typeStr == "char")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::CHAR);
        else
            // If the type isn't recognized, treat it as a custom type.
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::CUSTOM, std::string(typeStr));
    }

    // Parse a comma-separated list of parameters from the provided string view.
    // Expected format: "param1:int, param2:float"
    std::vector<ScaffoldProperties::Parameter> parseParameters(std::string_view paramStr)
    {
        std::vector<ScaffoldProperties::Parameter> params;
        // Trim the entire parameter string first.
        paramStr = ParserUtilities::trim(paramStr);

        // Loop until the entire string has been processed.
        while (!paramStr.empty())
        {
            // Find the position of the next comma.
            size_t commaPos = paramStr.find(',');
            std::string_view token;
            if (commaPos == std::string_view::npos)
            {
                // If no comma is found, the remainder is one token.
                token = paramStr;
                paramStr.remove_prefix(paramStr.size());
            }
            else
            {
                // Extract the token before the comma.
                token = paramStr.substr(0, commaPos);
                // Remove the processed token and the comma from the string.
                paramStr.remove_prefix(commaPos + 1);
            }

            // Trim whitespace from the token.
            token = ParserUtilities::trim(token);
            // Find the colon that separates the parameter name from its type.
            size_t colonPos = token.find(':');
            if (colonPos == std::string_view::npos)
            {
                // If no colon is found, the format is invalid.
                throw std::runtime_error("Invalid parameter format; expected 'name:type'.");
            }

            // Extract and trim the parameter name.
            std::string_view paramName = ParserUtilities::trim(token.substr(0, colonPos));
            // Extract and trim the parameter type.
            std::string_view paramType = ParserUtilities::trim(token.substr(colonPos + 1));

            // Create a Parameter object and add it to the list.
            params.emplace_back(ScaffoldProperties::Parameter(parseDataType(paramType), std::string(paramName)));
        }

        return params;
    }

} // namespace PropertiesParser
