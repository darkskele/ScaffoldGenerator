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

        // Parse declarators if any
        auto decl = parseTypeDeclarator(typeStr);

        // Compare the trimmed string against known types.
        if (typeStr == "void")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::VOID, quals, decl);  
        else if (typeStr == "int")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::INT, quals, decl);  
        else if (typeStr == "uint")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::UINT, quals, decl);  
        else if (typeStr == "long")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::LONG, quals, decl);  
        else if (typeStr == "ulong")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::ULONG, quals, decl);  
        else if (typeStr == "longlong")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::LONGLONG, quals, decl);  
        else if (typeStr == "ulonglong")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::ULONGLONG, quals, decl);  
        else if (typeStr == "float")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::FLOAT, quals, decl);  
        else if (typeStr == "double")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::DOUBLE, quals, decl);  
        else if (typeStr == "bool")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::BOOL, quals, decl);  
        else if (typeStr == "string")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::STRING, quals, decl);  
        else if (typeStr == "char")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::CHAR, quals, decl);        
        else if (typeStr == "auto")
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::AUTO, quals, decl); 
        else
            // If the type isn't recognized, treat it as a custom type.
            return ScaffoldProperties::DataType(ScaffoldProperties::Types::CUSTOM, std::string(typeStr), quals, decl); 
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

    ScaffoldProperties::TypeDeclarator parseTypeDeclarator(std::string_view& typeStr)
    {
        using namespace ScaffoldProperties;
        // By default there are no declarators
        TypeDeclarator tD;

        // Loop until no more declartors are available
        while(!typeStr.empty())
        {
            // Check for raw ptr in data type
            if(typeStr.back() == '*')
            {
                // Increment ptr count
                tD.ptrCount++;
                // Remove from view
                typeStr.remove_suffix(1);
            }
            // Check for references
            else if(typeStr.back() == '&')
            {
                // If already set to L value refernce, then this is the second ampersand 
                // which indicates rvalue ref
                if(tD.isLValReference)
                {
                    // Set to rval reference and unset lval ref
                    tD.isRValReference = true;
                    tD.isLValReference = false;
                    // Remove from view
                    typeStr.remove_suffix(1);
                }
                // If R value reference already set, then scaff is malformed (&&& is not a 
                // valid declarator)
                else if(tD.isRValReference)
                {
                    throw std::runtime_error("Invalid Reference configuration!");
                }
                // Else, first ampersand which equals a L val reference
                else
                {
                    // Set as L val reference
                    tD.isLValReference = true;
                    // Remove from view
                    typeStr.remove_suffix(1);
                }
            }
            // Check for array dimensions
            else if (typeStr.back() == ']') {
                // Find the matching '['.
                size_t openBracketPos = typeStr.find_last_of('[');
                if (openBracketPos == std::string_view::npos) {
                    throw std::runtime_error("Mismatched array brackets in declarator!");
                }
                // Extract the dimension between '[' and ']'
                std::string_view dim = typeStr.substr(openBracketPos + 1, typeStr.size() - openBracketPos - 2);
                
                // Validate the dimension: if non-empty, it must consist only of digits.
                if (!dim.empty()) {
                    for (char c : dim) {
                        if (!std::isdigit(c)) {
                            throw std::runtime_error("Array dimension must be a number.");
                        }
                    }
                }
                
                // Store the dimension (empty string for unsized arrays)
                tD.arrayDimensions.push_back(std::string(dim));
                // Remove the entire "[...]" from the view.
                typeStr.remove_suffix(typeStr.size() - openBracketPos);
            }
            else
            {
                // No more declarators found
                break;
            }
        }

        return tD;
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

    ScaffoldProperties::DeclartionSpecifier parseDeclarationSpecifier(std::string_view declStr)
    {
        using namespace ScaffoldProperties;
        // By default there are no declartion specifier
        DeclartionSpecifier decl{};
        declStr = ParserUtilities::trim(declStr);

        // Loop through until no more qualifiers are found 
        while(!declStr.empty())
        {
            // Check for 'static' specifier
            if(declStr.substr(0, 6) == "static")
            {
                // Add static specifier to declaration specifiers
                decl.isStatic = true;

                // Remove from view
                declStr.remove_prefix(6);
                declStr = ParserUtilities::trim(declStr);
            }
            // Check for 'inline' specifier
            else if(declStr.substr(0, 6) == "inline")
            {
                // Add inline specifier to declaration specifiers
                decl.isInline = true;

                // Remove from view
                declStr.remove_prefix(6);
                declStr = ParserUtilities::trim(declStr);
            }
            // Check for 'constexpr' specifier
            else if(declStr.substr(0, 9) == "constexpr")
            {
                // Add constexpr specifier to declaration specifiers
                decl.isConstexpr = true;

                // Remove from view
                declStr.remove_prefix(9);
                declStr = ParserUtilities::trim(declStr);
            }
            else
            {
                // No more specifiers present
                break;
            }
        }

        return decl;
    }

} // namespace PropertiesParser
