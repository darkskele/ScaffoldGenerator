#include "PropertiesGenerator.h"
#include "GeneratorUtilities.h"
#include <format>

namespace PropertiesGenerator
{
    // This function iterates over the provided vector of parameters and builds a formatted string where
    // each parameter is represented as "type name". Parameters are separated by a comma and a space.
    std::string generateParameterList(const std::vector<ScaffoldProperties::Parameter> &params)
    {
        std::string result; // Final string to accumulate the parameter list
        bool first = true;  // Flag to avoid inserting a comma before the first parameter

        // Iterate over each parameter in the vector
        for (const auto &param : params)
        {
            // If not the first parameter, prepend a comma and a space for separation.
            if (!first)
            {
                result += ", ";
            }
            first = false;

            // Format the parameter as "type name" using std::format and append to the result.
            result += std::format("{} {}", GeneratorUtilities::dataTypeToString(param.type), param.name);
        }

        return result;
    }

    // This function checks the fields of the provided declaration specifier object and collects each active
    // specifier (such as static, inline, constexpr) into a vector of strings. It then concatenates these
    // specifiers into a single space-separated string.
    std::string generateDeclarationSpecifier(const ScaffoldProperties::DeclartionSpecifier &dS)
    {
        std::vector<std::string> specifiers; // Temporary vector to hold each individual specifier

        // Check for 'static' specifier and add if applicable.
        if (dS.isStatic)
            specifiers.push_back("static");

        // Check for 'inline' specifier and add if applicable.
        if (dS.isInline)
            specifiers.push_back("inline");

        // Check for 'constexpr' specifier and add if applicable.
        if (dS.isConstexpr)
            specifiers.push_back("constexpr");

        std::string result; // String to accumulate the final result

        // Concatenate each specifier, followed by a space.
        for (const auto &spec : specifiers)
        {
            result += spec + " ";
        }

        return result;
    }

} // namespace PropertiesGenerator
