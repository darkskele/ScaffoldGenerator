#include "DiskFileWriter.h"
#include "GeneratorUtilities.h"

#include <filesystem>
#include <fstream>
#include <iostream>

/**
 * @brief Internal utility functions for file path manipulation and directory creation.
 *
 * This anonymous namespace contains static helper functions used internally by the DiskFileWriter
 * implementation. These functions are not visible outside this translation unit. They include:
 * - @ref constructFullPath: Constructs a full file path under the generatedOutputs directory.
 * - @ref ensureDirectoryExists: Ensures that the directory for a given file path exists, creating it if necessary.
 */
namespace
{
    /**
     * @brief Constructs the full file path for a generated file.
     *
     * This helper function removes the "ROOT/" prefix from the given file path,
     * appends the specified file extension, and constructs a complete file path under
     * the <outputFolder>/<subfolder>/ directory.
     *
     * @param outputFolder The output folder.
     * @param subfolder The subdirectory under outputFolder (e.g., "include" or "src").
     * @param filePath The base file path, possibly starting with "ROOT/".
     * @param extension The file extension to append (e.g., ".h" or ".cpp").
     * @return The full file path as a std::filesystem::path.
     */
    static std::filesystem::path constructFullPath(const std::string &outputFolder, const std::string &subfolder,
                                                   const std::string &filePath, const std::string &extension)
    {
        // Remove the ROOT prefix.
        std::string cleanedPath = GeneratorUtilities::removeRootPrefix(filePath);
        // Append the file extension.
        std::filesystem::path relativePath = cleanedPath + extension;
        // Build the full path: current_path()/<outputFolder>/subfolder/relativePath.
        std::filesystem::path fullPath = std::filesystem::current_path() / outputFolder / subfolder / relativePath;
        return fullPath;
    }

    /**
     * @brief Ensures that the directory for the given file path exists.
     *
     * This helper function verifies that the parent directory of the provided file path exists.
     * If the directory does not exist, it attempts to create all necessary directories.
     *
     * @param fullPath The complete file path for which the parent directory should exist.
     */
    static void ensureDirectoryExists(const std::filesystem::path &fullPath)
    {
        // Get the parent directory of the file.
        std::filesystem::path dir = fullPath.parent_path();
        if (!std::filesystem::exists(dir))
        {
            std::error_code ec;
            if (!std::filesystem::create_directories(dir, ec))
            {
                std::cerr << "Error creating directories: " << ec.message() << std::endl;
            }
        }
    }

    /**
     * @brief Writes a basic Doxygen file comment shell to the given file.
     *
     * This function writes a minimal Doxygen comment block for a file, including the
     * file name and an empty @brief tag for the user to fill in. If the file stream is
     * not valid, it prints an error message to std::cerr.
     *
     * @param file The output file stream where the Doxygen comment will be written.
     * @param fileName The path used to extract the file name for the Doxygen comment.
     */
    static void writeFileDoxygen(std::ofstream &file, const std::filesystem::path &fileName)
    {
        if (!file)
        {
            std::cerr << "Error opening file for writing: " << fileName << std::endl;
            return;
        }

        // Write basic file doxygen shell for user to fill in
        file << "/**\n * @file " << fileName.c_str() << "\n * @brief \n */\n\n";
    }

} // end anonymous namespace

namespace GeneratedFileWriter
{

    void DiskFileWriter::writeHeaderFile(const std::string &filePath, const std::string &content)
    {
        // Construct full path for the header file under <outputFolder>/include/.
        std::filesystem::path fullPath = constructFullPath(this->outputFolder, "include", filePath, ".h");
        // Ensure the target directory exists.
        ensureDirectoryExists(fullPath);

        // Open the file for writing.
        std::ofstream file(fullPath);
        if (!file)
        {
            std::cerr << "Error opening file for writing: " << fullPath << std::endl;
            return;
        }

        // Write file doxygen
        writeFileDoxygen(file, fullPath.filename());

        // Write header file includes (TODO : Expand these in future features)
        file << "#pragma once\n\n#include <string>\n#include <stdexcept>\n\n";

        // Write the content to the file.
        file << content;

        // Close file
        file.close();
    }

    void DiskFileWriter::writeSourceFile(const std::string &filePath, const std::string &content)
    {
        // Construct full path for the source file under <outputFolder>/src/.
        std::filesystem::path fullPath = constructFullPath(this->outputFolder, "src", filePath, ".cpp");
        // Ensure the target directory exists.
        ensureDirectoryExists(fullPath);

        // Open the file for writing.
        std::ofstream file(fullPath);
        if (!file)
        {
            std::cerr << "Error opening file for writing: " << fullPath << std::endl;
            return;
        }
        // Write header file includes (TODO : Expand these in future features)
        std::filesystem::path headerPath = fullPath;
        headerPath.replace_extension(".h");
        file << "#include \"" << headerPath.filename().string() << "\"\n\n";
        // Write the content to the file.
        file << content;

        // Close file
        file.close();
    }

    void DiskFileWriter::writeCmakeLists(const std::string &cmakeListsTxt) const
    {
        // Construct full path for the CmakeLists.txt at root
        std::filesystem::path fullPath = std::filesystem::current_path() / this->outputFolder / "CMakeLists.txt";
        // Ensure the target directory exists.
        ensureDirectoryExists(fullPath);

        // Open the file for writing.
        std::ofstream file(fullPath);
        if (!file)
        {
            std::cerr << "Error opening file for writing: " << fullPath << std::endl;
            return;
        }

        // Write the content to the file.
        file << cmakeListsTxt;

        // Close file
        file.close();
    }

    void DiskFileWriter::writeMain() const
    {
        // Construct file path to src/main.cpp
        std::filesystem::path fullPath = std::filesystem::current_path() / this->outputFolder / "src" / "main.cpp";

        // Ensure the target directory exists.
        ensureDirectoryExists(fullPath);

        // Open the file for writing.
        std::ofstream file(fullPath);
        if (!file)
        {
            std::cerr << "Error opening file for writing: " << fullPath << std::endl;
            return;
        }

        // Barebones main.cpp
        // Write the Doxygen file header
        file << "/**\n";
        file << " * @file main.cpp\n";
        file << " * @brief Main point of entry for the scaffolded project.\n";
        file << " */\n\n";
        file << "#include <iostream>\n\n";
        file << "/**\n";
        file << " * @brief Main.\n";
        file << " * @param argc Number of command line arguments.\n";
        file << " * @param argv Array of command line argument strings.\n";
        file << " * @return int Returns 0 on success, or 1 on error.\n";
        file << " */\n";

        // Write the main.cpp and print a hello world message
        file << "int main(int argc, char *argv[])\n";
        file << "{\n";
        file << "    std::cout << \"Hello, world!\" << std::endl;\n";
        file << "    return 0;\n";
        file << "}\n";

        // Close file
        file.close();
    }

    void DiskFileWriter::writeVsCodeJsons(const std::pair<std::string, std::string> &jsonsFiles) const
    {
        // Construct file path to .vscode/launch.json
        std::filesystem::path launchPath = std::filesystem::current_path() / this->outputFolder / ".vscode" / "launch.json";

        // Ensure the target directory exists.
        ensureDirectoryExists(launchPath);

        // Open the file for writing.
        std::ofstream launchFile(launchPath);
        if (!launchFile)
        {
            std::cerr << "Error opening file for writing: " << launchPath << std::endl;
            return;
        }

        // Write launch file
        launchFile << jsonsFiles.first;

        // Construct file path to .vscode/tasks.json
        std::filesystem::path tasksPath = std::filesystem::current_path() / this->outputFolder / ".vscode" / "tasks.json";

        // Ensure the target directory exists.
        ensureDirectoryExists(tasksPath);

        // Open the file for writing.
        std::ofstream tasksFile(tasksPath);
        if (!tasksFile)
        {
            std::cerr << "Error opening file for writing: " << tasksPath << std::endl;
            return;
        }

        // Write launch file
        tasksFile << jsonsFiles.second;
    }

} // namespace GeneratedFileWriter
