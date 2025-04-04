#include "NamespaceGenerator.h"
#include "ClassGenerator.h"
#include "CallableGenerator.h"
#include "GeneratorUtilities.h"

#include <sstream>

namespace NamespaceGenerator
{
    std::string generateNamespaceDeclaration(const CodeGroupModels::NamespaceModel &ns)
    {
        std::ostringstream oss;

        // If a description is provided, generate a Doxygen comment.
        if (!ns.description.empty())
        {
            oss << "/**\n"
                << " * @brief " << ns.description << "\n"
                << " */\n";
        }

        // Generate the namespace header.
        if (ns.name.empty())
        {
            // Anonymous namespace.
            oss << "namespace {\n";
        }
        else
        {
            oss << "namespace " << ns.name << " {\n";
        }

        // Generate declarations for nested classes.
        for (const auto &cls : ns.classes)
        {
            oss << ClassGenerator::generateClassDeclaration(cls) << "\n";
        }

        // Generate declarations for free functions.
        for (const auto &fn : ns.functions)
        {
            oss << CallableGenerator::generateFunctionDeclaration(fn) << "\n";
        }

        // Recursively generate declarations for nested namespaces.
        for (const auto &nestedNS : ns.namespaces)
        {
            oss << generateNamespaceDeclaration(nestedNS) << "\n";
        }

        // Close the namespace.
        oss << "} // namespace " << (ns.name.empty() ? "(anonymous)" : ns.name) << "\n";
        return oss.str();
    }

    std::string generateNamespaceDefinition(const CodeGroupModels::NamespaceModel &ns)
    {
        std::ostringstream oss;

        // Start the namespace definition block.
        if (ns.name.empty())
        {
            oss << "namespace {\n";
        }
        else
        {
            oss << "namespace " << ns.name << " {\n";
        }

        // Generate definitions for nested classes.
        std::ostringstream innerOss;
        for (const auto &cls : ns.classes)
        {
            innerOss << ClassGenerator::generateClassDefinition(cls) << "\n";
        }

        // Generate definitions for free functions.
        for (const auto &fn : ns.functions)
        {
            innerOss << CallableGenerator::generateFunctionDefinition(fn) << "\n";
        }

        // Recursively generate definitions for nested namespaces.
        for (const auto &nestedNS : ns.namespaces)
        {
            innerOss << generateNamespaceDefinition(nestedNS) << "\n";
        }

        // Indent the inner code
        std::string indentedInner = GeneratorUtilities::indentCode(innerOss.str());
        oss << indentedInner;

        // Close the namespace block.
        oss << "} // namespace " << (ns.name.empty() ? "(anonymous)" : ns.name) << "\n";

        return oss.str();
    }

} // namespace NamespaceGenerator
