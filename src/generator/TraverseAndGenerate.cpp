#include "TraverseAndGenerate.h"

namespace FileGeneration
{

    void traverseAndGenerate(const std::shared_ptr<DirectoryTree::DirectoryNode> &node,
                             IFileWriter &writer)
    {
        // Process each file node in the current directory.
        for (const auto &fileNode : node->getFileNodes())
        {
            // Generate file contents (header and source) for the current file node.
            auto generated = fileNode->generateFiles();

            // Write the header file.
            // The generated.baseFilePath starts with "ROOT/", but the writer cleans this.
            writer.writeHeaderFile(generated.baseFilePath, generated.headerContent);

            // Write the source file.
            writer.writeSourceFile(generated.baseFilePath, generated.sourceContent);
        }

        // Recursively traverse each subdirectory in a depth-first manner.
        for (const auto &subDir : node->getSubDirectories())
        {
            traverseAndGenerate(subDir, writer);
        }
    }

} // namespace FileGeneration
