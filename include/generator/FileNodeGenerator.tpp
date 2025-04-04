// This file contains the inline definitions for the templated FileNode class and the helper
// functions generateHeaderContent and generateSourceContent. These definitions are included
// by FileNodeGenerator.h.

#pragma once

#include "FileNodeGenerator.h"
// Include the pure generator headers.
#include "ClassGenerator.h"
#include "NamespaceGenerator.h"
#include "CallableGenerator.h"
// Also include the DSL model headers.
#include "ClassModels.h"
#include "CodeGroupModels.h"
#include "CallableModels.h"

#include <sstream> // For std::ostringstream
#include <string>

namespace FileNodeGenerator
{

    //--------------------------------------------------------------------------
    // FileNode<T> Member Function Definitions
    //--------------------------------------------------------------------------

    template <typename T>
    FileNode<T>::FileNode(const std::string &basePath, const std::string &fileName, T cont)
        : content(cont), basePath(basePath), fileName(fileName)
    {
        // Constructor initializes the basePath, fileName, and DSL content.
    }

    template <typename T>
    GeneratedFiles FileNode<T>::generateFiles() const
    {
        GeneratedFiles files;
        // Compute the base file path by combining basePath and fileName.
        files.baseFilePath = basePath + "/" + fileName;
        // Generate header content using the specialized helper function.
        files.headerContent = generateHeaderContent(content);
        // Generate source content using the specialized helper function.
        files.sourceContent = generateSourceContent(content);
        // Note: The caller will prepend "include/" or "src/" as needed when writing to disk.
        return files;
    }

    template <typename T>
    std::string FileNode<T>::getBasePath() const
    {
        return basePath;
    }

    //--------------------------------------------------------------------------
    // Default Helper Function Template Definitions
    //--------------------------------------------------------------------------

    template <typename T>
    std::string generateHeaderContent(const T &obj)
    {
        // Default stub implementation; should be specialized for each DSL type.
        return "// Default header content";
    }

    template <typename T>
    std::string generateSourceContent(const T &obj)
    {
        // Default stub implementation; should be specialized for each DSL type.
        return "// Default source content";
    }

    //--------------------------------------------------------------------------
    // Explicit Specializations for DSL Types
    //--------------------------------------------------------------------------

    // Specialization for ClassModels::ClassModel.
    template <>
    std::string generateHeaderContent<ClassModels::ClassModel>(const ClassModels::ClassModel &cl)
    {
        // Use the ClassGenerator to produce the class declaration.
        return ClassGenerator::generateClassDeclaration(cl);
    }

    template <>
    std::string generateSourceContent<ClassModels::ClassModel>(const ClassModels::ClassModel &cl)
    {
        // Use the ClassGenerator to produce the class definition.
        return ClassGenerator::generateClassDefinition(cl);
    }

    // Specialization for CodeGroupModels::NamespaceModel.
    template <>
    std::string generateHeaderContent<CodeGroupModels::NamespaceModel>(const CodeGroupModels::NamespaceModel &ns)
    {
        // Use the NamespaceGenerator to produce the namespace declaration.
        return NamespaceGenerator::generateNamespaceDeclaration(ns);
    }

    template <>
    std::string generateSourceContent<CodeGroupModels::NamespaceModel>(const CodeGroupModels::NamespaceModel &ns)
    {
        // Use the NamespaceGenerator to produce the namespace definition.
        return NamespaceGenerator::generateNamespaceDefinition(ns);
    }

    // Specialization for std::vector<CallableModels::FunctionModel> for header content.
    template <>
    std::string generateHeaderContent<std::vector<CallableModels::FunctionModel>>(const std::vector<CallableModels::FunctionModel> &funcs)
    {
        std::ostringstream oss;
        // Iterate through each free function and generate its declaration.
        for (const auto &func : funcs)
        {
            oss << CallableGenerator::generateFunctionDeclaration(func) << "\n";
        }
        return oss.str();
    }

    // Specialization for std::vector<CallableModels::FunctionModel> for source content.
    template <>
    std::string generateSourceContent<std::vector<CallableModels::FunctionModel>>(const std::vector<CallableModels::FunctionModel> &funcs)
    {
        std::ostringstream oss;
        // Iterate through each free function and generate its definition.
        for (const auto &func : funcs)
        {
            oss << CallableGenerator::generateFunctionDefinition(func) << "\n";
        }
        return oss.str();
    }

} // namespace FileNodeGenerator
