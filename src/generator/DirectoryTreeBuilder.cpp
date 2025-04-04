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
     * @brief Converts a FolderModel into a DirectoryNode.
     *
     * This function recursively converts a CodeGroupModels::FolderModel into a DirectoryTree::DirectoryNode.
     * It creates a DirectoryNode for the given folder, then processes each of its subfolders recursively,
     * adding the resulting child nodes to the parent's subDirectories vector.
     *
     * @param folder The FolderModel to convert.
     * @return A shared_ptr to the DirectoryNode representing the folder.
     *
     * @todo Process folder.classFiles, folder.namespaceFiles, and folder.functionFile.
     *       For each element, create an appropriate FileNode using FileNodeGenerator,
     *       and add it to node->fileNodes via node->addFileNode().
     */
    std::shared_ptr<DirectoryTree::DirectoryNode> buildTreeImpl(const CodeGroupModels::FolderModel &folder,
                                                                const std::string &parentPath,
                                                                const std::shared_ptr<DirectoryTree::DirectoryNode> &parent)
    {
        // Recursively converts a FolderModel into a DirectoryNode.
        // Each call constructs the current node and recurses on subfolders.
        // The relative path is computed based on parentPath and folder name,
        // and parent linkage is stored for later traversal.

        // Create a DirectoryNode for this folder.
        auto node = std::make_shared<DirectoryTree::DirectoryNode>(folder.name, parentPath, parent);

        // Process each subfolder recursively.
        for (const auto &subFolder : folder.subFolders)
        {
            auto childNode = buildTreeImpl(subFolder, node->relativePath, node);
            node->addSubDirectory(childNode);
        }

        // Process each Class Model in folder, each class will form a set of files (.h and .cpp)
        for (const auto &cl : folder.classFiles)
        {
            auto classNode = std::make_unique<FileNodeGenerator::FileNode<ClassModels::ClassModel>>(
                node->relativePath, cl.name, cl);
            node->addFileNode(std::move(classNode));
        }

        // Process each Namespace Model in folder, each namespace will form a set of files (.h and .cpp)
        for (const auto &ns : folder.namespaceFiles)
        {
            auto namespaceNode = std::make_unique<FileNodeGenerator::FileNode<CodeGroupModels::NamespaceModel>>(
                node->relativePath, ns.name, ns);
            node->addFileNode(std::move(namespaceNode));
        }

        // Process the vector of functions into a file node, the entire vector will form a set of files (.h and .cpp)
        if (folder.functionFile.size() > 0)
        {
            auto freeFunctionNode = std::make_unique<FileNodeGenerator::FileNode<std::vector<CallableModels::FunctionModel>>>(
                node->relativePath, node->folderName + "FreeFunctions", folder.functionFile);
            node->addFileNode(std::move(freeFunctionNode));
        }

        return node;
    }

    /**
     * @brief Converts a LibraryModel into a DirectoryNode.
     *
     * This function builds a directory node from a CodeGroupModels::LibraryModel.
     * It reuses folder conversion logic by calling buildTreeImpl(const FolderModel&)
     * and then optionally attaches library metadata for further use (e.g. during CMake generation).
     *
     * @param library The LibraryModel to convert.
     * @param metadata Reference to the ProjectMetadata::MetadataStore to register this library.
     * @param isTopLevel Indicates if this library is the project itself.
     * @return A shared_ptr to the DirectoryNode representing the library.
     */
    std::shared_ptr<DirectoryTree::DirectoryNode>
    buildTreeImpl(const CodeGroupModels::LibraryModel &library,
                  ProjectMetadata::ProjMetadata &metadata,
                  const std::string &parentPath,
                  const std::shared_ptr<DirectoryTree::DirectoryNode> &parent)
    {
        // Converts a LibraryModel into a DirectoryNode using folder logic,
        // and registers metadata for dependency resolution or CMake output.

        // Reuse folder build logic
        auto node = buildTreeImpl(static_cast<const CodeGroupModels::FolderModel &>(library), parentPath, parent);

        // Register metadata about this library
        metadata.libraries.emplace_back(
            ProjectMetadata::LibraryMetadata{
                node->relativePath,
                library.name,
                false, // isProjLevel - library level
                library.dependencies});

        return node;
    }

    /**
     * @brief Builds a directory tree from a ProjectModel.
     *
     * This internal helper function converts a DSL-defined ProjectModel into a DirectoryNode-based
     * directory tree. It processes all top-level folders, class files, namespace files, function files,
     * and registered libraries, recursively building a hierarchical representation of the generated
     * project structure.
     *
     * The resulting tree supports separate code generation for header and source files. The top-level
     * project node acts as the root of the generated include/src trees.
     *
     * Project-level dependencies are registered in the ProjectMetadata structure, and the project itself
     * is treated as a top-level library for purposes of CMake generation.
     *
     * @param project The ProjectModel to build from.
     * @param metadata A reference to the project metadata registry, where library metadata is collected.
     * @return A shared_ptr to the root DirectoryNode representing the top-level project folder.
     */
    std::shared_ptr<DirectoryTree::DirectoryNode> buildTreeImpl(const CodeGroupModels::ProjectModel &project,
                                                                ProjectMetadata::ProjMetadata &metadata)
    {
        // Create the root DirectoryNode for the project.
        auto root = std::make_shared<DirectoryTree::DirectoryNode>("ROOT");

        // Register metadata about this library
        metadata.libraries.emplace_back(
            ProjectMetadata::LibraryMetadata{
                root->relativePath, // Top level (include and src folders)
                project.name,
                true, // isProjLevel - project level
                project.dependencies});

        // Process project-level subfolders.
        for (const auto &folder : project.subFolders)
        {
            auto childNode = buildTreeImpl(folder, "ROOT", root);
            root->addSubDirectory(childNode);
        }

        // Process libraries.
        for (const auto &library : project.libraries)
        {
            auto libNode = buildTreeImpl(library, metadata, "ROOT", root);
            root->addSubDirectory(libNode);
        }

        // Process each Class Model in folder, each class will form a set of files (.h and .cpp)
        for (const auto &cl : project.classFiles)
        {
            auto classNode = std::make_unique<FileNodeGenerator::FileNode<ClassModels::ClassModel>>(
                root->relativePath, cl.name, cl);
            root->addFileNode(std::move(classNode));
        }

        // Process each Namespace Model in folder, each namespace will form a set of files (.h and .cpp)
        for (const auto &ns : project.namespaceFiles)
        {
            auto namespaceNode = std::make_unique<FileNodeGenerator::FileNode<CodeGroupModels::NamespaceModel>>(
                root->relativePath, ns.name, ns);
            root->addFileNode(std::move(namespaceNode));
        }

        // Process the vector of functions into a file node, the entire vector will form a set of files (.h and .cpp)
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

    // Public function: builds the directory tree from any DirectoryBaseModel.
    std::shared_ptr<DirectoryTree::DirectoryNode> buildDirectoryTree(const CodeGroupModels::ProjectModel &projModel,
                                                                     ProjectMetadata::ProjMetadata *projectMeta)
    {
        // Check for null project meta data ptr
        if (projectMeta == nullptr)
        {
            throw std::invalid_argument("Project metadata pointer cannot be null for ProjectModel.");
        }

        // Recursively build tree
        return buildTreeImpl(projModel, *projectMeta);
    }

} // namespace DirectoryTreeBuilder
