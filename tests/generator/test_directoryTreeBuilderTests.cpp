#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "DirectoryTreeBuilder.h"
#include "DirectoryNode.h"
#include "CodeGroupModels.h"
#include "ProjectMetadata.h"
#include "FileNodeGenerator.h"
#include "testUtility.h"

using namespace CodeGroupModels;
using namespace DirectoryTree;
using namespace DirectoryTreeBuilder;
using namespace ProjectMetadata;
using namespace FileNodeGenerator;

// Utility to count all nodes in a directory tree recursively.
size_t countDirectoryNodes(const std::shared_ptr<DirectoryNode> &node)
{
    size_t count = 1;
    for (const auto &child : node->getSubDirectories())
    {
        count += countDirectoryNodes(child);
    }
    return count;
}

// Utility to collect all relative paths in a directory tree.
void collectPaths(const std::shared_ptr<DirectoryNode> &node, std::vector<std::string> &out)
{
    out.push_back(node->relativePath);
    for (const auto &child : node->getSubDirectories())
    {
        collectPaths(child, out);
    }
}

TEST(DirectoryTreeBuilderTests, Level1_MinimalProject)
{
    // Create a minimal project model with no content
    ProjectModel model("MyProject", "1.0", {}, {});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    // Validate root node
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_TRUE(root->getSubDirectories().empty());
    EXPECT_TRUE(root->getFileNodes().empty());

    // Validate directory structure
    EXPECT_EQ(countDirectoryNodes(root), 1);

    std::vector<std::string> paths;
    collectPaths(root, paths);
    ASSERT_EQ(paths.size(), 1);
    EXPECT_EQ(paths[0], "ROOT");

    // Validate metadata
    ASSERT_EQ(metadata.libraries.size(), 1);

    const auto &projLib = metadata.libraries["proj"];
    EXPECT_EQ(projLib.name, "MyProject");
    EXPECT_EQ(projLib.relativePath, "ROOT");
    EXPECT_TRUE(projLib.isProjLevel);
    EXPECT_TRUE(projLib.dependencies.empty());
}

TEST(DirectoryTreeBuilderTests, Level1_ProjectWithSingleClass)
{
    using namespace ClassModels;

    // Create a minimal project with one class
    ProjectModel model("CoreProject", "1.0", {"DepA", "DepB"}, {},
                       {}, {createDummyClass("Hero")});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");

    // Should have no subdirectories
    EXPECT_TRUE(root->getSubDirectories().empty());

    const auto &files = root->getFileNodes();
    ASSERT_EQ(files.size(), 1);
    auto &file = files[0];
    auto generated = file->generateFiles();
    EXPECT_EQ(generated.baseFilePath, "ROOT/Hero");
    EXPECT_TRUE(generated.headerContent.find("Hero") != std::string::npos);
    EXPECT_TRUE(generated.sourceContent.find("Hero") != std::string::npos);

    // Check metadata
    ASSERT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "CoreProject");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
    ASSERT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "DepA");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "DepB");
}

TEST(DirectoryTreeBuilderTests, Level1_ProjectWithSingleNamespace)
{
    // Create a minimal project with one namespace
    ProjectModel model("CoreProject", "1.0", {"DepA"}, {},
                       {}, {}, {createDummyNamespace("EngineSpace")});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_TRUE(root->getSubDirectories().empty());

    const auto &files = root->getFileNodes();
    ASSERT_EQ(files.size(), 1);
    auto &file = files[0];
    auto generated = file->generateFiles();
    EXPECT_EQ(generated.baseFilePath, "ROOT/EngineSpace");
    EXPECT_TRUE(generated.headerContent.find("EngineSpace") != std::string::npos);
    EXPECT_TRUE(generated.sourceContent.find("EngineSpace") != std::string::npos);

    // Check metadata
    ASSERT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "CoreProject");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
    ASSERT_EQ(metadata.libraries["proj"].dependencies.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "DepA");
}

TEST(DirectoryTreeBuilderTests, Level1_ProjectWithSingleFreeFunctionFile)
{
    // Create a minimal project with one free function file
    ProjectModel model("CoreProject", "1.0", {"DepX", "DepY"}, {},
                       {}, {}, {}, {createDummyFunction("UtilityFunc")});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_TRUE(root->getSubDirectories().empty());

    const auto &files = root->getFileNodes();
    ASSERT_EQ(files.size(), 1);
    auto &file = files[0];
    auto generated = file->generateFiles();
    EXPECT_EQ(generated.baseFilePath, "ROOT/ROOTFreeFunctions");
    EXPECT_TRUE(generated.headerContent.find("UtilityFunc") != std::string::npos);
    EXPECT_TRUE(generated.sourceContent.find("UtilityFunc") != std::string::npos);

    // Check metadata
    ASSERT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "CoreProject");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
    ASSERT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "DepX");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "DepY");
}

TEST(DirectoryTreeBuilderTests, Level1_ProjectWithMixedFiles)
{
    using namespace ClassModels;

    // Build a minimal project directly holding mixed file types
    ProjectModel model("MixedRoot", "0.4", {"Dep1", "Dep2"},
                       {},                                                    // No libraries
                       {},                                                    // No folders
                       {createDummyClass("Physics"), createDummyClass("AI")}, // Classes
                       {createDummyNamespace("UI")},                          // Namespaces
                       {createDummyFunction("UtilFunc")});                    // Free functions

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    // Root should be valid
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_TRUE(root->getSubDirectories().empty());

    // Expect 4 file nodes: 2 classes, 1 namespace, 1 free function
    const auto &files = root->getFileNodes();
    ASSERT_EQ(files.size(), 4);

    // Physics class
    {
        auto generated = files[0]->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/Physics");
        EXPECT_TRUE(generated.headerContent.find("Physics") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("Physics") != std::string::npos);
    }

    // AI class
    {
        auto generated = files[1]->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/AI");
        EXPECT_TRUE(generated.headerContent.find("AI") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("AI") != std::string::npos);
    }

    // UI namespace
    {
        auto generated = files[2]->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/UI");
        EXPECT_TRUE(generated.headerContent.find("UI") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("UI") != std::string::npos);
    }

    // Free functions
    {
        auto generated = files[3]->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/ROOTFreeFunctions");
        EXPECT_TRUE(generated.headerContent.find("UtilFunc") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("UtilFunc") != std::string::npos);
    }

    // Validate project metadata
    ASSERT_EQ(metadata.libraries.size(), 1);
    const auto &projMeta = metadata.libraries["proj"];
    EXPECT_EQ(projMeta.name, "MixedRoot");
    EXPECT_EQ(projMeta.relativePath, "ROOT");
    EXPECT_TRUE(projMeta.isProjLevel);
    ASSERT_EQ(projMeta.dependencies.size(), 2);
    EXPECT_EQ(projMeta.dependencies[0], "Dep1");
    EXPECT_EQ(projMeta.dependencies[1], "Dep2");
}

TEST(DirectoryTreeBuilderTests, Level2_FolderWithSingleClass)
{
    using namespace ClassModels;

    // Create a folder with one dummy class
    FolderModel folder("CoreFolder", {}, {createDummyClass("Engine")});

    // Wrap in a minimal ProjectModel
    ProjectModel model("MyGame", "1.0", {"Test1", "Test2"}, {}, {folder});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_EQ(root->getSubDirectories().size(), 1);

    auto coreFolder = root->getSubDirectories()[0];
    EXPECT_EQ(coreFolder->folderName, "CoreFolder");
    EXPECT_EQ(coreFolder->relativePath, "ROOT/CoreFolder");

    // Check that the class file node exists
    const auto &files = coreFolder->getFileNodes();
    ASSERT_EQ(files.size(), 1);
    auto &file = files[0];
    auto generated = file->generateFiles();
    EXPECT_EQ(generated.baseFilePath, "ROOT/CoreFolder/Engine");
    EXPECT_TRUE(generated.headerContent.find("Engine") != std::string::npos);
    EXPECT_TRUE(generated.sourceContent.find("Engine") != std::string::npos);

    // Check project metadata
    EXPECT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "MyGame");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "Test1");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "Test2");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
}

TEST(DirectoryTreeBuilderTests, Level2_FolderWithSingleNamespace)
{
    using namespace ClassModels;

    // Create a folder with one dummy namespace
    FolderModel folder("CoreFolder", {}, {}, {createDummyNamespace("EngineSpace")});

    // Wrap in a minimal ProjectModel
    ProjectModel model("MyGame", "1.1", {"Test1", "Test2"}, {}, {folder});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_EQ(root->getSubDirectories().size(), 1);

    auto coreFolder = root->getSubDirectories()[0];
    EXPECT_EQ(coreFolder->folderName, "CoreFolder");
    EXPECT_EQ(coreFolder->relativePath, "ROOT/CoreFolder");

    // Check that the class file node exists
    const auto &files = coreFolder->getFileNodes();
    ASSERT_EQ(files.size(), 1);
    auto &file = files[0];
    auto generated = file->generateFiles();
    EXPECT_EQ(generated.baseFilePath, "ROOT/CoreFolder/EngineSpace");
    EXPECT_TRUE(generated.headerContent.find("EngineSpace") != std::string::npos);
    EXPECT_TRUE(generated.sourceContent.find("EngineSpace") != std::string::npos);

    // Check project metadata
    EXPECT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "MyGame");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "Test1");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "Test2");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
}

TEST(DirectoryTreeBuilderTests, Level2_FolderWithSingleFreeFuncFile)
{
    using namespace ClassModels;

    // Create a folder with one dummy namespace
    FolderModel folder("CoreFolder", {}, {}, {}, {createDummyFunction("GameUtility")});

    // Wrap in a minimal ProjectModel
    ProjectModel model("MyGame", "1.2", {"Test1", "Test2"}, {}, {folder});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_EQ(root->getSubDirectories().size(), 1);

    auto coreFolder = root->getSubDirectories()[0];
    EXPECT_EQ(coreFolder->folderName, "CoreFolder");
    EXPECT_EQ(coreFolder->relativePath, "ROOT/CoreFolder");

    // Check that the class file node exists
    const auto &files = coreFolder->getFileNodes();
    ASSERT_EQ(files.size(), 1);
    auto &file = files[0];
    auto generated = file->generateFiles();
    EXPECT_EQ(generated.baseFilePath, "ROOT/CoreFolder/CoreFolderFreeFunctions");
    EXPECT_TRUE(generated.headerContent.find("GameUtility") != std::string::npos);
    EXPECT_TRUE(generated.sourceContent.find("GameUtility") != std::string::npos);

    // Check project metadata
    EXPECT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "MyGame");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "Test1");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "Test2");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
}

TEST(DirectoryTreeBuilderTests, Level2_FolderWithMixedFiles)
{
    using namespace ClassModels;

    // Create a folder with one dummy namespace
    FolderModel folder("CoreFolder", {}, {createDummyClass("Engine"), createDummyClass("Graphics")},
                       {createDummyNamespace("Animations"), createDummyNamespace("Mechanics")}, {createDummyFunction("GameUtility")});

    // Wrap in a minimal ProjectModel
    ProjectModel model("MyGame", "1.2", {"Test1", "Test2", "Test3"}, {}, {folder});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_EQ(root->getSubDirectories().size(), 1);

    auto coreFolder = root->getSubDirectories()[0];
    EXPECT_EQ(coreFolder->folderName, "CoreFolder");
    EXPECT_EQ(coreFolder->relativePath, "ROOT/CoreFolder");

    // Check that the class file node exists
    const auto &files = coreFolder->getFileNodes();
    ASSERT_EQ(files.size(), 5);
    { // Check first class file
        auto &file = files[0];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreFolder/Engine");
        EXPECT_TRUE(generated.headerContent.find("Engine") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("Engine") != std::string::npos);
    }
    { // Check second class file
        auto &file = files[1];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreFolder/Graphics");
        EXPECT_TRUE(generated.headerContent.find("Graphics") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("Graphics") != std::string::npos);
    }
    { // Check first namespace file
        auto &file = files[2];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreFolder/Animations");
        EXPECT_TRUE(generated.headerContent.find("Animations") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("Animations") != std::string::npos);
    }
    {
        // Check second namespace file
        auto &file = files[3];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreFolder/Mechanics");
        EXPECT_TRUE(generated.headerContent.find("Mechanics") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("Mechanics") != std::string::npos);
    }
    {
        // Check free functions file
        auto &file = files[4];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreFolder/CoreFolderFreeFunctions");
        EXPECT_TRUE(generated.headerContent.find("GameUtility") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("GameUtility") != std::string::npos);
    }

    // Check project metadata
    EXPECT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "MyGame");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_EQ(metadata.libraries["proj"].dependencies.size(), 3);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "Test1");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "Test2");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[2], "Test3");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
}

TEST(DirectoryTreeBuilderTests, Level2_LibraryWithSingleClass)
{
    using namespace ClassModels;

    // Create a folder with one dummy class
    LibraryModel library("CoreLibrary", "1.0", {"Lib1", "Lib2"}, {}, {createDummyClass("Engine")});

    // Wrap in a minimal ProjectModel
    ProjectModel model("MyGame", "1.0", {"Test1", "Test2"}, {library});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_EQ(root->getSubDirectories().size(), 1);

    auto coreFolder = root->getSubDirectories()[0];
    EXPECT_EQ(coreFolder->folderName, "CoreLibrary");
    EXPECT_EQ(coreFolder->relativePath, "ROOT/CoreLibrary");

    // Check that the class file node exists
    const auto &files = coreFolder->getFileNodes();
    ASSERT_EQ(files.size(), 1);
    auto &file = files[0];
    auto generated = file->generateFiles();
    EXPECT_EQ(generated.baseFilePath, "ROOT/CoreLibrary/Engine");
    EXPECT_TRUE(generated.headerContent.find("Engine") != std::string::npos);
    EXPECT_TRUE(generated.sourceContent.find("Engine") != std::string::npos);

    // Check project metadata
    EXPECT_EQ(metadata.libraries.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].name, "MyGame");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "Test1");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "Test2");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].name, "CoreLibrary");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].relativePath, "ROOT/CoreLibrary");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["CoreLibrary"].dependencies[0], "Lib1");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].dependencies[1], "Lib2");
}

TEST(DirectoryTreeBuilderTests, Level2_LibraryWithSingleNamespace)
{
    using namespace ClassModels;

    // Create a library with one dummy namespace
    LibraryModel library("CoreLibrary", "1.0", {"Lib1", "Lib2"}, {}, {}, {createDummyNamespace("UtilityNS")});

    // Wrap in a minimal ProjectModel
    ProjectModel model("MyGame", "1.0", {"Test1", "Test2"}, {library});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_EQ(root->getSubDirectories().size(), 1);

    auto coreFolder = root->getSubDirectories()[0];
    EXPECT_EQ(coreFolder->folderName, "CoreLibrary");
    EXPECT_EQ(coreFolder->relativePath, "ROOT/CoreLibrary");

    // Check that the namespace file node exists
    const auto &files = coreFolder->getFileNodes();
    ASSERT_EQ(files.size(), 1);
    auto &file = files[0];
    auto generated = file->generateFiles();
    EXPECT_EQ(generated.baseFilePath, "ROOT/CoreLibrary/UtilityNS");
    EXPECT_TRUE(generated.headerContent.find("UtilityNS") != std::string::npos);
    EXPECT_TRUE(generated.sourceContent.find("UtilityNS") != std::string::npos);

    // Check project metadata
    ASSERT_EQ(metadata.libraries.size(), 2);

    // Project-level metadata
    EXPECT_EQ(metadata.libraries["proj"].name, "MyGame");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
    ASSERT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "Test1");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "Test2");

    // Library metadata
    EXPECT_EQ(metadata.libraries["CoreLibrary"].name, "CoreLibrary");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].relativePath, "ROOT/CoreLibrary");
    EXPECT_FALSE(metadata.libraries["CoreLibrary"].isProjLevel);
    ASSERT_EQ(metadata.libraries["CoreLibrary"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["CoreLibrary"].dependencies[0], "Lib1");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].dependencies[1], "Lib2");
}

TEST(DirectoryTreeBuilderTests, Level2_LibraryWithSingleFreeFuncFile)
{
    using namespace ClassModels;

    // Create a library with one dummy free function
    LibraryModel library("CoreLibrary", "1.0", {"Lib1", "Lib2"}, {}, {}, {}, {createDummyFunction("HelperFunc")});

    // Wrap in a minimal ProjectModel
    ProjectModel model("MyGame", "1.0", {"Test1", "Test2"}, {library});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_EQ(root->getSubDirectories().size(), 1);

    auto coreFolder = root->getSubDirectories()[0];
    EXPECT_EQ(coreFolder->folderName, "CoreLibrary");
    EXPECT_EQ(coreFolder->relativePath, "ROOT/CoreLibrary");

    // Check that the free function file node exists
    const auto &files = coreFolder->getFileNodes();
    ASSERT_EQ(files.size(), 1);
    auto &file = files[0];
    auto generated = file->generateFiles();
    EXPECT_EQ(generated.baseFilePath, "ROOT/CoreLibrary/CoreLibraryFreeFunctions");
    EXPECT_TRUE(generated.headerContent.find("HelperFunc") != std::string::npos);
    EXPECT_TRUE(generated.sourceContent.find("HelperFunc") != std::string::npos);

    // Check project metadata
    ASSERT_EQ(metadata.libraries.size(), 2);

    // Project-level metadata
    EXPECT_EQ(metadata.libraries["proj"].name, "MyGame");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
    ASSERT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "Test1");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "Test2");

    // Library metadata
    EXPECT_EQ(metadata.libraries["CoreLibrary"].name, "CoreLibrary");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].relativePath, "ROOT/CoreLibrary");
    EXPECT_FALSE(metadata.libraries["CoreLibrary"].isProjLevel);
    ASSERT_EQ(metadata.libraries["CoreLibrary"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["CoreLibrary"].dependencies[0], "Lib1");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].dependencies[1], "Lib2");
}

TEST(DirectoryTreeBuilderTests, Level2_LibraryWithMixedFiles)
{
    using namespace ClassModels;

    // Create a library with mixed file types
    LibraryModel library("CoreLibrary", "1.0",
                         {"Lib1", "Lib2"},
                         {}, // subfolders
                         {createDummyClass("Engine"), createDummyClass("Graphics")},
                         {createDummyNamespace("UI"), createDummyNamespace("Network")},
                         {createDummyFunction("Helpers")});

    // Wrap in a minimal ProjectModel
    ProjectModel model("MyGame", "1.0", {"Test1", "Test2"}, {library});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    ASSERT_EQ(root->getSubDirectories().size(), 1);

    auto coreFolder = root->getSubDirectories()[0];
    EXPECT_EQ(coreFolder->folderName, "CoreLibrary");
    EXPECT_EQ(coreFolder->relativePath, "ROOT/CoreLibrary");

    // Validate all 5 generated file nodes
    const auto &files = coreFolder->getFileNodes();
    ASSERT_EQ(files.size(), 5);

    // Check class files
    {
        auto &file = files[0];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreLibrary/Engine");
        EXPECT_TRUE(generated.headerContent.find("Engine") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("Engine") != std::string::npos);
    }
    {
        auto &file = files[1];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreLibrary/Graphics");
        EXPECT_TRUE(generated.headerContent.find("Graphics") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("Graphics") != std::string::npos);
    }

    // Check namespace files
    {
        auto &file = files[2];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreLibrary/UI");
        EXPECT_TRUE(generated.headerContent.find("UI") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("UI") != std::string::npos);
    }
    {
        auto &file = files[3];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreLibrary/Network");
        EXPECT_TRUE(generated.headerContent.find("Network") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("Network") != std::string::npos);
    }

    // Check free function file
    {
        auto &file = files[4];
        auto generated = file->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/CoreLibrary/CoreLibraryFreeFunctions");
        EXPECT_TRUE(generated.headerContent.find("Helpers") != std::string::npos);
        EXPECT_TRUE(generated.sourceContent.find("Helpers") != std::string::npos);
    }

    // Check project metadata
    ASSERT_EQ(metadata.libraries.size(), 2);

    // Project-level
    EXPECT_EQ(metadata.libraries["proj"].name, "MyGame");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
    ASSERT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "Test1");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "Test2");

    // Library-level
    EXPECT_EQ(metadata.libraries["CoreLibrary"].name, "CoreLibrary");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].relativePath, "ROOT/CoreLibrary");
    EXPECT_FALSE(metadata.libraries["CoreLibrary"].isProjLevel);
    ASSERT_EQ(metadata.libraries["CoreLibrary"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["CoreLibrary"].dependencies[0], "Lib1");
    EXPECT_EQ(metadata.libraries["CoreLibrary"].dependencies[1], "Lib2");
}

TEST(DirectoryTreeBuilderTests, EdgeCase_EmptyProjectWithNullMetadata)
{
    ProjectModel model("EdgeProject", "0.0", {}, {}, {});

    // Pass nullptr for metadata — this should still work for ProjectModel
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->folderName, "ROOT");
    EXPECT_EQ(root->relativePath, "ROOT");
    EXPECT_TRUE(root->getSubDirectories().empty());
    EXPECT_TRUE(root->getFileNodes().empty());

    ASSERT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "EdgeProject");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
    EXPECT_TRUE(metadata.libraries["proj"].dependencies.empty());
}

TEST(DirectoryTreeBuilderTests, EdgeCase_ProjectWithOnlyDependencies)
{
    ProjectModel model("DepOnlyProject", "1.2", {"Dep1", "Dep2"}, {}, {});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    EXPECT_TRUE(root->getSubDirectories().empty());
    EXPECT_TRUE(root->getFileNodes().empty());

    ASSERT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "DepOnlyProject");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
    EXPECT_TRUE(metadata.libraries["proj"].isProjLevel);
    ASSERT_EQ(metadata.libraries["proj"].dependencies.size(), 2);
    EXPECT_EQ(metadata.libraries["proj"].dependencies[0], "Dep1");
    EXPECT_EQ(metadata.libraries["proj"].dependencies[1], "Dep2");
}

TEST(DirectoryTreeBuilderTests, EdgeCase_ProjectWithEmptyFolder)
{
    FolderModel emptyFolder("EmptyZone");

    ProjectModel model("LonelyProject", "0.9", {}, {}, {emptyFolder});
    ProjMetadata metadata({});

    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getSubDirectories().size(), 1);
    auto sub = root->getSubDirectories()[0];
    EXPECT_EQ(sub->folderName, "EmptyZone");
    EXPECT_TRUE(sub->getSubDirectories().empty());
    EXPECT_TRUE(sub->getFileNodes().empty());

    ASSERT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "LonelyProject");
    EXPECT_EQ(metadata.libraries["proj"].relativePath, "ROOT");
}

TEST(DirectoryTreeBuilderTests, EdgeCase_DeeplyNestedFoldersWithNoFiles)
{
    FolderModel level3("Level3");
    FolderModel level2("Level2", {level3});
    FolderModel level1("Level1", {level2});

    ProjectModel model("DeepProject", "1.0", {}, {}, {level1});
    ProjMetadata metadata({});

    auto root = buildDirectoryTree(model, metadata);

    ASSERT_NE(root, nullptr);
    auto l1 = root->getSubDirectories()[0];
    EXPECT_EQ(l1->folderName, "Level1");

    auto l2 = l1->getSubDirectories()[0];
    EXPECT_EQ(l2->folderName, "Level2");

    auto l3 = l2->getSubDirectories()[0];
    EXPECT_EQ(l3->folderName, "Level3");

    EXPECT_TRUE(l3->getFileNodes().empty());

    ASSERT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "DeepProject");
}

TEST(DirectoryTreeBuilderTests, EdgeCase_DuplicateClassNamesDifferentScopes)
{
    using namespace ClassModels;

    FolderModel coreA("CoreA", {}, {createDummyClass("Duplicated")});
    FolderModel coreB("CoreB", {}, {createDummyClass("Duplicated")});

    ProjectModel model("ScopeProject", "1.0", {}, {}, {coreA, coreB});
    ProjMetadata metadata({});

    auto root = buildDirectoryTree(model, metadata);
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getSubDirectories().size(), 2);

    for (const auto &dir : root->getSubDirectories())
    {
        const auto &files = dir->getFileNodes();
        ASSERT_EQ(files.size(), 1);
        auto generated = files[0]->generateFiles();
        EXPECT_EQ(generated.baseFilePath, "ROOT/" + dir->folderName + "/Duplicated");
        EXPECT_TRUE(generated.headerContent.find("Duplicated") != std::string::npos);
    }

    ASSERT_EQ(metadata.libraries.size(), 1);
    EXPECT_EQ(metadata.libraries["proj"].name, "ScopeProject");
}
