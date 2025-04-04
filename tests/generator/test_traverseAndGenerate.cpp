#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "DirectoryTreeBuilder.h"
#include "DirectoryNode.h"
#include "FileNodeGenerator.h"
#include "ProjectMetadata.h"
#include "testUtility.h" // Contains helper functions like createDummyClass, createDummyNamespace, etc.
#include "TraverseAndGenerate.h"

using namespace CodeGroupModels;
using namespace DirectoryTree;
using namespace DirectoryTreeBuilder;
using namespace ProjectMetadata;
using namespace FileNodeGenerator;
using namespace FileGeneration;

// Test case: Minimal project with a single class file.
TEST(TraverseAndGenerateTests, SingleFileTest)
{
    // Create a minimal project model with one class "Hero".
    ProjectModel model("CoreProject", "1.0", {}, {},
                       {}, {createDummyClass("Hero")});
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // Create an instance of the stub file writer.
    TestFileWriter testWriter;

    // Traverse the directory tree and generate file writes.
    traverseAndGenerate(root, testWriter);

    // Expect exactly two calls (header and source) for the class file.
    ASSERT_EQ(testWriter.calls.size(), 2);

    // Check that both header and source files were generated with the expected path and content.
    bool headerFound = false;
    bool sourceFound = false;
    for (const auto &call : testWriter.calls)
    {
        if (call.type == "header")
        {
            headerFound = true;
            EXPECT_EQ(call.filePath, "ROOT/Hero");
            EXPECT_NE(call.content.find("Hero"), std::string::npos);
        }
        else if (call.type == "source")
        {
            sourceFound = true;
            EXPECT_EQ(call.filePath, "ROOT/Hero");
            EXPECT_NE(call.content.find("Hero"), std::string::npos);
        }
    }
    EXPECT_TRUE(headerFound);
    EXPECT_TRUE(sourceFound);
}

// Test case: Project with a folder containing a single class.
TEST(TraverseAndGenerateTests, NestedDirectoryTest)
{
    // Create a folder containing one dummy class "Engine".
    FolderModel folder("CoreFolder", {}, {createDummyClass("Engine")});
    // Wrap the folder in a minimal project.
    ProjectModel model("MyGame", "1.0", {}, {}, {folder});
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // Create the test stub writer.
    TestFileWriter testWriter;

    // Traverse the directory tree and perform file generation.
    traverseAndGenerate(root, testWriter);

    // Expect exactly two calls for the file node in the folder "CoreFolder".
    ASSERT_EQ(testWriter.calls.size(), 2);

    // Check that both header and source files are generated with the expected path and content.
    for (const auto &call : testWriter.calls)
    {
        EXPECT_EQ(call.filePath, "ROOT/CoreFolder/Engine");
        EXPECT_NE(call.content.find("Engine"), std::string::npos);
    }
}

// Test case: A complex project that mixes classes, namespaces, and free functions
TEST(TraverseAndGenerateTests, ComplexProjectTest)
{
    // Create a project with:
    // - Two classes at project level: "Player" and "Enemy"
    // - One namespace at project level: "GameLogic" with its own dummy members
    // - One free function file at project level: "GlobalUtil"
    ProjectModel model("ComplexProject", "2.0", {"LibX", "LibY"}, {},
                       {},
                       {createDummyClass("Player"), createDummyClass("Enemy")},
                       {createDummyNamespace("GameLogic")},
                       {createDummyFunction("GlobalUtil")});

    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // Create a TestFileWriter stub to capture file write operations.
    TestFileWriter testWriter;

    // Perform the traverse-and-generate operation.
    traverseAndGenerate(root, testWriter);

    // Count the expected file nodes:
    // 2 for classes, 1 for the namespace, and 1 for free functions = 4 file nodes, each producing 2 calls.
    const size_t expectedCalls = 4 * 2;
    ASSERT_EQ(testWriter.calls.size(), expectedCalls);

    // Verify that each generated file node's base path and content include the correct identifiers.
    // For the class "Player" and "Enemy", namespace "GameLogic", and free function file "ROOTFreeFunctions"
    bool playerFound = false, enemyFound = false, logicFound = false, globalUtilFound = false;
    for (const auto &call : testWriter.calls)
    {
        if (call.filePath.find("Player") != std::string::npos)
        {
            playerFound = true;
            EXPECT_NE(call.content.find("Player"), std::string::npos);
        }
        if (call.filePath.find("Enemy") != std::string::npos)
        {
            enemyFound = true;
            EXPECT_NE(call.content.find("Enemy"), std::string::npos);
        }
        if (call.filePath.find("GameLogic") != std::string::npos)
        {
            logicFound = true;
            EXPECT_NE(call.content.find("GameLogic"), std::string::npos);
        }
        if (call.filePath.find("ROOTFreeFunctions") != std::string::npos ||
            call.content.find("GlobalUtil") != std::string::npos)
        {
            globalUtilFound = true;
        }
    }
    EXPECT_TRUE(playerFound);
    EXPECT_TRUE(enemyFound);
    EXPECT_TRUE(logicFound);
    EXPECT_TRUE(globalUtilFound);
}

// Test case: A deeply nested project with multiple folders and mixed file types.
TEST(TraverseAndGenerateTests, DeeplyNestedProjectTest)
{
    // Build a nested folder structure:
    //   ROOT/
    //     Core/
    //       Graphics/         -> contains a class "Renderer"
    //       Physics/          -> contains a class "Collider"
    //     Utilities/            -> contains a namespace "HelperNS" and a free function file "Utils"
    FolderModel graphicsFolder("Graphics", {}, {createDummyClass("Renderer")});
    FolderModel physicsFolder("Physics", {}, {createDummyClass("Collider")});

    // Create a folder "Core" that contains the "Graphics" and "Physics" folders.
    FolderModel coreFolder("Core", {graphicsFolder, physicsFolder});

    // Create a folder "Utilities" with a namespace and free function.
    FolderModel utilitiesFolder("Utilities", {}, {}, {createDummyNamespace("HelperNS")}, {createDummyFunction("Utils")});

    // Wrap the folders into a ProjectModel.
    ProjectModel model("DeepProject", "3.0", {"DepA"}, {}, {coreFolder, utilitiesFolder});
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // Create the stub file writer.
    TestFileWriter testWriter;

    // Execute the traverse-and-generate.
    traverseAndGenerate(root, testWriter);

    // Expect file nodes:
    // In Core/Graphics: 1 (class) -> 2 calls.
    // In Core/Physics: 1 (class) -> 2 calls.
    // In Utilities: 1 (namespace) + 1 (free functions) -> 4 calls.
    // Total = 2 + 2 + 4 = 8 calls.
    ASSERT_EQ(testWriter.calls.size(), 8);

    // Verify the paths and contents.
    bool rendererFound = false, colliderFound = false, helperNSFound = false, utilsFound = false;
    for (const auto &call : testWriter.calls)
    {
        if (call.filePath.find("Graphics/Renderer") != std::string::npos)
        {
            rendererFound = true;
            EXPECT_NE(call.content.find("Renderer"), std::string::npos);
        }
        if (call.filePath.find("Physics/Collider") != std::string::npos)
        {
            colliderFound = true;
            EXPECT_NE(call.content.find("Collider"), std::string::npos);
        }
        if (call.filePath.find("Utilities/HelperNS") != std::string::npos)
        {
            helperNSFound = true;
            EXPECT_NE(call.content.find("HelperNS"), std::string::npos);
        }
        if (call.filePath.find("Utilities/UtilitiesFreeFunctions") != std::string::npos ||
            call.content.find("Utils") != std::string::npos)
        {
            utilsFound = true;
        }
    }
    EXPECT_TRUE(rendererFound);
    EXPECT_TRUE(colliderFound);
    EXPECT_TRUE(helperNSFound);
    EXPECT_TRUE(utilsFound);
}

// Test: Deep tree with weighted file distribution at different directory levels.
TEST(TraverseAndGenerateTests, DeepTreeWeightedTest)
{
    // Build a deep directory tree where:
    // Level 1: "Level1" folder has 1 file (a class).
    // Level 2: Under "Level1", a folder "Level2" with 3 file nodes:
    //          - 1 class, 1 namespace, 1 free function file.
    // Level 3: Under "Level2", a folder "Level3" with 2 files (both classes).

    // Level 3 folder with two classes.
    FolderModel level3("Level3", {},
                       {createDummyClass("ClassA"), createDummyClass("ClassB")});

    // Level 2 folder with one class, one namespace, and one free function.
    FolderModel level2("Level2", {level3},
                       {createDummyClass("ClassC")},
                       {createDummyNamespace("NS_Level2")},
                       {createDummyFunction("FreeFunc_Level2")});

    // Level 1 folder with one class.
    FolderModel level1("Level1", {level2},
                       {createDummyClass("ClassD")});

    // Wrap the folder in a ProjectModel.
    ProjectModel model("WeightedProject", "1.0", {"W1"}, {}, {level1});
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // Create our test stub writer.
    TestFileWriter testWriter;

    // Perform traverse and generate.
    traverseAndGenerate(root, testWriter);

    // Expected file nodes:
    // - Level1: ClassD -> 2 calls.
    // - Level2: ClassC, NS_Level2, and free function file -> 3 nodes * 2 = 6 calls.
    // - Level3: ClassA and ClassB -> 2 nodes * 2 = 4 calls.
    const size_t expectedCalls = 2 + 6 + 4;
    ASSERT_EQ(testWriter.calls.size(), expectedCalls);

    // Verify that files in each level have the correct weighted paths.
    // For example, Level1 should have base path: "ROOT/Level1/ClassD"
    bool classDFound = false, classCFound = false, nsLevel2Found = false;
    bool freeFuncLevel2Found = false, classAFound = false, classBFound = false;
    for (const auto &call : testWriter.calls)
    {
        if (call.filePath == "ROOT/Level1/ClassD")
        {
            classDFound = true;
            EXPECT_NE(call.content.find("ClassD"), std::string::npos);
        }
        if (call.filePath == "ROOT/Level1/Level2/ClassC")
        {
            classCFound = true;
            EXPECT_NE(call.content.find("ClassC"), std::string::npos);
        }
        if (call.filePath == "ROOT/Level1/Level2/NS_Level2")
        {
            nsLevel2Found = true;
            EXPECT_NE(call.content.find("NS_Level2"), std::string::npos);
        }
        // Free function file may follow a default naming convention (e.g., "ROOT/Level1/Level2/Level2FreeFunctions")
        if (call.filePath.find("Level2FreeFunctions") != std::string::npos ||
            call.content.find("FreeFunc_Level2") != std::string::npos)
        {
            freeFuncLevel2Found = true;
        }
        if (call.filePath == "ROOT/Level1/Level2/Level3/ClassA")
        {
            classAFound = true;
            EXPECT_NE(call.content.find("ClassA"), std::string::npos);
        }
        if (call.filePath == "ROOT/Level1/Level2/Level3/ClassB")
        {
            classBFound = true;
            EXPECT_NE(call.content.find("ClassB"), std::string::npos);
        }
    }
    EXPECT_TRUE(classDFound);
    EXPECT_TRUE(classCFound);
    EXPECT_TRUE(nsLevel2Found);
    EXPECT_TRUE(freeFuncLevel2Found);
    EXPECT_TRUE(classAFound);
    EXPECT_TRUE(classBFound);
}

// Test: A large, deep project filled with classes at many nested levels.
TEST(TraverseAndGenerateTests, DeepBigProjectTest)
{
    // Build a deep tree that simulates a large project.
    // We'll generate a tree 5 levels deep with each level having 3 subfolders.
    // Each folder at the deepest level will contain 4 class file nodes.

    const int levels = 5;
    const int subfoldersPerLevel = 3;
    const int filesPerDeepFolder = 4;

    // Recursive lambda to build the tree.
    std::function<FolderModel(int, const std::string &)> buildDeepTree;
    buildDeepTree = [&](int currentLevel, const std::string &folderName) -> FolderModel
    {
        if (currentLevel == levels)
        {
            // At the deepest level, create a folder with a set of class files.
            std::vector<ClassModels::ClassModel> classFiles;
            for (int i = 0; i < filesPerDeepFolder; i++)
            {
                // Construct a unique class name based on folder name and index.
                std::string className = folderName + "_Class" + std::to_string(i);
                classFiles.push_back(createDummyClass(className));
            }
            return FolderModel(folderName, {}, classFiles);
        }
        else
        {
            // Otherwise, create subfolders and leave the file nodes empty at this level.
            std::vector<FolderModel> subfolders;
            for (int i = 0; i < subfoldersPerLevel; i++)
            {
                // Create a subfolder with a unique name.
                std::string subfolderName = folderName + "_Sub" + std::to_string(i);
                subfolders.push_back(buildDeepTree(currentLevel + 1, subfolderName));
            }
            return FolderModel(folderName, subfolders);
        }
    };

    // Build the deep tree starting with a root folder "DeepRoot".
    FolderModel deepRoot = buildDeepTree(1, "DeepRoot");

    // Wrap the deep tree inside a project model.
    ProjectModel model("BigDeepProject", "5.0", {"BigDep"}, {}, {deepRoot});
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // Create our test stub writer.
    TestFileWriter testWriter;

    // Traverse and generate files.
    traverseAndGenerate(root, testWriter);

    // Calculate expected file write calls:
    // The tree has a single branch at the top level (folder "DeepRoot") with a branching factor of 3 at each level for levels 2..5.
    // Total deepest folders = subfoldersPerLevel^(levels - 1)
    int deepestFolders = 1;
    for (int i = 1; i < levels; i++)
    {
        deepestFolders *= subfoldersPerLevel;
    }
    // Each deepest folder contains filesPerDeepFolder nodes, each generating 2 calls.
    int expectedCalls = deepestFolders * filesPerDeepFolder * 2;
    ASSERT_EQ(testWriter.calls.size(), static_cast<size_t>(expectedCalls));

    // Optionally, verify that at least one file comes from a known deep branch.
    // We check that at least one call's file path contains "DeepRoot_Sub0" (a substring unique to the generated names).
    std::string expectedSubfolder = "DeepRoot_Sub0";
    bool foundAtLeastOne = false;
    for (const auto &call : testWriter.calls)
    {
        if (call.filePath.find(expectedSubfolder) != std::string::npos)
        {
            foundAtLeastOne = true;
            break;
        }
    }
    EXPECT_TRUE(foundAtLeastOne);
}

// Edge Case 1: An empty project that produces no file nodes.
TEST(TraverseAndGenerateEdgeCases, EmptyProjectNoFiles) {
    // Create a project with no classes, namespaces, or free functions.
    ProjectModel model("EmptyProject", "0.1", {}, {}, {});
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // Ensure the directory tree contains no file nodes.
    EXPECT_TRUE(root->getFileNodes().empty());

    // Create our TestFileWriter stub.
    TestFileWriter testWriter;

    // Traverse the tree.
    traverseAndGenerate(root, testWriter);

    // No file nodes mean no file writes.
    EXPECT_EQ(testWriter.calls.size(), 0);
}

// Edge Case 2: A project with empty folders (folders exist but contain no file nodes).
TEST(TraverseAndGenerateEdgeCases, EmptyFolders) {
    // Create an empty folder.
    FolderModel emptyFolder("EmptyFolder");

    // Wrap the empty folder into a project model.
    ProjectModel model("FolderProject", "0.5", {}, {}, {emptyFolder});
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // Check that the folder exists and is empty.
    ASSERT_EQ(root->getSubDirectories().size(), 1);
    auto folderNode = root->getSubDirectories()[0];
    EXPECT_EQ(folderNode->folderName, "EmptyFolder");
    EXPECT_TRUE(folderNode->getFileNodes().empty());

    // Create the stub writer.
    TestFileWriter testWriter;

    // Traverse the tree.
    traverseAndGenerate(root, testWriter);

    // No file nodes were present so no write calls should be recorded.
    EXPECT_EQ(testWriter.calls.size(), 0);
}

// Edge Case 3: Duplicate file names across different scopes.
// This ensures that even if two file nodes generate the same base file path,
// the traverse-and-generate process logs each call separately.
TEST(TraverseAndGenerateEdgeCases, DuplicateFileNames) {
    using namespace ClassModels;

    // Create two folders with a class of the same name "Duplicate".
    FolderModel folderA("FolderA", {}, { createDummyClass("Duplicate") });
    FolderModel folderB("FolderB", {}, { createDummyClass("Duplicate") });

    // Wrap both folders in a project.
    ProjectModel model("DuplicateProject", "1.0", {}, {}, { folderA, folderB });
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // There should be two subdirectories each containing one file node.
    ASSERT_EQ(root->getSubDirectories().size(), 2);

    // Create the stub writer.
    TestFileWriter testWriter;

    // Traverse the tree.
    traverseAndGenerate(root, testWriter);

    // Each file node generates two write calls, so expect 2 folders * 1 file each * 2 = 4 calls.
    EXPECT_EQ(testWriter.calls.size(), 4);

    // Verify that each call is recorded even if the base file path is the same at the file level.
    int duplicateCount = 0;
    for (const auto &call : testWriter.calls) {
        if (call.filePath.find("Duplicate") != std::string::npos) {
            duplicateCount++;
            EXPECT_NE(call.content.find("Duplicate"), std::string::npos);
        }
    }
    EXPECT_EQ(duplicateCount, 4);
}

// Edge Case 4: A project with only dependencies (no file nodes generated).
TEST(TraverseAndGenerateEdgeCases, ProjectWithOnlyDependencies) {
    // Create a project that has dependencies defined but no DSL objects (no classes, namespaces, or free functions).
    ProjectModel model("DepOnlyProject", "1.2", {"Dep1", "Dep2"}, {}, {});
    ProjMetadata metadata({});
    auto root = buildDirectoryTree(model, &metadata);
    ASSERT_NE(root, nullptr);

    // No file nodes should be present.
    EXPECT_TRUE(root->getFileNodes().empty());

    // Create the stub writer.
    TestFileWriter testWriter;

    // Traverse the tree.
    traverseAndGenerate(root, testWriter);

    // Expect no file write calls.
    EXPECT_EQ(testWriter.calls.size(), 0);
}