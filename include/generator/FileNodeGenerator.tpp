#pragma once

#include "FileNodeGenerator.h"

#include <sstream>
#include <string>

namespace FileNodeGenerator
{

    // --------------------------------------------------------------------------
    // FileNode<T> Member Function Definitions
    // --------------------------------------------------------------------------

    template <typename T>
        requires ValidFileNodeType<T>
    FileNode<T>::FileNode(const std::string &basePath, const std::string &fileName, T cont)
        : content(cont), basePath(basePath), fileName(fileName)
    {
    }

    template <typename T>
        requires ValidFileNodeType<T>
    GeneratedFiles FileNode<T>::generateFiles() const
    {
        GeneratedFiles files;
        files.baseFilePath = basePath + "/" + fileName;
        files.headerContent = generateHeaderContent(content);
        files.sourceContent = generateSourceContent(content);
        return files;
    }

    template <typename T>
        requires ValidFileNodeType<T>
    std::string FileNode<T>::getBasePath() const
    {
        return basePath;
    }

    // --------------------------------------------------------------------------
    // Default Helper Function Template Definitions (fallback for missing specializations)
    // --------------------------------------------------------------------------

    // Triggers a compile-time error if instantiated without a specialization.
    template <typename T>
        requires ValidFileNodeType<T>
    std::string generateHeaderContent(const T &)
    {
        static_assert(sizeof(T) == 0, "generateHeaderContent not implemented for this DSL model type");
        return {};
    }

    // Triggers a compile-time error if instantiated without a specialization.
    template <typename T>
        requires ValidFileNodeType<T>
    std::string generateSourceContent(const T &)
    {
        static_assert(sizeof(T) == 0, "generateSourceContent not implemented for this DSL model type");
        return {};
    }

} // namespace FileNodeGenerator
