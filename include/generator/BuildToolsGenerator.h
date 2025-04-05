/**
 * @file BuildToolGenerator.h
 * @brief Provides functionality to generate CMake build configuration files.
 *
 * This header declares functions within the BuildToolGenerator namespace that
 * generate CMakeLists.txt files based on the project's metadata. The generated
 * configuration includes targets for libraries and the main executable, as well
 * as proper linking and include directory settings.
 */

 #pragma once

 #include "ProjectMetadata.h"
 
 #include <string>
 
 /**
  * @namespace BuildToolGenerator
  * @brief Contains functions for generating build tool configuration files.
  *
  * The BuildToolGenerator namespace offers functionality to generate the contents of
  * CMake configuration files (CMakeLists.txt) based on the metadata provided by the
  * ProjectMetadata::ProjMetadata structure. This facilitates automated build system
  * generation in the scaffolder.
  */
 namespace BuildToolGenerator
 {
 
     /**
      * @brief Generates the content of a CMakeLists.txt file based on project metadata.
      *
      * This function takes the project metadata, which includes information about
      * libraries, dependencies, and directory structure, and produces a complete
      * CMakeLists.txt configuration as a string. The generated content defines the
      * project settings, build targets for libraries and the main executable, and the
      * linking of dependencies.
      *
      * @param projMetaData The project metadata containing library and dependency information.
      * @return A string containing the generated CMakeLists.txt content.
      */
     std::string generateCmakeLists(const ProjectMetadata::ProjMetadata& projMetaData);
 
 } // namespace BuildToolGenerator
 