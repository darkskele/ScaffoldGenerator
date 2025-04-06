#include "PropertiesGenerator.h"
#include "GeneratorUtilities.h"

#include <format>
#include <numeric> // for std::accumulate

namespace PropertiesGenerator
{
    // This function iterates over the provided vector of parameters and builds a formatted string where
    // each parameter is represented as "type name". Parameters are separated by a comma and a space.
    std::string generateParameterList(const std::vector<PropertiesModels::Parameter> &params)
    {
        // Handle edge case: if the vector is empty, return an empty string.
        if (params.empty())
        {
            return "";
        }

        // Create a vector of formatted parameter strings ("type name").
        std::vector<std::string> formattedParams;
        formattedParams.reserve(params.size());
        for (const auto &param : params)
        {
            formattedParams.push_back(std::format("{} {}", GeneratorUtilities::dataTypeToString(param.type), param.name));
        }

        // Use std::accumulate to join the strings with ", " as a separator.
        return std::accumulate(
            std::next(formattedParams.begin()),
            formattedParams.end(),
            formattedParams.front(),
            [](const std::string &a, const std::string &b)
            {
                return a + ", " + b;
            });
    }

    // This function checks the fields of the provided declaration specifier object and collects each active
    // specifier (such as static, inline, constexpr) into a vector of strings. It then concatenates these
    // specifiers into a single space-separated string.
    std::string generateDeclarationSpecifier(const PropertiesModels::DeclartionSpecifier &dS, const bool def)
    {
        std::vector<std::string> specifiers;

        if (dS.isStatic && !def)
            specifiers.push_back("static");
        if (dS.isInline)
            specifiers.push_back("inline");
        if (dS.isConstexpr)
            specifiers.push_back("constexpr");

        // Edge case: if no specifiers are set, return an empty string.
        if (specifiers.empty())
        {
            return "";
        }

        // Use std::accumulate to concatenate specifiers with a space separator.
        std::string concatenated = std::accumulate(
            std::next(specifiers.begin()),
            specifiers.end(),
            specifiers.front(),
            [](const std::string &a, const std::string &b)
            {
                return a + " " + b;
            });

        return concatenated + " ";
    }

} // namespace PropertiesGenerator
