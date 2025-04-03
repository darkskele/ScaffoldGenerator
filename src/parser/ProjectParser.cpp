#include "ProjectParser.h"
#include "LibraryParser.h"
#include "FolderParser.h"
#include "ClassParser.h"
#include "CallableParser.h"
#include "NamespaceParser.h"
#include "ParserUtilities.h"
#include "CallableModels.h"
#include "ClassModels.h"

#include <sstream>
#include <stdexcept>
#include <deque>
#include <optional>
#include <vector>
#include <string>

namespace ProjectParser
{
    CodeGroupModels::ProjectModel parseProjectBlock(const std::string &projectName, std::deque<std::string_view> &lines)
    {
        // Project-specific properties.
        std::string version;
        std::vector<std::string> dependencies;

        // Process property lines (starting with '|') for version and dependency.
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
                throw std::runtime_error("Invalid property in project block: " + std::string(line));

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
                throw std::runtime_error("Unknown property in project block: " + key);
            }
        }

        // The remainder of the project block contains nested DSL elements.
        std::vector<CodeGroupModels::FolderModel> subFolders;
        std::vector<ClassModels::ClassModel> classFiles;
        std::vector<CodeGroupModels::NamespaceModel> namespaceFiles;
        std::vector<CallableModels::FunctionModel> functionFiles;
        std::vector<CodeGroupModels::LibraryModel> libraries;

        bool validContentFound = false;

        // Process nested blocks.
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

                // Split header into keyword and optional identifier.
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
                        throw std::runtime_error("Folder block must have an identifier in project block.");
                    auto nestedFolder = FolderParser::parseFolderBlock(identifier, lines);
                    subFolders.push_back(nestedFolder);
                }
                else if (keyword == "class")
                {
                    if (identifier.empty())
                        throw std::runtime_error("Class block must have an identifier in project block.");
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
                        throw std::runtime_error("Function block must have an identifier in project block.");
                    auto fn = CallableParser::parseFunctionProperties(identifier, lines);
                    functionFiles.push_back(fn);
                }
                else if (keyword == "library")
                {
                    if (identifier.empty())
                        throw std::runtime_error("Library block must have an identifier in project block.");
                    auto lib = LibraryParser::parseLibraryBlock(identifier, lines);
                    libraries.push_back(lib);
                }
                else if (keyword == "method")
                {
                    // Methods are not allowed at the project level.
                    throw std::runtime_error("Methods cannot be declared directly in a project block: " + identifier);
                }
                else
                {
                    throw std::runtime_error("Unknown block keyword in project block: " + keyword);
                }
                validContentFound = true;
            }
            else if (line.starts_with("|"))
            {
                throw std::runtime_error("Properties are only allowed at the beginning of a project block: " + std::string(line));
            }
            else
            {
                if (!validContentFound)
                    throw std::runtime_error("Malformed DSL file in project block: unexpected line '" + std::string(line) + "'");
            }
        }

        if (!validContentFound)
            throw std::runtime_error("Malformed DSL file: no valid content found in project block");

        // Construct and return the ProjectModel.
        return CodeGroupModels::ProjectModel(projectName, version, dependencies, libraries,
                                             subFolders, classFiles, namespaceFiles, functionFiles);
    }

} // namespace ProjectParser
