/**
 * @file FileNodeGenerator.h
 * @brief Defines the templated file node classes for code generation.
 *
 * This header provides the interface and templated declarations for generating code files.
 * Each file node produces both header and source file contents based on a DSL model.
 * The generated file paths are built using a unified base path. At the time of writing to disk,
 * the caller can prepend "include/" for header files and "src/" for source files.
 *
 * @note This library is part of the code generator and emphasizes modern C++20 practices.
 */

#pragma once

#include "ClassModels.h"
#include "CodeGroupModels.h"
#include "CallableModels.h"

#include <string>
#include <concepts>

/**
 * @namespace FileNodeGenerator
 * @brief Contains classes and functions for generating code files from DSL models.
 *
 * This namespace provides a templated FileNode class that implements IGeneratedFile.
 * The FileNode wraps a DSL object and uses specialized generator functions to produce
 * header and source file contents.
 */
namespace FileNodeGenerator
{

    /**
     * @struct GeneratedFiles
     * @brief Holds the generated header and source file contents along with the base file path.
     */
    struct GeneratedFiles
    {
        std::string headerContent; ///< Generated header file content.
        std::string sourceContent; ///< Generated source file content.
        std::string baseFilePath;  ///< Base relative file path (e.g., "MyProject/core/TestClass").
    };

    /**
     * @class IGeneratedFile
     * @brief Interface for file nodes that generate code files.
     *
     * This interface defines a method for generating both header and source file contents,
     * as well as for retrieving the base file path.
     */
    class IGeneratedFile
    {
    public:
        virtual ~IGeneratedFile() = default;
        /**
         * @brief Generates both header and source file contents.
         *
         * @return A GeneratedFiles struct containing the generated header content,
         *         source content, and the base file path.
         */
        virtual GeneratedFiles generateFiles() const = 0;
        /**
         * @brief Retrieves the base relative file path.
         *
         * @return The base file path as a std::string.
         */
        virtual std::string getBasePath() const = 0;
    };

    /**
     * @brief Concept that restricts file node types to those that represent actual files.
     *
     * A valid file node type must be one of:
     * - ClassModels::ClassModel
     * - CodeGroupModels::NamespaceModel
     * - std::vector<CallableModels::FunctionModel>
     *
     * Used to ensure only these types can be passed into FileNode for code generation.
     *
     * @tparam T The DSL type to test.
     */
    template <typename T>
    concept ValidFileNodeType =
        std::same_as<T, ClassModels::ClassModel> ||
        std::same_as<T, CodeGroupModels::NamespaceModel> ||
        std::same_as<T, std::vector<CallableModels::FunctionModel>>;

    /**
     * @brief Templated FileNode class for generating code files.
     *
     * The FileNode class wraps a DSL object (content) and generates both header and source file
     * contents using specialized generator functions. The final file path is computed by combining
     * the provided base path with the file name.
     *
     * @tparam T The type of the DSL object used for code generation. Must satisfy the ValidFileNodeType concept.
     */
    template <ValidFileNodeType T>
    class FileNode : public IGeneratedFile
    {
    public:
        T content;            ///< The DSL object used for code generation.
        std::string basePath; ///< The base relative path within the project (e.g., "MyProject/core").
        std::string fileName; ///< The base file name (without extension).

        /**
         * @brief Constructs a new FileNode.
         *
         * @param basePath The base relative path for the file.
         * @param fileName The base name of the file (without extension).
         * @param cont The DSL object for generating file content.
         */
        FileNode(const std::string &basePath, const std::string &fileName, T cont);

        /**
         * @brief Generates the header and source file contents.
         *
         * This method computes the base file path by combining the basePath and fileName.
         * It then generates header content (via generateHeaderContent()) and source content
         * (via generateSourceContent()). The caller is responsible for prepending "include/" and "src/".
         *
         * @return A GeneratedFiles struct containing the generated header and source content,
         *         as well as the base file path.
         */
        GeneratedFiles generateFiles() const override;

        /**
         * @brief Retrieves the base relative file path.
         *
         * @return The base relative file path as a std::string.
         */
        std::string getBasePath() const override;
    };

    /**
     * @brief Generates the header content for a DSL object.
     *
     * This function should be specialized for different DSL types.
     *
     * @tparam T The type of the DSL object.
     * @param obj The DSL object.
     * @return A std::string containing the generated header content.
     */
    template <typename T>
    std::string generateHeaderContent(const T &obj);

    /**
     * @brief Generates the source content for a DSL object.
     *
     * This function should be specialized for different DSL types.
     *
     * @tparam T The type of the DSL object.
     * @param obj The DSL object.
     * @return A std::string containing the generated source content.
     */
    template <typename T>
    std::string generateSourceContent(const T &obj);

} // namespace FileNodeGenerator

// Include inline template definitions.
#include "FileNodeGenerator.tpp"
