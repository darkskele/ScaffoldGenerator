#include "PropertiesGenerator.h"
#include "GeneratorUtilities.h"
#include <format>

namespace PropertiesGenerator
{
    std::string generateParameterList(const std::vector<ScaffoldProperties::Parameter> &params)
    {
        std::string result;
        bool first = true;

        for (const auto &param : params)
        {
            // Separate parameters with comma
            if (!first)
            {
                result += ", ";
            }
            first = false;

            // Using std::format to generate "type name" string.
            result += std::format("{} {}", GeneratorUtilities::dataTypeToString(param.type), param.name);
        }

        return result;
    }

    std::string generateDeclarationSpecifier(const ScaffoldProperties::DeclartionSpecifier &dS)
    {
        std::vector<std::string> specifiers;

        if(dS.isStatic)
            specifiers.push_back("static");

        if(dS.isInline)
            specifiers.push_back("inline");

        if(dS.isConstexpr)
            specifiers.push_back("constexpr");

        std::string result = "";

        for(const auto& spec : specifiers)
        {
            result += spec + " ";
        }

        return result;
    }
    
} // namespace PropertiesGenerator