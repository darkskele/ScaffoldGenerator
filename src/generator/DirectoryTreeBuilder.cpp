// This file provides the definitions for functions declared in DirectoryTreeBuilder.h.
// It converts DSL models (FolderModel, LibraryModel, ProjectModel) into a tree of DirectoryNode objects.
// The public function buildDirectoryTree() accepts any DirectoryBaseModel and internally calls the
// appropriate overload based on the actual type. When processing a ProjectModel, a non-null ProjectMetadata
// pointer is required.

#include "DirectoryTreeBuilder.h"

#include <stdexcept>
#include <iostream>

/**
 * @brief Internal helper functions for building the directory tree.
 *
 * This anonymous namespace contains internal helper functions used by the DirectoryTreeBuilder
 * implementation to convert DSL models into a hierarchical tree of DirectoryNode objects.
 * These functions are not part of the public API.
 */
namespace
{
    /**
     * @brief Recursively converts a FolderModel into a DirectoryNode and registers its subdirectory path with the library metadata.
     *
     * This function creates a DirectoryNode for the given folder and recursively processes its subfolders.
     * In addition to constructing the node hierarchy, it updates the provided ProjectMetadata by adding the relative
     * path of each processed folder to the subDirectories vector of the specified library (or project-level) entry.
     *
     * @param folder The FolderModel to convert.
     * @param parentPath The relative path of the parent directory.
     * @param parent A shared pointer to the parent DirectoryNode.
     * @param libName The name of the library to which this folder belongs. Use "proj" for project-level folders.
     * @param metadata Reference to the ProjectMetadata where library metadata is registered.
     * @return A shared_ptr to the DirectoryNode representing the folder.
     *
     * @todo Process folder.classFiles, folder.namespaceFiles, and folder.functionFile.
     *       For each element, create an appropriate FileNode using FileNodeGenerator,
     *       and add it to node->fileNodes via node->addFileNode().
     */
    std::shared_ptr<DirectoryTree::DirectoryNode> buildTreeImpl(const CodeGroupModels::FolderModel &folder,
                                                                const std::string &parentPath,
                                                                const std::shared_ptr<DirectoryTree::DirectoryNode> &parent,
                                                                const std::string &libName,
                                                                ProjectMetadata::ProjMetadata &metadata)
    {
        // Create a DirectoryNode for this folder.
        auto node = std::make_shared<DirectoryTree::DirectoryNode>(folder.name, parentPath, parent);

        // Register this folder's relative path under the appropriate library key.
        metadata.libraries[libName].subDirectories.emplace_back(node->relativePath);

        // Process each subfolder recursively.
        for (const auto &subFolder : folder.subFolders)
        {
            auto childNode = buildTreeImpl(subFolder, node->relativePath, node, libName, metadata);
            node->addSubDirectory(childNode);
        }

        // Process each Class Model in folder: each class forms a set of files (.h and .cpp).
        for (const auto &cl : folder.classFiles)
        {
            auto classNode = std::make_unique<FileNodeGenerator::FileNode<ClassModels::ClassModel>>(
                node->relativePath, cl.name, cl);
            node->addFileNode(std::move(classNode));
        }

        // Process each Namespace Model in folder: each namespace forms a set of files (.h and .cpp).
        for (const auto &ns : folder.namespaceFiles)
        {
            auto namespaceNode = std::make_unique<FileNodeGenerator::FileNode<CodeGroupModels::NamespaceModel>>(
                node->relativePath, ns.name, ns);
            node->addFileNode(std::move(namespaceNode));
        }

        // Process function files: the entire vector forms a set of files (.h and .cpp).
        if (folder.functionFile.size() > 0)
        {
            auto freeFunctionNode = std::make_unique<FileNodeGenerator::FileNode<std::vector<CallableModels::FunctionModel>>>(
                node->relativePath, node->folderName + "FreeFunctions", folder.functionFile);
            node->addFileNode(std::move(freeFunctionNode));
        }

        return node;
    }

    /**
     * @brief Converts a LibraryModel into a DirectoryNode and registers its metadata.
     *
     * This function converts a CodeGroupModels::LibraryModel into a DirectoryNode using the folder
     * conversion logic provided by buildTreeImpl(), and registers library metadata for later use (e.g., during CMake generation).
     * The metadata is updated to include the library's relative path and dependencies.
     *
     * @param library The LibraryModel to convert.
     * @param metadata Reference to the ProjectMetadata where this library's metadata is stored.
     * @param parentPath The relative path of the parent directory.
     * @param parent A shared pointer to the parent DirectoryNode.
     * @return A shared_ptr to the DirectoryNode representing the library.
     */
    std::shared_ptr<DirectoryTree::DirectoryNode>
    buildTreeImpl(const CodeGroupModels::LibraryModel &library,
                  ProjectMetadata::ProjMetadata &metadata,
                  const std::string &parentPath,
                  const std::shared_ptr<DirectoryTree::DirectoryNode> &parent)
    {
        // Register metadata about this library. Note: the relativePath will be updated after tree construction.
        metadata.libraries[library.name] =
            ProjectMetadata::LibraryMetadata{
                "", // Temporary; will be set after full tree construction.
                library.name,
                false, // This is a library-level (not project-level) entry.
                library.dependencies};

        // Convert the LibraryModel using folder logic.
        auto node = buildTreeImpl(static_cast<const CodeGroupModels::FolderModel &>(library), parentPath, parent, library.name, metadata);

        // Update the library metadata with the correct relative path.
        metadata.libraries[library.name].relativePath = node->relativePath;

        return node;
    }

    /**
     * @brief Builds a directory tree from a ProjectModel and registers project-level metadata.
     *
     * This function converts a DSL-defined ProjectModel into a DirectoryNode-based directory tree.
     * It processes all top-level folders, libraries, class files, namespace files, and function files,
     * recursively building a hierarchical representation of the generated project structure.
     * The project itself is treated as a top-level library ("proj") for the purposes of metadata registration.
     *
     * @param project The ProjectModel to build from.
     * @param metadata A reference to the ProjectMetadata registry where project-level and library metadata is collected.
     * @return A shared_ptr to the root DirectoryNode representing the top-level project folder.
     */
    std::shared_ptr<DirectoryTree::DirectoryNode> buildTreeImpl(const CodeGroupModels::ProjectModel &project,
                                                                ProjectMetadata::ProjMetadata &metadata)
    {
        // Create the root DirectoryNode for the project.
        auto root = std::make_shared<DirectoryTree::DirectoryNode>("ROOT");

        // Register project-level metadata using the special key "proj".
        metadata.libraries["proj"] =
            ProjectMetadata::LibraryMetadata{
                root->relativePath, // Relative path for project-level include/src folders.
                project.name,
                true, // This is a project-level entry.
                project.dependencies};

        // Process project-level subfolders.
        for (const auto &folder : project.subFolders)
        {
            auto childNode = buildTreeImpl(folder, "ROOT", root, "proj", metadata);
            root->addSubDirectory(childNode);
        }

        // Process libraries.
        for (const auto &library : project.libraries)
        {
            auto libNode = buildTreeImpl(library, metadata, "ROOT", root);
            root->addSubDirectory(libNode);
        }

        // Process each Class Model at the project level.
        for (const auto &cl : project.classFiles)
        {
            auto classNode = std::make_unique<FileNodeGenerator::FileNode<ClassModels::ClassModel>>(
                root->relativePath, cl.name, cl);
            root->addFileNode(std::move(classNode));
        }

        // Process each Namespace Model at the project level.
        for (const auto &ns : project.namespaceFiles)
        {
            auto namespaceNode = std::make_unique<FileNodeGenerator::FileNode<CodeGroupModels::NamespaceModel>>(
                root->relativePath, ns.name, ns);
            root->addFileNode(std::move(namespaceNode));
        }

        // Process project-level function files.
        if (project.functionFile.size() > 0)
        {
            auto freeFunctionNode = std::make_unique<FileNodeGenerator::FileNode<std::vector<CallableModels::FunctionModel>>>(
                root->relativePath, root->folderName + "FreeFunctions", project.functionFile);
            root->addFileNode(std::move(freeFunctionNode));
        }

        return root;
    }

} // end anonymous namespace

namespace DirectoryTreeBuilder
{
    std::shared_ptr<DirectoryTree::DirectoryNode> buildDirectoryTree(const CodeGroupModels::ProjectModel &projModel,
                                                                     ProjectMetadata::ProjMetadata *projectMeta)
    {
        // Check for null project metadata pointer.
        if (projectMeta == nullptr)
        {
            throw std::invalid_argument("Project metadata pointer cannot be null for ProjectModel.");
        }

        // Recursively build and return the directory tree.
        return buildTreeImpl(projModel, *projectMeta);
    }

} // namespace DirectoryTreeBuilder
