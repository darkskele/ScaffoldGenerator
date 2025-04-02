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

    std::string generateDestructorDeclaration(const std::string& className)
    {
        std::ostringstream oss;
        // Build the destructor declaration.
        // This generates a declaration like:
        // "    ~MyClass() = default;"
        oss << "    ~" << className << "() = default;";
        return oss.str();
    }

    std::string generateDestructorDefinition(const std::string& className)
    {
        // For default destructors, no out-of-line definition is needed.
        // Return an empty string as a placeholder.
        return "";
    }

    std::string generateMoveAssignmentDeclaration(const std::string& className)
    {
        std::ostringstream oss;
        // Build move assignment operator declaration.
        // This creates a declaration of the form:
        // MyClass& operator=(MyClass&& other) noexcept;
        oss << "    " << className << "& operator=(" << className << "&& other) noexcept;";
        return oss.str();
    }

    std::string generateMoveAssignmentDefinition(const std::string& className)
    {
        std::ostringstream oss;
        // Start constructing the move assignment operator definition.
        // The generated signature will be:
        // MyClass& MyClass::operator=(MyClass&& other) noexcept {
        oss << className << "& " << className << "::operator=(" << className << "&& other) noexcept {\n";
        // Insert a placeholder body that indicates the method is not yet implemented.
        oss << "    // Placeholder implementation: move assignment operator is not implemented.\n";
        oss << "    throw std::runtime_error(\"Not implemented\");\n";
        oss << "}\n";
        return oss.str();
    }

    std::string generateCopyAssignmentDeclaration(const std::string& className)
    {
        std::ostringstream oss;
        // Build copy assignment operator declaration.
        // This creates a declaration of the form:
        // MyClass& operator=(const MyClass& other);
        oss << "    " << className << "& operator=(const " << className << "& other);";
        return oss.str();
    }

    std::string generateCopyAssignmentDefinition(const std::string& className)
    {
        std::ostringstream oss;
        // Start constructing the copy assignment operator definition.
        // The generated signature will be:
        // MyClass& MyClass::operator=(const MyClass& other) {
        oss << className << "& " << className << "::operator=(const " << className << "& other) {\n";
        // Insert a placeholder body that indicates the method is not yet implemented.
        oss << "    // Placeholder implementation: copy assignment operator is not implemented.\n";
        oss << "    throw std::runtime_error(\"Not implemented\");\n";
        oss << "}\n";
        return oss.str();
    }

} // namespace SpecialMemberGenerator
