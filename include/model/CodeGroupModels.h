/**
 * @file CodeGroupModels.h
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This file defines the structures that represent various DSL elements such as namespaces,
 * folders, libraries, and projects. These models are used by the generator to create code from
 * the high-level DSL. The models adhere to modern C++20 practices, emphasizing immutability,
 * efficient resource management, and robust type safety.
 */

#pragma once

#include "PropertiesModels.h"
#include "CallableModels.h"
#include "ClassModels.h"

#include <vector>
#include <string>
#include <optional>

/**
 * @namespace CodeGroupModels
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This namespace includes structures that represent various DSL elements used by the code generator,
 * such as namespaces, folders, libraries, and projects. The models are designed to be immutable and
 * to facilitate efficient code generation.
 */
namespace CodeGroupModels
{
    /**
     * @brief Represents a namespace in the code generation system.
     *
     * This struct models a namespace that can contain nested classes and functions.
     * Additional nested namespaces can be added if necessary.
     */
    struct NamespaceModel
    {
        std::string name;                                     ///< The name of the namespace.
        std::string description;                              ///< A description of what the namespace represents.
        std::vector<ClassModels::ClassModel> classes;         ///< A list of classes defined within this namespace.
        std::vector<CallableModels::FunctionModel> functions; ///< A list of functions defined within this namespace.
        std::vector<NamespaceModel> namespaces;               ///< A list of nested namespaces.
    };

    /**
     * @brief Base model for directory-based code groups.
     *
     * This structure serves as the common base for DSL elements that correspond to directories
     * (i.e. folders, libraries, and projects). It contains properties shared among these models.
     */
    struct DirectoryBaseModel
    {
        std::string name; /**< The name of the directory-based code group. */

        /**
         * @brief Virtual destructor to enable safe polymorphic behavior.
         */
        virtual ~DirectoryBaseModel() = default;

        /**
         * @brief Constructs a new DirectoryBaseModel.
         *
         * @param name The name of the code group.
         */
        DirectoryBaseModel(std::string name)
            : name(std::move(name))
        {
        }
    };

    /**
     * @brief Represents a folder in the project structure.
     *
     * A folder serves as a container for files and is created in both the include and src directories.
     * The folder's nesting within the DSL determines its relative placement in the generated file system.
     * Each DSL element declared within the folder is assumed to generate its own file.
     */
    struct FolderModel : public DirectoryBaseModel
    {
        /// Subfolders contained within this folder.
        std::vector<FolderModel> subFolders;

        /// Classes defined in this folder; each will generate its own file.
        std::vector<ClassModels::ClassModel> classFiles;

        /// Namespaces defined in this folder; each will generate its own file.
        std::vector<NamespaceModel> namespaceFiles;

        // The entire vector represents a file that may contain multiple free functions.
        std::vector<CallableModels::FunctionModel> functionFile;

        /**
         * @brief Constructs a new FolderModel.
         *
         * @param name The name of the folder.
         * @param subFolders A vector of subfolders contained within this folder.
         * @param classFiles A vector of class models that will each generate a file.
         * @param namespaceFiles A vector of namespace models that will each generate a file.
         * @param functionFile A vector of functions; the vector itself represents a file containing free functions.
         */
        FolderModel(std::string name,
                    const std::vector<FolderModel> &subFolders = {},
                    const std::vector<ClassModels::ClassModel> &classFiles = {},
                    const std::vector<NamespaceModel> &namespaceFiles = {},
                    const std::vector<CallableModels::FunctionModel> &functionFile = {})
            : DirectoryBaseModel(std::move(name)),
              subFolders(subFolders),
              classFiles(classFiles),
              namespaceFiles(namespaceFiles),
              functionFile(functionFile)
        {
        }
    };

    /**
     * @brief Represents a library in the project scaffolding DSL.
     *
     * Inherits from FolderModel, so a LibraryModel creates a folder in both the include and src directories.
     * Additional properties include versioning and dependency management for CMake configuration.
     *
     * Note: Libraries cannot be nested within other libraries. Attempting to do so will result in a DSL error.
     */
    struct LibraryModel : public FolderModel
    {
        /// The version of the library.
        std::string version;
        /// A vector of dependencies (e.g., other libraries or build features such as Boost, C++20).
        std::vector<std::string> dependencies;

        /**
         * @brief Constructs a new LibraryModel.
         *
         * @param name The name of the library. This is also used as the folder name.
         * @param version The version of the library.
         * @param dependencies A vector of dependencies required by the library.
         * @param subFolders Optional subfolder models nested inside the library.
         * @param classFiles Optional class models that generate individual files.
         * @param namespaceFiles Optional namespace models that generate individual files.
         * @param functionFile Optional free function models; the vector represents a file containing functions.
         */
        LibraryModel(std::string name,
                     std::string version,
                     std::vector<std::string> dependencies,
                     const std::vector<FolderModel> &subFolders = {},
                     const std::vector<ClassModels::ClassModel> &classFiles = {},
                     const std::vector<NamespaceModel> &namespaceFiles = {},
                     const std::vector<CallableModels::FunctionModel> &functionFile = {})
            : FolderModel(std::move(name), subFolders, classFiles, namespaceFiles, functionFile),
              version(std::move(version)),
              dependencies(std::move(dependencies))
        {
        }
    };

    /**
     * @brief Represents a project in the scaffolding DSL.
     *
     * Inherits from FolderModel, so a ProjectModel creates a folder in the project root.
     * It includes versioning and dependency management for CMake configuration,
     * and additionally holds a collection of libraries.
     */
    struct ProjectModel : public FolderModel
    {
        /// The version of the project.
        std::string version;
        /// A vector of dependencies required by the project.
        std::vector<std::string> dependencies;
        /// A vector of libraries that are part of the project.
        std::vector<LibraryModel> libraries;

        /**
         * @brief Constructs a new ProjectModel.
         *
         * @param name The name of the project. This is also used as the folder name.
         * @param version The version of the project.
         * @param dependencies A vector of dependencies required by the project.
         * @param libraries A vector of LibraryModel instances that are part of the project.
         * @param subFolders Optional subfolder models nested inside the project.
         * @param classFiles Optional class models that generate individual files.
         * @param namespaceFiles Optional namespace models that generate individual files.
         * @param functionFile Optional free function models; the vector represents a file containing functions.
         */
        ProjectModel(std::string name,
                     std::string version,
                     std::vector<std::string> dependencies,
                     std::vector<LibraryModel> libraries,
                     const std::vector<FolderModel> &subFolders = {},
                     const std::vector<ClassModels::ClassModel> &classFiles = {},
                     const std::vector<NamespaceModel> &namespaceFiles = {},
                     const std::vector<CallableModels::FunctionModel> &functionFile = {})
            : FolderModel(std::move(name), subFolders, classFiles, namespaceFiles, functionFile),
              version(std::move(version)),
              dependencies(std::move(dependencies)),
              libraries(std::move(libraries))
        {
        }
    };

} // namespace CodeGroupModels
