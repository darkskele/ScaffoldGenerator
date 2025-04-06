#include "DiskFileWriter.h"
#include "GeneratorUtilities.h"

#include <filesystem>
#include <fstream>
#include <functional>
#include <stdexcept>

/**
 * @brief Internal utility functions for file path manipulation and directory creation.
 *
 * This anonymous namespace contains static helper functions used internally by the DiskFileWriter
 * implementation. These functions are not visible outside this translation unit. They include:
 * - @ref constructFullPath: Constructs a full file path under the generatedOutputs directory.
 * - @ref ensureDirectoryExists: Ensures that the directory for a given file path exists, creating it if necessary.
 * - @ref writeToFile: Centralizes the file opening, error checking, writing, and closing logic.
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
     * @throws std::runtime_error if it fails to create the necessary directories.
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
                throw std::runtime_error("Error creating directories: " + ec.message());
            }
        }
    }

    /**
     * @brief Centralizes file writing logic.
     *
     * This helper function ensures that the target directory exists, opens the file for writing,
     * and then calls the provided lambda to write content to the file. It throws a std::runtime_error
     * if the file cannot be opened.
     *
     * @param fullPath The full path to the file.
     * @param writer A lambda that takes an std::ofstream reference and writes content to the file.
     * @throws std::runtime_error if the file cannot be opened.
     */
    static void writeToFile(const std::filesystem::path &fullPath, const std::function<void(std::ofstream &)> &writer)
    {
        ensureDirectoryExists(fullPath);
        std::ofstream file(fullPath);
        if (!file)
        {
            throw std::runtime_error("Error opening file for writing: " + fullPath.string());
        }
        writer(file);
        file.close();
    }

    /**
     * @brief Writes a basic Doxygen file comment shell to the given file.
     *
     * This function writes a minimal Doxygen comment block for a file, including the
     * file name and an empty @brief tag for the user to fill in.
     *
     * @param file The output file stream where the Doxygen comment will be written.
     * @param fileName The path used to extract the file name for the Doxygen comment.
     * @throws std::runtime_error if the file stream is not valid.
     */
    static void writeFileDoxygen(std::ofstream &file, const std::filesystem::path &fileName)
    {
        // Since writeToFile already guarantees the file stream is valid, we can write directly.
        file << "/**\n * @file " << fileName.c_str() << "\n * @brief \n */\n\n";
    }

} // end anonymous namespace

namespace GeneratedFileWriter
{

    void DiskFileWriter::writeHeaderFile(const std::string &filePath, const std::string &content)
    {
        // Construct full path for the header file under <outputFolder>/include/.
        std::filesystem::path fullPath = constructFullPath(this->outputFolder, "include", filePath, ".h");

        writeToFile(fullPath, [&fullPath, &content](std::ofstream &file)
                    {
            // Write file doxygen.
            writeFileDoxygen(file, fullPath.filename());
            // Write header file includes (TODO: Expand these in future features).
            file << "#pragma once\n\n#include <string>\n#include <stdexcept>\n\n";
            // Write the content to the file.
            file << content; });
    }

    void DiskFileWriter::writeSourceFile(const std::string &filePath, const std::string &content)
    {
        // Construct full path for the source file under <outputFolder>/src/.
        std::filesystem::path fullPath = constructFullPath(this->outputFolder, "src", filePath, ".cpp");

        writeToFile(fullPath, [&fullPath, &content](std::ofstream &file)
                    {
            // Write header file includes (TODO: Expand these in future features).
            std::filesystem::path headerPath = fullPath;
            headerPath.replace_extension(".h");
            file << "#include \"" << headerPath.filename().string() << "\"\n\n";
            // Write the content to the file.
            file << content; });
    }

    void DiskFileWriter::writeCmakeLists(const std::string &cmakeListsTxt) const
    {
        // Construct full path for the CMakeLists.txt at root.
        std::filesystem::path fullPath = std::filesystem::current_path() / this->outputFolder / "CMakeLists.txt";

        writeToFile(fullPath, [&cmakeListsTxt](std::ofstream &file)
                    {
            // Write the content to the file.
            file << cmakeListsTxt; });
    }

    void DiskFileWriter::writeMain() const
    {
        // Construct file path to src/main.cpp.
        std::filesystem::path fullPath = std::filesystem::current_path() / this->outputFolder / "src" / "main.cpp";

        writeToFile(fullPath, [](std::ofstream &file)
                    {
            // Barebones main.cpp.
            // Write the Doxygen file header.
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
            // Write the main.cpp content and print a hello world message.
            file << "int main(int argc, char *argv[])\n";
            file << "{\n";
            file << "    std::cout << \"Hello, world!\" << std::endl;\n";
            file << "    return 0;\n";
            file << "}\n"; });
    }

    void DiskFileWriter::writeVsCodeJsons(const std::pair<std::string, std::string> &jsonsFiles) const
    {
        // Construct file path to .vscode/launch.json.
        std::filesystem::path launchPath = std::filesystem::current_path() / this->outputFolder / ".vscode" / "launch.json";

        writeToFile(launchPath, [&jsonsFiles](std::ofstream &launchFile)
                    {
            // Write launch file.
            launchFile << jsonsFiles.first; });

        // Construct file path to .vscode/tasks.json.
        std::filesystem::path tasksPath = std::filesystem::current_path() / this->outputFolder / ".vscode" / "tasks.json";

        writeToFile(tasksPath, [&jsonsFiles](std::ofstream &tasksFile)
                    {
            // Write tasks file.
            tasksFile << jsonsFiles.second; });
    }

} // namespace GeneratedFileWriter
