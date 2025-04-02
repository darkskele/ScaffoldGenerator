#include "SpecialMemberGenerator.h"
#include "PropertiesGenerator.h"

#include <stdexcept>
#include <sstream>

namespace SpecialMemberGenerator
{
    std::string generateConstructorDeclaration(const std::string &className, const ScaffoldModels::Constructor &ctor)
    {
        std::ostringstream oss;
        oss << "    " << className << "(";

        if (ctor.type == ScaffoldModels::ConstructorType::CUSTOM)
        {
            oss << PropertiesGenerator::generateParameterList(ctor.parameters) << ");\n";
        }
        else if (ctor.type == ScaffoldModels::ConstructorType::COPY)
        {
            oss << "const " << className << "& other);\n";
        }
        else if (ctor.type == ScaffoldModels::ConstructorType::MOVE)
        {
            oss << className << "&& other) noexcept;\n";
        }
        else if (ctor.type == ScaffoldModels::ConstructorType::DEFAULT)
        {
            oss << ") = default;\n";
            oss << "    " << className << "(const " << className << "&) = default;\n";
            oss << "    " << className << "(" << className << "&&) = default;\n";
        }
        else
        {
            throw std::runtime_error("Unrecognised constructor type!");
        }

        return oss.str();
    }

    std::string generateConstructorDefinition(const std::string &className, const ScaffoldModels::Constructor &ctor,
                                              const std::vector<ScaffoldProperties::Parameter> publicMembers,
                                              const std::vector<ScaffoldProperties::Parameter> privateMembers,
                                              const std::vector<ScaffoldProperties::Parameter> protectedMembers)
    {
        // For DEFAULT constructor, no out-of-line definition is needed.
        if (ctor.type == ScaffoldModels::ConstructorType::DEFAULT)
        {
            return ""; // or a comment string if you prefer to generate something
        }

        std::ostringstream oss;
        oss << className << "::" << className << "(";

        if (ctor.type == ScaffoldModels::ConstructorType::CUSTOM)
        {
            oss << PropertiesGenerator::generateParameterList(ctor.parameters) << ")";
        }
        else if (ctor.type == ScaffoldModels::ConstructorType::COPY)
        {
            oss << "const " << className << "& other)";
        }
        else if (ctor.type == ScaffoldModels::ConstructorType::MOVE)
        {
            oss << className << "&& other) noexcept";
        }
        else
        {
            throw std::runtime_error("Unrecognised constructor type!");
        }

        // Begin the initializer list.
        std::ostringstream initList;
        bool firstInit = true;
        std::vector<std::vector<ScaffoldProperties::Parameter>> memberScopes = {publicMembers, privateMembers, protectedMembers};
        for (const auto &scope : memberScopes)
        {
            for (const auto &p : scope)
            {
                if (!firstInit)
                    initList << ", ";
                else
                    firstInit = false;
                initList << p.name << "()";
            }
        }

        if (!firstInit)
            oss << " : " << initList.str();

        // Append an empty body for the constructor definition.
        oss << "\n{\n}\n";

        return oss.str();
    }

} // namespace SpecialMemberGenerator
