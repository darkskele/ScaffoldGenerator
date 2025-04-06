/**
 * @file ProjectMetadata.h
 * @brief Contains definitions for project and library metadata used in code generation.
 *
 * This header defines custom metadata structures that capture information about the project’s
 * directory layout and the libraries defined within the DSL. Each library’s metadata includes
 * its relative path in the directory tree, its name, whether it is a top-level library (i.e. the
 * project-level library), its dependencies, and a list of subdirectories for CMake file generation.
 *
 * @note This metadata is built during the directory tree construction phase.
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

/**
 * @namespace ProjectMetadata
 * @brief Contains metadata structures for project and library dependencies.
 *
 * The ProjectMetadata namespace defines data structures that capture information about the
 * overall project and its libraries, including each library’s relative path, name, whether it is
 * a top-level library, its dependency list, and the set of subdirectories that may need to be
 * included during CMake file generation.
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
     * its dependencies, and the nested folder structure needed for CMake generation.
     */
    struct LibraryMetadata
    {
        std::string relativePath;                ///< The relative path where the library is located.
        std::string name;                        ///< The name of the library.
        bool isProjLevel;                        ///< True if the library is at the project (top) level.
        std::vector<std::string> dependencies;   ///< A list of dependencies for the library.
        std::vector<std::string> subDirectories; ///< A list of nested folders for CMake file generation.

        /**
         * @brief Default constructor for LibraryMetadata.
         *
         * This constructor initializes the library metadata with default values.
         * It sets the relative path and name to empty strings, the isProjLevel flag to false,
         * and leaves both the dependencies and subDirectories vectors empty.
         */
        LibraryMetadata()
            : relativePath(""),
              name(""),
              isProjLevel(false),
              dependencies(),
              subDirectories()
        {
        }

        /**
         * @brief Constructs a new LibraryMetadata object.
         *
         * The subDirectories vector is automatically initialized with the provided relativePath.
         *
         * @param relativePath The relative directory path for the library.
         * @param name The name of the library.
         * @param isProjLevel True if this library is at the project level.
         * @param dependencies A vector of dependencies for the library.
         */
        LibraryMetadata(const std::string relativePath, const std::string name, const bool isProjLevel, const std::vector<std::string> dependencies)
            : relativePath(std::move(relativePath)),
              name(std::move(name)),
              isProjLevel(isProjLevel),
              dependencies(std::move(dependencies)),
              subDirectories({this->relativePath})
        {
        }
    };

    /**
     * @brief Represents overall project metadata.
     *
     * This structure aggregates project-wide information, mapping library names to their metadata.
     */
    struct ProjMetadata
    {
        std::unordered_map<std::string, LibraryMetadata> libraries; ///< Metadata for each library in the project.
    };

} // namespace ProjectMetadata
