/**
 * @file DirectoryTreeBuilder.h
 * @brief Declares the public interface for building a directory tree from directory-based DSL models.
 *
 * This header exposes a single function to build a directory tree from any DSL model derived from
 * DirectoryBaseModel (e.g., FolderModel, LibraryModel, or ProjectModel). The public interface accepts
 * a reference to the base model and an optional pointer to a ProjectMetadata object, which is used to
 * record project-level dependencies and library metadata.
 *
 * Internally, overloaded helper functions (defined in the CPP file) handle the specifics of each derived type.
 *
 * @note NamespaceModel is not processed since it does not represent a folder.
 *
 * @see DirectoryNode, CodeGroupModels::DirectoryBaseModel, ProjectMetadata::ProjectMetadata
 */

#pragma once

#include <memory>
#include "DirectoryNode.h"
#include "CodeGroupModels.h"
#include "ProjectMetadata.h"

/**
 * @namespace DirectoryTreeBuilder
 * @brief Contains functions for building and managing the project directory tree.
 *
 * The DirectoryTreeBuilder namespace provides functions to convert DSL models (derived from
 * DirectoryBaseModel) into a hierarchical tree of DirectoryNode objects. It offers overloaded
 * helper functions that handle FolderModel, LibraryModel, and ProjectModel specifically. The
 * public interface exposes a single build function that accepts a DirectoryBaseModel reference
 * along with optional project metadata. This design encapsulates the details of constructing
 * the directory tree while enabling clients to build the tree from any directory-based DSL model.
 *
 * @note NamespaceModel is excluded since it does not map to a directory.
 */
namespace DirectoryTreeBuilder
{

    /**
     * @brief Builds the directory tree from a ProjectModel.
     *
     * This public function converts a DSL-defined ProjectModel into a DirectoryNode-based directory tree.
     * It processes all top-level folders, libraries, class files, namespace files, and function files, and
     * registers the corresponding project metadata. A valid (non-null) pointer to ProjectMetadata must be provided.
     *
     * @param projModel The ProjectModel to convert into a directory tree.
     * @param projectMeta Pointer to the ProjectMetadata registry where metadata is stored.
     * @return A shared_ptr to the root DirectoryNode representing the top-level project folder.
     * @throws std::invalid_argument if the projectMeta pointer is null.
     */
    std::shared_ptr<DirectoryTree::DirectoryNode> buildDirectoryTree(const CodeGroupModels::ProjectModel &projModel,
                                                                     ProjectMetadata::ProjMetadata *projectMeta);

} // namespace DirectoryTreeBuilder
