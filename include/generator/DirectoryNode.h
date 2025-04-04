/**
 * @file DirectoryNode.h
 * @brief Defines the DirectoryNode class for the project directory tree.
 *
 * This file declares the DirectoryNode class, which represents a directory (folder)
 * in the generated project structure. Each DirectoryNode holds its folder name, computed
 * relative path, a weak pointer to its parent node, a collection of child directory nodes,
 * and a collection of file nodes. File nodes are stored as unique pointers to IGeneratedFile,
 * allowing heterogeneous FileNode objects (from different DSL models) to reside in the same container.
 *
 * The directory tree is built on the heap using shared_ptr for node ownership, while
 * each node's parent is held as a weak_ptr to avoid cyclic references.
 *
 * @note This file is part of the code generator and adheres to modern C++20 practices.
 */

#pragma once

#include <memory>
#include <vector>
#include <string>
#include "FileNodeGenerator.h" // Provides IGeneratedFile and GeneratedFiles

/**
 * @namespace DirectoryTree
 * @brief Contains classes and functions for building and managing the project directory tree.
 *
 * This namespace provides the necessary classes for constructing a hierarchical representation
 * of the generated project's directory structure. The core class, DirectoryNode, represents a folder
 * in the project and maintains:
 * - The folder's name and computed relative path.
 * - A weak pointer to its parent node (to avoid cyclic dependencies).
 * - A collection of child DirectoryNode objects (using shared pointers).
 * - A collection of file nodes (stored as unique pointers to IGeneratedFile) for the files generated
 *   from DSL models.
 *
 * The DirectoryTree namespace is designed to support efficient file generation and cleanup in large projects,
 * ensuring that file content is streamed to disk and memory is released promptly.
 *
 * @note This namespace leverages modern C++20 smart pointer facilities (e.g., std::shared_ptr, std::unique_ptr,
 * and std::weak_ptr) along with std::enable_shared_from_this to safely manage the directory tree hierarchy.
 */
namespace DirectoryTree
{

    /**
     * @class DirectoryNode
     * @brief Represents a node in the project directory tree.
     *
     * Each DirectoryNode corresponds to a folder in the generated project. It maintains:
     * - The folder name.
     * - The computed relative path from the project root.
     * - A weak pointer to its parent DirectoryNode (nullptr for the root).
     * - A collection of child DirectoryNode objects (stored as shared_ptr).
     * - A collection of file nodes (stored as unique_ptr to IGeneratedFile).
     */
    class DirectoryNode : public std::enable_shared_from_this<DirectoryNode>
    {
    public:
        std::string folderName;              ///< The name of this directory.
        std::string relativePath;            ///< The computed relative path from the project root.
        std::weak_ptr<DirectoryNode> parent; ///< Weak pointer to the parent DirectoryNode (empty for root).

        /**
         * @brief Constructs a new DirectoryNode.
         *
         * @param folderName The name of the directory.
         * @param parentPath The relative path of the parent directory (empty for root).
         * @param parent A shared_ptr to the parent DirectoryNode (nullptr for root).
         */
        DirectoryNode(const std::string &folderName,
                      const std::string &parentPath = "",
                      std::shared_ptr<DirectoryNode> parent = nullptr);

        /**
         * @brief Adds a child directory node.
         *
         * Adds a directory node to the sub directories vector.
         *
         * @param dirNode The sub directory to add.
         */
        void addSubDirectory(const std::shared_ptr<DirectoryNode> &dirNode);

        /**
         * @brief Adds a file node to this directory.
         *
         * Adds the given file node (allocated on the heap) to the directory's collection of file nodes.
         *
         * @param fileNode A unique_ptr to an IGeneratedFile representing a file.
         */
        void addFileNode(std::unique_ptr<FileNodeGenerator::IGeneratedFile> fileNode);

        /**
         * @brief Retrieves the parent directory node.
         *
         * @return A shared_ptr to the parent DirectoryNode (nullptr if this is the root).
         */
        std::shared_ptr<DirectoryNode> getParent() const;

        /**
         * @brief Returns the list of child subdirectories.
         *
         * @return A const reference to the vector of shared pointers to child DirectoryNodes.
         */
        const std::vector<std::shared_ptr<DirectoryNode>> &getSubDirectories() const;

        /**
         * @brief Returns the list of file nodes.
         *
         * @return A const reference to the vector of unique pointers to file nodes.
         */
        const std::vector<std::unique_ptr<FileNodeGenerator::IGeneratedFile>> &getFileNodes() const;

    private:
        /**
         * @brief Container for child directory nodes.
         *
         * Each child is stored as a shared_ptr to a DirectoryNode.
         */
        std::vector<std::shared_ptr<DirectoryNode>> subDirectories;

        /**
         * @brief Container for file nodes in this directory.
         *
         * File nodes are stored polymorphically as unique_ptr to IGeneratedFile.
         */
        std::vector<std::unique_ptr<FileNodeGenerator::IGeneratedFile>> fileNodes;
    };

} // namespace DirectoryTree
