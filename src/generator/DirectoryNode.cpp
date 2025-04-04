// This file provides the definitions for the DirectoryNode class declared in DirectoryNode.h.
// Detailed inline comments explain the functionality of each method.

#include "DirectoryNode.h"

namespace DirectoryTree
{

    //--------------------------------------------------------------------------
    // Constructor
    //--------------------------------------------------------------------------

    DirectoryNode::DirectoryNode(const std::string &folderName,
                                 const std::string &parentPath,
                                 std::shared_ptr<DirectoryNode> parent)
        : folderName(folderName)
    {
        // Only parents have no parent path. Otherwise throw error.
        if (parentPath.empty() && parent != nullptr)
        {
            throw std::invalid_argument("Non-root node must have a parentPath.");
        }

        // Compute the relative path:
        // If parentPath is empty, this node is the root; otherwise, combine parent's path and folder name.
        if (parentPath.empty())
        {
            relativePath = std::move(folderName);
        }
        else
        {
            relativePath = parentPath + "/" + folderName;
        }
        // Store the parent as a weak pointer.
        this->parent = std::move(parent);
    }

    //--------------------------------------------------------------------------
    // addSubDirectory Method
    //--------------------------------------------------------------------------

    void DirectoryNode::addSubDirectory(const std::shared_ptr<DirectoryNode> &dirNode)
    {
        // Add the new child node to the subDirectories vector.
        subDirectories.push_back(dirNode);
    }

    //--------------------------------------------------------------------------
    // addFileNode Method
    //--------------------------------------------------------------------------

    void DirectoryNode::addFileNode(std::unique_ptr<FileNodeGenerator::IGeneratedFile> fileNode)
    {
        // Move the file node into the fileNodes vector.
        fileNodes.push_back(std::move(fileNode));
    }

    //--------------------------------------------------------------------------
    // getParent Method
    //--------------------------------------------------------------------------

    std::shared_ptr<DirectoryNode> DirectoryNode::getParent() const
    {
        // Convert the weak pointer to a shared_ptr. This may return nullptr if the parent no longer exists.
        return parent.lock();
    }

    //--------------------------------------------------------------------------
    // getSubDirectories Method
    //--------------------------------------------------------------------------

    const std::vector<std::shared_ptr<DirectoryTree::DirectoryNode>> &DirectoryNode::getSubDirectories() const
    {
        return subDirectories;
    }

    //--------------------------------------------------------------------------
    // getFileNodes Method
    //--------------------------------------------------------------------------

    const std::vector<std::unique_ptr<FileNodeGenerator::IGeneratedFile>> &DirectoryNode::getFileNodes() const
    {
        return fileNodes;
    }

} // namespace DirectoryTree
