/**
 * @file main.cpp
 * @brief Entry point for the Project Scaffolder CLI tool.
 *
 * This file implements the main function for the scaffolder CLI tool.
 * It reads a .scaff file (or searches a directory for one), splits the content into lines,
 * parses the project block from the DSL, builds the corresponding directory tree, and
 * traverses the tree to generate files on disk.
 *
 * The tool accepts a required input path (file or directory) and an optional --output-folder argument.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <stdexcept>
#include <filesystem>
#include <algorithm>

// Include DSL parsing and generation headers.
#include "ProjectParser.h"        // Parses project blocks from the DSL.
#include "DirectoryTreeBuilder.h" // Builds a directory tree from DSL models.
#include "TraverseAndGenerate.h"  // Traverses the directory tree and generates files.
#include "DiskFileWriter.h"       // Writes generated files to disk.
#include "ProjectMetadata.h"      // Holds metadata for the project.
#include "ParserUtilities.h"      // Provides trim and other utilities for project block.
#include "BuildToolsGenerator.h"  // Provides generators for CMakeLists, Tasks.json, Launch.json

namespace fs = std::filesystem; ///< Filesystem namespace alias for brevity

/**
 * @brief Reads the entire content of a file into a string.
 *
 * @param filePath The path to the file to be read.
 * @return A string containing the full content of the file.
 * @throws std::runtime_error if the file cannot be opened.
 */
std::string readFile(const fs::path &filePath)
{
    std::ifstream inFile(filePath);
    if (!inFile)
    {
        // Throw an error if the file cannot be opened.
        throw std::runtime_error("Unable to open file: " + filePath.string());
    }
    std::stringstream buffer;
    buffer << inFile.rdbuf(); // Read entire file content into the buffer.
    return buffer.str();
}

/**
 * @brief Splits a file's content into lines stored as string_views.
 *
 * This function processes the given file content by splitting it on newline characters.
 * Each line is stored as a std::string_view, and all lines are added to a std::deque.
 * Note: The backing string must remain in scope while the views are used.
 *
 * @param fileContent The full content of the file as a string.
 * @return A deque containing a string_view for each line in the file.
 */
std::deque<std::string_view> splitIntoLines(const std::string &fileContent)
{
    std::deque<std::string_view> lines;
    size_t start = 0;
    // Process each line until the end of the file content.
    while (start < fileContent.size())
    {
        size_t end = fileContent.find('\n', start);
        if (end == std::string::npos)
        {
            // If no newline is found, set end to the end of the file.
            end = fileContent.size();
        }
        // Create a string_view for the current line.
        std::string_view line(fileContent.data() + start, end - start);
        // Remove carriage return at end of line if present (Windows compatibility).
        if (!line.empty() && line.back() == '\r')
        {
            line.remove_suffix(1);
        }
        lines.push_back(line); // Add the line to the deque.
        start = end + 1;       // Move start to the beginning of the next line.
    }
    return lines;
}

/**
 * @brief Main function for the scaffolder CLI tool.
 *
 * This function processes command line arguments, reads and splits a .scaff file,
 * parses the project block, builds a directory tree, and generates output files to disk.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 * @return int Returns 0 on success, or 1 on error.
 */
int main(int argc, char *argv[])
{
    try
    {
        // Check for minimum required arguments.
        if (argc < 2)
        {
            std::cerr << "Usage: scaffolder <input_path> [--output-folder <output_path>]" << std::endl;
            return 1;
        }

        // Set input and default output paths.
        fs::path inputPath = argv[1];
        fs::path outputFolder = "generatedOutputs"; // Default output folder

        // Process optional command line argument for output folder.
        for (int i = 2; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "--output-folder" && i + 1 < argc)
            {
                outputFolder = argv[++i];
            }
        }

        // Verify the input path exists.
        if (!fs::exists(inputPath))
        {
            throw std::runtime_error("Input path does not exist: " + inputPath.string());
        }

        fs::path scaffFile; // Variable to hold the path to the .scaff file.

        // Check if the input path is a file or directory.
        if (fs::is_regular_file(inputPath))
        {
            // Use the file directly if input is a file.
            scaffFile = inputPath;
        }
        else if (fs::is_directory(inputPath))
        {
            // If input is a directory, search for the first .scaff file alphabetically.
            std::vector<fs::path> scaffFiles;
            for (const auto &entry : fs::directory_iterator(inputPath))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".scaff")
                {
                    scaffFiles.push_back(entry.path());
                }
            }
            // Throw an error if no .scaff file is found.
            if (scaffFiles.empty())
            {
                throw std::runtime_error("No .scaff file found in directory: " + inputPath.string());
            }
            std::sort(scaffFiles.begin(), scaffFiles.end());
            scaffFile = scaffFiles.front();
        }
        else
        {
            // Input is neither a file nor a directory.
            throw std::runtime_error("Input is neither a file nor a directory: " + inputPath.string());
        }

        // Read the content of the selected .scaff file.
        std::string fileContent = readFile(scaffFile);
        if (fileContent.empty())
        {
            throw std::runtime_error("The scaff file is empty: " + scaffFile.string());
        }

        // Split the file content into individual lines.
        std::deque<std::string_view> lines = splitIntoLines(fileContent);

        // Clean first line of scaff file
        auto firstLine = ParserUtilities::trim(lines.front());
        if (firstLine.front() == '-')
        {
            firstLine.remove_prefix(1);
            firstLine = ParserUtilities::trim(firstLine);
            // Validate that the file starts with a project block.
            if (lines.empty() || firstLine.find("project") == std::string::npos)
            {
                throw std::runtime_error("The scaff file must start with a project block.");
            }
        }
        else
        {
            throw std::runtime_error("The scaff file must start with a project block.");
        }

        // Extract the project name from the first line.
        // Expected format: "project <projectName>"
        std::istringstream headerStream{ std::string(firstLine) };
        std::string keyword, projectName;
        headerStream >> keyword >> projectName;
        if (keyword != "project" || projectName.empty())
        {
            throw std::runtime_error("Malformed project block header. Expected: project <projectName>");
        }

        // Remove colon from now if it exists
        if (projectName.back() == ':')
        {
            projectName.pop_back();
        }

        // Remove the project header line from the deque.
        lines.pop_front();

        // Parse the project block to build the DSL model.
        CodeGroupModels::ProjectModel projModel = ProjectParser::parseProjectBlock(projectName, lines);
        std::cout << "Project block parsed successfully for project: " << projectName << std::endl;

        // Create a project metadata object for directory tree construction.
        ProjectMetadata::ProjMetadata projectMeta{{}};

        // Build the directory tree from the project model.
        std::shared_ptr<DirectoryTree::DirectoryNode> rootNode =
            DirectoryTreeBuilder::buildDirectoryTree(projModel, &projectMeta);
        std::cout << "Directory tree built successfully." << std::endl;

        // Create an instance of DiskFileWriter for file generation.
        GeneratedFileWriter::DiskFileWriter diskWriter(outputFolder.string());

        // Traverse the directory tree and generate output files using the disk writer.
        FileGeneration::traverseAndGenerate(rootNode, diskWriter);
        std::cout << "File generation completed successfully." << std::endl;

        // Generate the CMake file
        std::string cmakeFile = BuildToolGenerator::generateCmakeLists(projectMeta);
        diskWriter.writeCmakeLists(cmakeFile);

        // Generate main file
        diskWriter.writeMain();

        // Generate vscode Jsons
        diskWriter.writeVsCodeJsons(BuildToolGenerator::generateVscodeJSONs(projModel.name));
    }
    catch (const std::exception &ex)
    {
        // Report any errors that occur during processing.
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
