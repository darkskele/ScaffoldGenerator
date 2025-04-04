#include "LibraryParser.h"
#include "FolderParser.h"
#include "ParserUtilities.h"
#include "ClassParser.h"
#include "CallableParser.h"
#include "NamespaceParser.h"
#include "ClassModels.h"
#include "CallableModels.h"

#include <sstream>
#include <stdexcept>
#include <deque>
#include <optional>
#include <vector>
#include <string>

namespace LibraryParser
{
    CodeGroupModels::LibraryModel parseLibraryBlock(const std::string &libraryName, std::deque<std::string_view> &lines)
    {
        // Library-specific properties.
        std::string version;
        std::vector<std::string> dependencies;

        // Process property lines (starting with '|') that define version and dependency.
        while (!lines.empty())
        {
            std::string_view line = ParserUtilities::trim(lines.front());
            if (!line.starts_with("|"))
                break;         // End of property section.
            lines.pop_front(); // Consume the property line.
            // Remove the leading '|' and trim.
            line.remove_prefix(1);
            line = ParserUtilities::trim(line);

            // Expect key=value format.
            size_t eqPos = line.find('=');
            if (eqPos == std::string_view::npos)
                throw std::runtime_error("Invalid property in library block: " + std::string(line));

            std::string key = std::string(ParserUtilities::trim(line.substr(0, eqPos)));
            std::string value = std::string(ParserUtilities::trim(line.substr(eqPos + 1)));

            if (key == "version")
            {
                version = value;
            }
            else if (key == "dependency")
            {
                // Split the value on commas to extract dependencies.
                std::istringstream iss(value);
                std::string dep;
                while (std::getline(iss, dep, ','))
                {
                    dependencies.push_back(std::string(ParserUtilities::trim(dep)));
                }
            }
            else
            {
                throw std::runtime_error("Unknown property in library block: " + key);
            }
        }

        // The remainder of the library block contains nested DSL elements.
        // We reuse folder parsing logic here.
        std::vector<CodeGroupModels::FolderModel> subFolders;
        std::vector<ClassModels::ClassModel> classFiles;
        std::vector<CodeGroupModels::NamespaceModel> namespaceFiles;
        std::vector<CallableModels::FunctionModel> functionFile;

        bool validContentFound = false;

        // Process nested lines similar to FolderParser::parseFolderBlock.
        while (!lines.empty())
        {
            std::string_view line = ParserUtilities::trim(lines.front());
            lines.pop_front(); // Consume the line.

            if (line.empty())
                continue;

            if (line == "_")
            {
                validContentFound = true;
                break;
            }
            else if (!line.empty() && line.front() == '-')
            {
                // Remove the initial '-' and trim.
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
                    keyword = std::string(line);
                }
                else
                {
                    keyword = std::string(ParserUtilities::trim(line.substr(0, spacePos)));
                    identifier = std::string(ParserUtilities::trim(line.substr(spacePos + 1)));
                }

                // Process nested blocks.
                if (keyword == "folder")
                {
                    if (identifier.empty())
                        throw std::runtime_error("Folder block must have an identifier in library block.");

                    auto nestedFolder = FolderParser::parseFolderBlock(identifier, lines);
                    subFolders.push_back(nestedFolder);
                }
                else if (keyword == "class")
                {
                    if (identifier.empty())
                        throw std::runtime_error("Class block must have an identifier in library block.");

                    auto cls = ClassParser::parseClassBlock(identifier, lines);
                    classFiles.push_back(cls);
                }
                else if (keyword == "namespace")
                {
                    auto ns = NamespaceParser::parseNamespaceBlock(
                        identifier.empty() ? std::nullopt : std::make_optional(identifier), lines);
                    namespaceFiles.push_back(ns);
                }
                else if (keyword == "function")
                {
                    if (identifier.empty())
                        throw std::runtime_error("Function block must have an identifier in library block.");

                    auto fn = CallableParser::parseFunctionProperties(identifier, lines);
                    functionFile.push_back(fn);
                }
                else if (keyword == "library")
                {
                    // Nested libraries are not allowed.
                    throw std::runtime_error("Nested library blocks are not allowed in library: " + identifier);
                }
                else if (keyword == "method")
                {
                    // Methods cannot be declared directly in a library block.
                    throw std::runtime_error("Methods cannot be declared directly in a library block: " + identifier);
                }
                else
                {
                    throw std::runtime_error("Unknown block keyword in library: " + keyword);
                }
                validContentFound = true;
            }
            else if (line.starts_with("|"))
            {
                throw std::runtime_error("Properties are only allowed at the beginning of a library block: " + std::string(line));
            }
            else
            {
                if (!validContentFound)
                    throw std::runtime_error("Malformed DSL file in library block: unexpected line '" + std::string(line) + "'");
            }
        }

        if (!validContentFound)
            throw std::runtime_error("Malformed DSL file: no valid content found in library block");

        // Create a FolderModel for the nested content.
        CodeGroupModels::FolderModel folderModel(libraryName, subFolders, classFiles, namespaceFiles, functionFile);

        // Construct and return the LibraryModel using the parsed properties and nested content.
        return CodeGroupModels::LibraryModel(libraryName, version, dependencies,
                                             folderModel.subFolders,
                                             folderModel.classFiles,
                                             folderModel.namespaceFiles,
                                             folderModel.functionFile);
    }

} // namespace LibraryParser
