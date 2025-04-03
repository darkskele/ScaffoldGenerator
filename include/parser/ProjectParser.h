/**
 * @file ProjectParser.h
 * @brief Declares the parser for project blocks in the scaffolder DSL.
 *
 * This header file declares functions to parse project blocks from the DSL.
 * A project block defines the top-level element in the DSL and creates a root folder
 * in both the include and src directories. It carries properties for versioning and
 * dependency management for CMake configuration, and it can contain nested library,
 * folder, class, namespace, and function blocks.
 *
 * Note: The project block must be the first element in the DSL file.
 */

 #pragma once

 #include "ScaffoldModels.h"   // Contains ProjectModel, FolderModel, and related models.
 #include <deque>
 #include <string_view>
 #include <string>
 
 /**
 * @namespace ProjectParser
 * @brief Contains functions to parse project blocks from the scaffolder DSL.
 *
 * The ProjectParser namespace provides functionality to process project blocks in the DSL.
 * A project block defines the top-level element in the DSL, creating a root folder in both the
 * include and src directories. It carries properties for versioning and dependency management,
 * and it can contain nested library, folder, class, namespace, and function blocks.
 *
 * Note: The project block must appear as the first element in the DSL file; otherwise, parsing will fail.
 */
 namespace ProjectParser
 {
     /**
      * @brief Parses a project block from the DSL.
      *
      * This function processes the lines representing a project block from the DSL.
      * A project block starts with a header line using the `project` keyword and is terminated
      * by an underscore (`_`). It first processes property lines (e.g., version and dependency)
      * and then parses nested blocks using the same logic as for folder parsing.
      *
      * @param projectName The identifier for the project.
      * @param lines A deque of DSL lines to be parsed.
      * @return A ProjectModel representing the parsed project, its properties, and its contained blocks.
      * @throws std::runtime_error if the DSL is malformed.
      */
     ScaffoldModels::ProjectModel parseProjectBlock(const std::string &projectName, std::deque<std::string_view>& lines);
 }
 