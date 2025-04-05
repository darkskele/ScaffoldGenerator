#include "BuildToolsGenerator.h"
#include "GeneratorUtilities.h"

#include <fstream>
#include <iostream>
#include <format>
#include <sstream>
#include <vector>
#include <filesystem>

/**
 * @namespace
 * @brief Internal anonymous namespace for CMake generation helper functions.
 *
 * This anonymous namespace contains internal helper functions used for generating
 * portions of a CMakeLists.txt file based on the project's metadata. These functions
 * are not exposed outside of this translation unit.
 */
namespace
{
    namespace fs = std::filesystem;

    /**
     * @brief Generates CMake commands for finding and linking dependencies.
     *
     * This function iterates over the dependency strings stored in the library metadata. It assumes that
     * each dependency is specified in the "<first>::<second>" format (for example, "Boost::boost"). The function
     * extracts the package name (the part before "::") to use with the find_package() command and then uses
     * the full dependency string to link the target using target_link_libraries().
     *
     * @param lib The LibraryMetadata object containing dependency information.
     * @param binName The name of the binary (executable or library target) to link the dependencies to.
     * @return A string containing the generated CMake commands for dependency linking.
     */
    std::string generateDependencies(const ProjectMetadata::LibraryMetadata &lib, const std::string &binName)
    {
        std::ostringstream dependencies;

        for (const auto &dep : lib.dependencies)
        {
            // Extract the package name from the dependency string, assuming the format "<first>::<second>".
            std::string packageName = dep.substr(0, dep.find("::"));

            dependencies << "\n# Find and link " << dep << " library\n";
            dependencies << "find_package(" << packageName << " REQUIRED)\n";
            dependencies << "if(" << packageName << "_FOUND)\n";
            dependencies << "target_link_libraries(" << binName << " PUBLIC " << dep << ")\n";
            dependencies << "endif()\n";
        }

        return dependencies.str();
    }

    /**
     * @brief Generates CMake snippet for library targets based on project metadata.
     *
     * This function iterates over the project's library metadata and creates CMake
     * commands to define each library target. It skips the main binary (project-level)
     * target and assumes that non-project-level library source files are located in
     * "src/<relativePath>/..." and that the include directories are defined by the
     * library's subDirectories vector.
     *
     * @param projMeta The project metadata containing information about all libraries.
     * @return A string containing the CMake commands for defining library targets.
     */
    std::string generateLibraryTargets(const ProjectMetadata::ProjMetadata &projMeta)
    {
        std::string cmakeSnippet;
        for (const auto &[_, lib] : projMeta.libraries)
        {
            // Skip the main binary target (project-level library) for now.
            if (lib.isProjLevel)
            {
                continue;
            }
            // Prune ROOT from relative path for globbing purposes.
            std::string relPath = GeneratorUtilities::removeRootPrefix(lib.relativePath);
            // Glob all .cpp files in the library folder (including all subdirectories).
            std::string globCommand = std::format("file(GLOB_RECURSE {0}_SOURCES CONFIGURE_DEPENDS \"${{CMAKE_SOURCE_DIR}}/src/{1}/*.cpp\")\n",
                                                  lib.name, relPath);
            globCommand += std::format("add_library({} ${{{}_SOURCES}})\n", lib.name, lib.name);
            cmakeSnippet += globCommand;

            // Instead of using a single include directory, add all subdirectories stored in metadata.
            for (const auto &subDir : lib.subDirectories)
            {
                std::string subRelPath = GeneratorUtilities::removeRootPrefix(subDir);
                cmakeSnippet += std::format("target_include_directories({} PUBLIC ${{CMAKE_SOURCE_DIR}}/include/{}/)\n",
                                            lib.name, subRelPath);
            }

            // Generate dependency linking commands using the dependency generator.
            cmakeSnippet += generateDependencies(lib, lib.name) + "\n";
        }
        return cmakeSnippet;
    }

    /**
     * @brief Generates CMake snippet for the main binary target.
     *
     * This function locates the project-level target from the metadata, collects the
     * directories of non-project-level libraries to exclude, and generates CMake commands
     * to glob all source files from the src directory (excluding library subdirectories)
     * to build the main executable.
     *
     * @param projMeta The project metadata containing library information.
     * @return A string containing the CMake commands for defining the main executable target.
     */
    std::string generateMainBinaryTarget(const ProjectMetadata::ProjMetadata &projMeta)
    {
        // Find the project-level metadata entry.
        const ProjectMetadata::LibraryMetadata *mainBinary = nullptr;
        // Also collect library directories to exclude from the main binary.
        std::vector<std::string> libraryDirs;

        for (const auto &[_, lib] : projMeta.libraries)
        {
            if (lib.isProjLevel)
            {
                mainBinary = &lib;
            }
            else
            {
                std::string relPath = GeneratorUtilities::removeRootPrefix(lib.relativePath);
                libraryDirs.push_back(relPath);
            }
        }

        if (!mainBinary)
        {
            std::cerr << "Error: No project-level (main binary) target defined in metadata.\n";
            return "";
        }

        // Generate the CMake code that uses file globbing and filtering.
        std::string snippet;
        snippet += std::format("set(MAIN_TARGET {})\n", mainBinary->name);

        // Convert the library directories into a CMake list format.
        snippet += "set(LIBRARY_DIRS";
        for (const auto &dir : libraryDirs)
        {
            snippet += " " + dir;
        }
        snippet += ")";

        snippet += std::string(R"(
# Glob all .cpp files in src with CONFIGURE_DEPENDS for automatic reconfiguration.
file(GLOB_RECURSE ALL_SRCS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Exclude sources from library subdirectories.
foreach(lib_dir IN LISTS LIBRARY_DIRS)
    list(FILTER ALL_SRCS EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/src/${lib_dir}/.*")
endforeach()

# Create the main executable target.
add_executable(${MAIN_TARGET} ${ALL_SRCS})
target_include_directories(${MAIN_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/include)
)");

        // Link main target to its own dependencies (if any).
        snippet += generateDependencies(*mainBinary, "${MAIN_TARGET}");

        // Now, link all non-project-level libraries to the main binary.
        for (const auto &[_, lib] : projMeta.libraries)
        {
            if (!lib.isProjLevel)
            {
                snippet += "target_link_libraries(${MAIN_TARGET} PUBLIC " + lib.name + ")\n";
            }
        }

        return snippet;
    }

}

namespace BuildToolGenerator
{
    std::string generateCmakeLists(const ProjectMetadata::ProjMetadata &projMetaData)
    {
        std::ostringstream cmakeFile;

        // Write basic project settings
        cmakeFile << "cmake_minimum_required(VERSION 3.16)\n";
        cmakeFile << "project(MyProject LANGUAGES CXX)\n\n";
        cmakeFile << "set(CMAKE_CXX_STANDARD 20)\n";
        cmakeFile << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n";
        cmakeFile << "# Global include directory\n";
        cmakeFile << "include_directories(${CMAKE_SOURCE_DIR}/include)\n\n";

        // Generate library targets based on metadata (non-project-level libraries)
        cmakeFile << "# Library Targets\n";
        cmakeFile << generateLibraryTargets(projMetaData);

        // Generate main binary target which excludes library directories in src.
        cmakeFile << "# Main Binary Target\n";
        cmakeFile << generateMainBinaryTarget(projMetaData) << "\n";

        return cmakeFile.str();
    }

    std::pair<std::string, std::string> generateVscodeJSONs(const std::string &projectName)
    {
        // Build the launch.json configuration using an ostringstream.
        std::ostringstream launchOss;
        launchOss << "{\n"
                  << "    \"version\": \"0.2.0\",\n"
                  << "    \"configurations\": [\n"
                  << "        {\n"
                  << "            \"name\": \"Debug " << projectName << "\",\n"
                  << "            \"type\": \"cppdbg\",\n"
                  << "            \"request\": \"launch\",\n"
                  << "            \"program\": \"${workspaceFolder}/build-" << projectName << "/" << projectName << "\",\n"
                  << "            \"args\": [],\n"
                  << "            \"stopAtEntry\": false,\n"
                  << "            \"cwd\": \"${workspaceFolder}/build-" << projectName << "\",\n"
                  << "            \"environment\": [],\n"
                  << "            \"externalConsole\": false,\n"
                  << "            \"MIMode\": \"gdb\",\n"
                  << "            \"preLaunchTask\": \"Build and Run " << projectName << "\"\n"
                  << "        }\n"
                  << "    ]\n"
                  << "}";

        // Build the tasks.json configuration using another ostringstream.
        std::ostringstream tasksOss;
        tasksOss << "{\n";
        tasksOss << "    \"version\": \"2.0.0\",\n";
        tasksOss << "    \"tasks\": [\n";
        tasksOss << "        {\n";
        tasksOss << "            \"label\": \"Build and Run " << projectName << "\",\n";
        tasksOss << "            \"type\": \"shell\",\n";
        tasksOss << "            \"command\": \"/bin/bash\",\n";
        tasksOss << "            \"args\": [\n";
        tasksOss << "                \"-c\",\n";
        tasksOss << "                \"mkdir -p build-" << projectName << " && cd build-" << projectName << " && cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build . --target " << projectName << " -- -j$(nproc)\"\n";
        tasksOss << "            ],\n";
        tasksOss << "            \"group\": {\n";
        tasksOss << "                \"kind\": \"build\",\n";
        tasksOss << "                \"isDefault\": true\n";
        tasksOss << "            },\n";
        tasksOss << "            \"presentation\": {\n";
        tasksOss << "                \"reveal\": \"always\",\n";
        tasksOss << "                \"panel\": \"shared\"\n";
        tasksOss << "            },\n";
        tasksOss << "            \"problemMatcher\": [\n";
        tasksOss << "                \"$gcc\"\n";
        tasksOss << "            ]\n";
        tasksOss << "        }\n";
        tasksOss << "    ]\n";
        tasksOss << "}";

        // Return a pair where the first element is launch.json and the second is tasks.json.
        return {launchOss.str(), tasksOss.str()};
    }

} // namespace BuildToolGenerator