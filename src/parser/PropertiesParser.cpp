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

        // Parse qualifiers if any
        auto quals = parseTypeQualifier(typeStr);

        // Compare the trimmed string against known types.
        if (typeStr == "void")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::VOID, quals);  
        else if (typeStr == "int")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::INT, quals);  
        else if (typeStr == "uint")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::UINT, quals);  
        else if (typeStr == "long")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::LONG, quals);  
        else if (typeStr == "ulong")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::ULONG, quals);  
        else if (typeStr == "longlong")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::LONGLONG, quals);  
        else if (typeStr == "ulonglong")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::ULONGLONG, quals);  
        else if (typeStr == "float")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::FLOAT, quals);  
        else if (typeStr == "double")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::DOUBLE, quals);  
        else if (typeStr == "bool")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::BOOL, quals);  
        else if (typeStr == "string")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::STRING, quals);  
        else if (typeStr == "char")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::CHAR, quals);        
        else if (typeStr == "auto")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::AUTO, quals);
        else
            // If the type isn't recognized, treat it as a custom type.
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::CUSTOM, std::string(typeStr), quals);
    }

    // Checks for type qualifiers in the provided string view and parses if found. 
    ScaffoldProperties::TypeQualifier parseTypeQualifier(std::string_view& qualStr)
    {
        using namespace ScaffoldProperties;
        // By default there are no qualifiers
        TypeQualifier quals = TypeQualifier::NONE;

        // Loop through until no more qualifiers are found 
        while(!qualStr.empty())
        {
            // Check for 'const' qualifier
            if(qualStr.substr(0, 5) == "const")
            {
                // Add const qualifier to type qualifiers
                quals = quals | TypeQualifier::CONST;

                // Remove from view
                qualStr.remove_prefix(5);
                qualStr = ParserUtilities::trim(qualStr);
            }
            // Check for 'volatile' qualifier
            else if(qualStr.substr(0, 8) == "volatile")
            {
                // Add volatile qualifier to type qualifiers
                quals = quals | TypeQualifier::VOLATILE;

                // Remove from view
                qualStr.remove_prefix(8);
                qualStr = ParserUtilities::trim(qualStr);
            }
            else
            {
                // No more qualifiers present
                break;
            }
        }

        return quals;
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
