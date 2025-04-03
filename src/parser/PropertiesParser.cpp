#include "PropertiesParser.h"
#include "ParserUtilities.h"

#include <stdexcept>
#include <cctype>

namespace PropertiesParser
{
    // Parse a data type from the provided string view.
    PropertiesModels::DataType parseDataType(std::string_view typeStr)
    {
        // Trim whitespace from the input.
        typeStr = ParserUtilities::trim(typeStr);

        // Extract any type qualifiers from the beginning.
        auto quals = parseTypeQualifier(typeStr);

        // Extract any type declarators (pointers, references, arrays) from the end.
        auto decl = parseTypeDeclarator(typeStr);

        // Compare the remaining string against known type names.
        if (typeStr == "void")
            return PropertiesModels::DataType(PropertiesModels::Types::VOID, quals, decl);
        else if (typeStr == "int")
            return PropertiesModels::DataType(PropertiesModels::Types::INT, quals, decl);
        else if (typeStr == "uint")
            return PropertiesModels::DataType(PropertiesModels::Types::UINT, quals, decl);
        else if (typeStr == "long")
            return PropertiesModels::DataType(PropertiesModels::Types::LONG, quals, decl);
        else if (typeStr == "ulong")
            return PropertiesModels::DataType(PropertiesModels::Types::ULONG, quals, decl);
        else if (typeStr == "longlong")
            return PropertiesModels::DataType(PropertiesModels::Types::LONGLONG, quals, decl);
        else if (typeStr == "ulonglong")
            return PropertiesModels::DataType(PropertiesModels::Types::ULONGLONG, quals, decl);
        else if (typeStr == "float")
            return PropertiesModels::DataType(PropertiesModels::Types::FLOAT, quals, decl);
        else if (typeStr == "double")
            return PropertiesModels::DataType(PropertiesModels::Types::DOUBLE, quals, decl);
        else if (typeStr == "bool")
            return PropertiesModels::DataType(PropertiesModels::Types::BOOL, quals, decl);
        else if (typeStr == "string")
            return PropertiesModels::DataType(PropertiesModels::Types::STRING, quals, decl);
        else if (typeStr == "char")
            return PropertiesModels::DataType(PropertiesModels::Types::CHAR, quals, decl);
        else if (typeStr == "auto")
            return PropertiesModels::DataType(PropertiesModels::Types::AUTO, quals, decl);
        else
            // If no known type matches, treat it as a custom type.
            return PropertiesModels::DataType(PropertiesModels::Types::CUSTOM, std::string(typeStr), quals, decl);
    }

    // Parse type qualifiers from the provided string view.
    PropertiesModels::TypeQualifier parseTypeQualifier(std::string_view &qualStr)
    {
        using namespace PropertiesModels;
        // Start with no qualifiers.
        TypeQualifier quals = TypeQualifier::NONE;

        // Loop to extract all leading qualifiers.
        while (!qualStr.empty())
        {
            // Check if the string starts with "const".
            if (qualStr.substr(0, 5) == "const")
            {
                // Add the CONST flag.
                quals = quals | TypeQualifier::CONST;
                // Remove the parsed qualifier and trim.
                qualStr.remove_prefix(5);
                qualStr = ParserUtilities::trim(qualStr);
            }
            // Check if the string starts with "volatile".
            else if (qualStr.substr(0, 8) == "volatile")
            {
                // Add the VOLATILE flag.
                quals = quals | TypeQualifier::VOLATILE;
                // Remove the parsed qualifier and trim.
                qualStr.remove_prefix(8);
                qualStr = ParserUtilities::trim(qualStr);
            }
            else
            {
                // No further qualifiers found.
                break;
            }
        }

        return quals;
    }

    // Parse type declarators from the provided string view.
    PropertiesModels::TypeDeclarator parseTypeDeclarator(std::string_view &typeStr)
    {
        using namespace PropertiesModels;
        // Initialize an empty TypeDeclarator.
        TypeDeclarator tD;

        // Loop to process all declarator tokens from the end of the string.
        while (!typeStr.empty())
        {
            // Check if the last character is a pointer symbol '*'.
            if (typeStr.back() == '*')
            {
                // Increase pointer count.
                tD.ptrCount++;
                // Remove the '*' from the end.
                typeStr.remove_suffix(1);
            }
            // Check if the last character indicates a reference.
            else if (typeStr.back() == '&')
            {
                // If an lvalue reference is already detected, this '&' indicates an rvalue reference.
                if (tD.isLValReference)
                {
                    tD.isRValReference = true;
                    tD.isLValReference = false;
                    typeStr.remove_suffix(1);
                }
                // If already an rvalue reference is set, further '&' is invalid.
                else if (tD.isRValReference)
                {
                    throw std::runtime_error("Invalid reference configuration: too many '&' symbols.");
                }
                else
                {
                    // Set as an lvalue reference.
                    tD.isLValReference = true;
                    typeStr.remove_suffix(1);
                }
            }
            // Check for array declarators.
            else if (typeStr.back() == ']')
            {
                // Find the matching '['.
                size_t openBracketPos = typeStr.find_last_of('[');
                if (openBracketPos == std::string_view::npos)
                {
                    throw std::runtime_error("Mismatched array brackets in declarator.");
                }
                // Extract the dimension string between '[' and ']'.
                std::string_view dim = typeStr.substr(openBracketPos + 1, typeStr.size() - openBracketPos - 2);
                // Validate the dimension if non-empty (must be numeric).
                if (!dim.empty())
                {
                    for (char c : dim)
                    {
                        if (!std::isdigit(c))
                        {
                            throw std::runtime_error("Array dimension must be a number.");
                        }
                    }
                }
                // Save the array dimension (empty for unsized arrays).
                tD.arrayDimensions.push_back(std::string(dim));
                // Remove the entire array declarator from the string.
                typeStr.remove_suffix(typeStr.size() - openBracketPos);
            }
            else
            {
                // No more declarators detected.
                break;
            }
        }

        return tD;
    }

    // Parse a comma-separated list of parameters from the provided string view.
    std::vector<PropertiesModels::Parameter> parseParameters(std::string_view paramStr)
    {
        std::vector<PropertiesModels::Parameter> params;
        // Trim whitespace from the entire parameter string.
        paramStr = ParserUtilities::trim(paramStr);

        // Process each parameter token until the string is exhausted.
        while (!paramStr.empty())
        {
            // Find the next comma that separates parameters.
            size_t commaPos = paramStr.find(',');
            std::string_view token;
            if (commaPos == std::string_view::npos)
            {
                // If no comma, the remaining string is one token.
                token = paramStr;
                paramStr.remove_prefix(paramStr.size());
            }
            else
            {
                // Extract the token before the comma.
                token = paramStr.substr(0, commaPos);
                // Remove the token and the comma from the parameter string.
                paramStr.remove_prefix(commaPos + 1);
            }

            // Trim the token.
            token = ParserUtilities::trim(token);
            // Find the colon that separates the parameter name and its type.
            size_t colonPos = token.find(':');
            if (colonPos == std::string_view::npos)
            {
                // Malformed parameter if colon is missing.
                throw std::runtime_error("Invalid parameter format; expected 'name:type'.");
            }

            // Extract and trim the parameter name.
            std::string_view paramName = ParserUtilities::trim(token.substr(0, colonPos));
            // Extract and trim the parameter type.
            std::string_view paramType = ParserUtilities::trim(token.substr(colonPos + 1));

            // Create a Parameter object and add it to the list.
            params.emplace_back(PropertiesModels::Parameter(parseDataType(paramType), std::string(paramName)));
        }

        return params;
    }

    // Parse declaration specifiers from the provided string view.
    PropertiesModels::DeclartionSpecifier parseDeclarationSpecifier(std::string_view declStr)
    {
        using namespace PropertiesModels;
        // Initialize an empty declaration specifier.
        DeclartionSpecifier decl{};
        // Trim whitespace from the input.
        declStr = ParserUtilities::trim(declStr);

        // Loop to extract all declaration specifiers.
        while (!declStr.empty())
        {
            // Check for "static" specifier.
            if (declStr.substr(0, 6) == "static")
            {
                decl.isStatic = true;
                declStr.remove_prefix(6);
                declStr = ParserUtilities::trim(declStr);
            }
            // Check for "inline" specifier.
            else if (declStr.substr(0, 6) == "inline")
            {
                decl.isInline = true;
                declStr.remove_prefix(6);
                declStr = ParserUtilities::trim(declStr);
            }
            // Check for "constexpr" specifier.
            else if (declStr.substr(0, 9) == "constexpr")
            {
                decl.isConstexpr = true;
                declStr.remove_prefix(9);
                declStr = ParserUtilities::trim(declStr);
            }
            else
            {
                // No further specifiers found.
                break;
            }
        }

        return decl;
    }

} // namespace PropertiesParser
