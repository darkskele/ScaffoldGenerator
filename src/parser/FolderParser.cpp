#include "FolderParser.h"
#include "ParserUtilities.h"
#include "ClassParser.h"
#include "CallableParser.h"
#include "NamespaceParser.h"
#include "CodeGroupModels.h"
#include "ClassModels.h"
#include "CallableModels.h"

#include <stdexcept>
#include <deque>
#include <optional>

namespace FolderParser
{
    // Parses a folder block from the DSL.
    // 'folderName' is the name given to the current folder block.
    // 'lines' contains the DSL lines to be processed.
    CodeGroupModels::FolderModel parseFolderBlock(const std::string &folderName, std::deque<std::string_view> &lines)
    {
        // Vectors to store the parsed elements from the folder block.
        std::vector<CodeGroupModels::FolderModel> subFolders;        // Nested folder blocks.
        std::vector<ClassModels::ClassModel> classFiles;             // Class blocks (each generating its own file).
        std::vector<CodeGroupModels::NamespaceModel> namespaceFiles; // Namespace blocks (each generating its own file).
        std::vector<CallableModels::FunctionModel> functionFile;    // Free function blocks, all grouped into one file.

        // Flag to indicate that valid DSL content has been encountered.
        bool validContentFound = false;

        // Process lines until we reach the end marker '_' or run out of lines.
        while (!lines.empty())
        {
            // Retrieve the next line and trim whitespace.
            std::string_view line = ParserUtilities::trim(lines.front());
            lines.pop_front(); // Consume the line.

            // Skip empty lines.
            if (line.empty())
                continue;

            // Check if the current line is the end-of-block marker.
            if (line == "_")
            {
                validContentFound = true;
                break;
            }
            // Process header lines that start with '-'.
            else if (!line.empty() && line.front() == '-')
            {
                // Remove the initial '-' and trim surrounding spaces.
                line.remove_prefix(1);
                line = ParserUtilities::trim(line);

                // Remove trailing colon if present.
                if (!line.empty() && line.back() == ':')
                {
                    line.remove_suffix(1);
                    line = ParserUtilities::trim(line);
                }

                // Split the header into keyword and optional identifier.
                size_t spacePos = line.find(' ');
                std::string keyword, identifier;
                if (spacePos == std::string_view::npos)
                {
                    // Entire line is the keyword if no space is found.
                    keyword = std::string(line);
                }
                else
                {
                    // Extract keyword and identifier from the header.
                    keyword = std::string(ParserUtilities::trim(line.substr(0, spacePos)));
                    identifier = std::string(ParserUtilities::trim(line.substr(spacePos + 1)));
                }

                // Process the header based on the extracted keyword.
                if (keyword == "folder")
                {
                    // A folder block must have an identifier.
                    if (identifier.empty())
                        throw std::runtime_error("Folder block must have an identifier.");

                    // Recursively parse the nested folder block.
                    auto nestedFolder = parseFolderBlock(identifier, lines);
                    subFolders.push_back(nestedFolder);
                }
                else if (keyword == "class")
                {
                    // A class block must have an identifier.
                    if (identifier.empty())
                        throw std::runtime_error("Class block must have an identifier.");

                    // Parse the class block using the ClassParser.
                    auto cls = ClassParser::parseClassBlock(identifier, lines);
                    classFiles.push_back(cls);
                }
                else if (keyword == "namespace")
                {
                    // Parse the namespace block using the NamespaceParser.
                    auto ns = NamespaceParser::parseNamespaceBlock(
                        identifier.empty() ? std::nullopt : std::make_optional(identifier), lines);
                    namespaceFiles.push_back(ns);
                }
                else if (keyword == "function")
                {
                    // A function block must have an identifier.
                    if (identifier.empty())
                        throw std::runtime_error("Function block must have an identifier.");

                    // Parse the function block using the CallableParser.
                    auto fn = CallableParser::parseFunctionProperties(identifier, lines);
                    // Add the function to the functionFile vector.
                    functionFile.push_back(fn);
                }
                else if (keyword == "method")
                {
                    // Methods cannot be declared at the folder level.
                    throw std::runtime_error("Methods cannot be declared directly in a folder block: " + identifier);
                }
                else
                {
                    // Unknown keyword encountered.
                    throw std::runtime_error("Unknown block keyword in folder: " + keyword);
                }
                // Mark that valid content has been processed.
                validContentFound = true;
            }
            // If a property line (starting with '|') is encountered, throw an error.
            else if (line.starts_with("|"))
            {
                throw std::runtime_error("Properties are not supported in folder blocks: " + std::string(line));
            }
            else
            {
                // If no valid content has been processed and an unexpected line is found, throw an error.
                if (!validContentFound)
                    throw std::runtime_error("Malformed DSL file in folder block: unexpected line '" + std::string(line) + "'");
                // Otherwise, ignore any extraneous trailing content.
            }
        }

        // If no valid DSL content was found, throw an error.
        if (!validContentFound)
            throw std::runtime_error("Malformed DSL file: no valid content found in folder block");

        // Construct and return the FolderModel with the parsed subfolders, class files, namespace files, and free function blocks.
        return CodeGroupModels::FolderModel(folderName, subFolders, classFiles, namespaceFiles, functionFile);
    }

} // namespace FolderParser
