/**
 * @file ProjectMetadata.h
 * @brief Contains definitions for project and library metadata used in code generation.
 *
 * This header defines custom metadata structures that capture information about the project’s
 * directory layout and the libraries defined within the DSL. Each library’s metadata includes
 * its relative path in the directory tree, its name, whether it is a top-level library (i.e. the
 * project-level library), and its dependencies. This metadata is used during CMake file generation
 * to correctly link libraries and set up dependencies.
 *
 * @note This metadata is built during the directory tree construction phase.
 */

#pragma once

#include <string>
#include <vector>

/**
 * @namespace ProjectMetadata
 * @brief Contains metadata structures for project and library dependencies.
 *
 * The ProjectMetadata namespace defines data structures that capture information about the
 * overall project and its libraries, including each library’s relative path, name, whether it is
 * a top-level library, and its dependency list. This metadata is used during CMake file generation
 * to correctly configure and link libraries, ensuring that project-level dependencies are handled
 * appropriately.
 *
 * @note The metadata structures in this namespace are constructed during the directory tree building
 * phase from the parsed DSL.
 */
namespace ProjectMetadata
{

    /**
     * @brief Represents metadata for a single library.
     *
     * This structure captures the essential information for a library defined in the DSL,
     * including its relative path (in the directory tree), its name, whether it is a top-level library,
     * and a list of dependencies.
     */
    struct LibraryMetadata
    {
        std::string relativePath;              ///< The relative path where the library is located.
        std::string name;                      ///< The name of the library.
        bool isProjLevel;                      ///< True if the library is at the project (top) level.
        std::vector<std::string> dependencies; ///< A list of dependencies for the library.

        /**
         * @brief Constructs a new LibraryMetadata object.
         *
         * @param relativePath The relative directory path for the library.
         * @param name The name of the library.
         * @param isProjLevel True if this library is at the project level.
         * @param dependencies A vector of dependencies for the library.
         */
        LibraryMetadata(std::string relativePath, std::string name, bool isProjLevel, std::vector<std::string> dependencies)
            : relativePath(std::move(relativePath)), name(std::move(name)), isProjLevel(isProjLevel), dependencies(std::move(dependencies))
        {
        }
    };

    /**
     * @brief Represents overall project metadata.
     *
     * This structure aggregates project-wide information.
     */
    struct ProjMetadata
    {
        std::vector<LibraryMetadata> libraries; ///< Metadata for each library in the project.

        /**
         * @brief Constructs a new ProjMetadata object.
         *
         * @param libraries A vector of LibraryMetadata objects representing each library.
         */
        ProjMetadata(std::vector<LibraryMetadata> libraries)
            : libraries(std::move(libraries))
        {
        }
    };

} // namespace ProjectMetadata
