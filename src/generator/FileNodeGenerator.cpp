#include "FileNodeGenerator.h"
#include "ClassGenerator.h"
#include "NamespaceGenerator.h"
#include "CallableGenerator.h"

#include <sstream>

namespace FileNodeGenerator
{

    // --------------------------------------------------------------------------
    // Explicit Specializations for DSL Types
    // --------------------------------------------------------------------------

    // Specialization for generating header content from a ClassModel.
    // Uses the ClassGenerator to create the class declaration.
    template <>
    std::string generateHeaderContent<ClassModels::ClassModel>(const ClassModels::ClassModel &cl)
    {
        return ClassGenerator::generateClassDeclaration(cl);
    }

    // Specialization for generating source content from a ClassModel.
    // Uses the ClassGenerator to create the class definition.
    template <>
    std::string generateSourceContent<ClassModels::ClassModel>(const ClassModels::ClassModel &cl)
    {
        return ClassGenerator::generateClassDefinition(cl);
    }

    // Specialization for generating header content from a NamespaceModel.
    // Uses the NamespaceGenerator to emit the namespace declaration.
    template <>
    std::string generateHeaderContent<CodeGroupModels::NamespaceModel>(const CodeGroupModels::NamespaceModel &ns)
    {
        return NamespaceGenerator::generateNamespaceDeclaration(ns);
    }

    // Specialization for generating source content from a NamespaceModel.
    // Uses the NamespaceGenerator to emit the namespace definition.
    template <>
    std::string generateSourceContent<CodeGroupModels::NamespaceModel>(const CodeGroupModels::NamespaceModel &ns)
    {
        return NamespaceGenerator::generateNamespaceDefinition(ns);
    }

    // Specialization for generating header content for a vector of free-standing functions.
    // Each function in the vector will be turned into a forward declaration.
    template <>
    std::string generateHeaderContent<std::vector<CallableModels::FunctionModel>>(const std::vector<CallableModels::FunctionModel> &funcs)
    {
        std::ostringstream oss;
        for (const auto &func : funcs)
        {
            oss << CallableGenerator::generateFunctionDeclaration(func) << "\n";
        }
        return oss.str();
    }

    // Specialization for generating source content for a vector of free-standing functions.
    // Each function in the vector will be turned into a definition.
    template <>
    std::string generateSourceContent<std::vector<CallableModels::FunctionModel>>(const std::vector<CallableModels::FunctionModel> &funcs)
    {
        std::ostringstream oss;
        for (const auto &func : funcs)
        {
            oss << CallableGenerator::generateFunctionDefinition(func) << "\n";
        }
        return oss.str();
    }

} // namespace FileNodeGenerator
