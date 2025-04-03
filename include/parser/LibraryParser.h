/**
 * @file LibraryParser.h
 * @brief Declares the parser for library blocks in the scaffolder DSL.
 *
 * This header file declares functions to parse library blocks from the DSL.
 * A library block defines a modular library that creates a folder in both the include
 * and src directories. It additionally carries properties for versioning and dependency
 * management for CMake configuration.
 *
 * Note: Libraries cannot be nested within other libraries. Attempting to nest one will result
 * in an error.
 */

 #pragma once

 #include "ScaffoldModels.h"   // Contains FolderModel and related models.
 #include <deque>
 #include <string_view>
 #include <string>
 
 /**
 * @namespace NamespaceParser
 * @brief Contains functions to parse namespace blocks from the scaffolder DSL.
 *
 * The NamespaceParser namespace provides functionality to process namespace blocks in the DSL.
 * A namespace block starts with a header line using the `namespace` keyword (optionally followed by a name)
 * and is terminated by an underscore (`_`). It may contain nested class blocks, function blocks,
 * or even other namespace blocks.
 */
 namespace LibraryParser
 {
     /**
      * @brief Parses a library block from the DSL.
      *
      * This function processes the lines representing a library block from the DSL.
      * A library block starts with a header line using the `library` keyword and is terminated
      * by an underscore (`_`). It first processes property lines (e.g., version and dependency)
      * and then parses nested blocks using folder parsing logic. All free function blocks encountered
      * are grouped together into a single vector.
      *
      * @param libraryName The identifier for the library.
      * @param lines A deque of DSL lines to be parsed.
      * @return A LibraryModel representing the parsed library, its properties, and its contained files.
      * @throws std::runtime_error if the DSL is malformed or if a nested library block is encountered.
      */
     ScaffoldModels::LibraryModel parseLibraryBlock(const std::string &libraryName, std::deque<std::string_view>& lines);
 }
 