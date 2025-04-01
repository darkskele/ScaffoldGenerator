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
} // namespace PropertiesGenerator