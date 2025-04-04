/**
 * @file TraverseAndGenerate.h
 * @brief Declares the traverseAndGenerate function for processing a directory tree.
 *
 * This header defines the interface for a function that traverses a directory tree,
 * generates file content from file nodes, and writes the files to disk using an IFileWriter.
 */

#pragma once

#include <memory>
#include "DirectoryNode.h" // Provides the DirectoryTree::DirectoryNode class.
#include "IFileWriter.h"   // Provides the GeneratedFileWriter::IFileWriter interface.

/**
 * @namespace FileGeneration
 * @brief Contains functions for traversing directory trees and generating files.
 *
 * This namespace encapsulates functionality to perform a depth-first traversal of a directory tree,
 * generate file contents from file nodes, and write the generated files to disk. The process involves:
 * - Iterating over each file node in a directory, invoking its generateFiles() method.
 * - Writing header and source files using an implementation of the GeneratedFileWriter::IFileWriter interface.
 * - Recursively processing subdirectories.
 *
 * The generated base file paths are expected to start with "ROOT/", which is removed during file writing.
 */
namespace FileGeneration
{

    /**
     * @brief Traverses the directory tree and generates files.
     *
     * This function performs a depth-first traversal of the directory tree. For each
     * directory node, it iterates over its file nodes, invokes the generateFiles() method
     * to obtain file contents, and writes the header and source files using the provided
     * IFileWriter instance. The generated base file path is assumed to start with "ROOT/",
     * which will be removed by the file writer implementation.
     *
     * @param node A shared pointer to the current DirectoryNode.
     * @param writer A reference to an implementation of IFileWriter used to write files.
     */
    void traverseAndGenerate(const std::shared_ptr<DirectoryTree::DirectoryNode> &node,
                             IFileWriter &writer);

} // namespace FileGeneration
