/**
 * @file FolderParser.h
 * @brief Declares the parser for folder blocks in the scaffolder DSL.
 *
 * This header file declares functions to parse folder blocks from the DSL.
 * A folder block defines a container for files that are generated in both the include
 * and src directories. Each DSL element declared within a folder (such as a class, namespace,
 * or free function) is assumed to generate its own file. In the case of free functions, all
 * function blocks encountered in the folder are grouped together into a single file.
 */

#pragma once

#include "CodeGroupModels.h" // Contains FolderModel and related models.

#include <deque>
#include <string_view>
#include <string>

/**
 * @namespace FolderParser
 * @brief Contains functions to parse folder blocks from the scaffolder DSL.
 *
 * The FolderParser namespace provides functionality to process folder blocks in the DSL.
 * A folder block starts with a header line using the `folder` keyword and is terminated by
 * an underscore (`_`). It can include nested folder blocks, class blocks, namespace blocks,
 * or free function blocks. In this implementation, all free function blocks are collected into
 * a single vector, which will be output as one file.
 */
namespace FolderParser
{
    /**
     * @brief Parses a folder block from the DSL.
     *
     * This function processes the lines representing a folder block from the DSL.
     * A folder block starts with a header line using the `folder` keyword and is terminated
     * by an underscore (`_`). The block may contain nested folder blocks, class blocks, namespace
     * blocks, or free function blocks. All free function blocks encountered are grouped together
     * into a single vector.
     *
     * @param folderName The identifier for the folder.
     * @param lines A deque of DSL lines to be parsed.
     * @return A FolderModel representing the parsed folder and its contained files.
     * @throws std::runtime_error if the DSL is malformed.
     */
    CodeGroupModels::FolderModel parseFolderBlock(const std::string &folderName, std::deque<std::string_view> &lines);
}
