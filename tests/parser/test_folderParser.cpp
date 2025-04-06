#include <gtest/gtest.h>
#include "FolderParser.h"
#include "CodeGroupModels.h"  // Provides FolderModel and related models.
#include <string_view>
#include <deque>

using namespace FolderParser;
using namespace CodeGroupModels;

TEST(FolderParserTest, ParsesEmptyFolderBlock) {
    // A folder block with no content (only the end marker).
    std::deque<std::string_view> lines = { "_" };
    FolderModel folder = parseFolderBlock("EmptyFolder", lines);

    EXPECT_EQ(folder.name, "EmptyFolder");
    EXPECT_TRUE(folder.subFolders.empty());
    EXPECT_TRUE(folder.classFiles.empty());
    EXPECT_TRUE(folder.namespaceFiles.empty());
    EXPECT_TRUE(folder.functionFile.empty());
}

TEST(FolderParserTest, ParsesFolderWithNestedClass) {
    // A folder block containing a nested class block.
    std::deque<std::string_view> lines = {
        "- class MyClass:",
        "| description = \"A class in folder\"",
        "_",
        "_" // End folder block.
    };

    FolderModel folder = parseFolderBlock("FolderWithClass", lines);
    EXPECT_EQ(folder.name, "FolderWithClass");
    ASSERT_EQ(folder.classFiles.size(), 1);
    EXPECT_EQ(folder.classFiles[0].name, "MyClass");
    EXPECT_TRUE(folder.subFolders.empty());
    EXPECT_TRUE(folder.namespaceFiles.empty());
    EXPECT_TRUE(folder.functionFile.empty());
}

TEST(FolderParserTest, ParsesFolderWithNestedNamespace) {
    // A folder block containing a nested namespace block.
    std::deque<std::string_view> lines = {
        "- namespace MyNamespace:",
        "| description = \"A namespace in folder\"",
        "_",
        "_" // End folder block.
    };

    FolderModel folder = parseFolderBlock("FolderWithNS", lines);
    EXPECT_EQ(folder.name, "FolderWithNS");
    ASSERT_EQ(folder.namespaceFiles.size(), 1);
    EXPECT_EQ(folder.namespaceFiles[0].name, "MyNamespace");
}

TEST(FolderParserTest, ParsesFolderWithFunctionGroup) {
    // A folder block containing consecutive free function blocks.
    std::deque<std::string_view> lines = {
        "- function funcOne:",
        "| declaration = inline",
        "| return = int",
        "| parameters = param:int",
        "| description = \"First free function\"",
        "_",
        "- function funcTwo:",
        "| declaration = static",
        "| return = void",
        "| parameters = ",
        "| description = \"Second free function\"",
        "_",
        "_" // End folder block.
    };

    FolderModel folder = parseFolderBlock("FolderWithFunctions", lines);
    EXPECT_EQ(folder.name, "FolderWithFunctions");
    // All free functions should now be in a single vector.
    ASSERT_EQ(folder.functionFile.size(), 2);
    EXPECT_EQ(folder.functionFile[0].name, "funcOne");
    EXPECT_EQ(folder.functionFile[1].name, "funcTwo");
}

TEST(FolderParserTest, ParsesFolderWithNestedFolder) {
    // A folder block containing a nested folder block.
    std::deque<std::string_view> lines = {
        "- folder SubFolder:",
        "- class SubClass:",
        "| description = \"Class in subfolder\"",
        "_",
        "_" , // End nested folder block.
        "_"  // End parent folder block.
    };

    FolderModel folder = parseFolderBlock("ParentFolder", lines);
    EXPECT_EQ(folder.name, "ParentFolder");
    ASSERT_EQ(folder.subFolders.size(), 1);
    FolderModel subFolder = folder.subFolders[0];
    EXPECT_EQ(subFolder.name, "SubFolder");
    ASSERT_EQ(subFolder.classFiles.size(), 1);
    EXPECT_EQ(subFolder.classFiles[0].name, "SubClass");
}

TEST(FolderParserTest, ThrowsErrorOnMethodInFolder) {
    // A folder block that attempts to declare a method, which is not allowed.
    std::deque<std::string_view> lines = {
        "- method invalidMethod:",
        "| return = void",
        "| parameters = ",
        "| description = \"Invalid method in folder\"",
        "_",
        "_" // End folder block.
    };

    EXPECT_THROW({
        FolderModel folder = parseFolderBlock("BadFolder", lines);
    }, std::runtime_error);
}

TEST(FolderParserTest, ThrowsOnPropertyInFolder) {
    // Folder blocks do not support property lines.
    std::deque<std::string_view> lines = {
        "| description = \"Folder property not allowed\"",
        "_"
    };

    EXPECT_THROW({
        FolderModel folder = parseFolderBlock("PropertyFolder", lines);
    }, std::runtime_error);
}

TEST(FolderParserTest, IgnoresTrailingGarbage) {
    // Trailing non-DSL text should be ignored.
    std::deque<std::string_view> lines = {
        "- class TrailingClass:",
        "| description = \"A class in folder\"",
        "_",
        "_" , // End folder block.
        "non DSL text",
        "- something irrelevant:"
    };

    FolderModel folder = parseFolderBlock("FolderTrailing", lines);
    ASSERT_EQ(folder.classFiles.size(), 1);
    EXPECT_EQ(folder.classFiles[0].name, "TrailingClass");
}

TEST(FolderParserTest, ParsesMixedNestedContentInFolder) {
    // A folder block with mixed nested content: free functions, a class, and a nested namespace.
    std::deque<std::string_view> lines = {
        "- function freeFunc1:",
        "| declaration = inline",
        "| return = int",
        "| parameters = param1:int",
        "| description = \"First free function\"",
        "_",
        "- class TestClass:",
        "| description = \"A class in folder\"",
        "- constructor default:",
        "| description = \"Default constructor for TestClass\"",
        "_",
        "- method doSomething:",
        "| return = void",
        "| parameters = ",
        "| description = \"A method in TestClass\"",
        "_",
        "_" , // End class block.
        "- namespace NestedNS:",
        "| description = \"A nested namespace in folder\"",
        "- function nestedFunc:",
        "| return = void",
        "| parameters = ",
        "| description = \"Nested function\"",
        "_",
        "_" , // End nested namespace block.
        "_"  // End folder block.
    };

    FolderModel folder = parseFolderBlock("MixedFolder", lines);
    EXPECT_EQ(folder.name, "MixedFolder");
    // One free function.
    ASSERT_EQ(folder.functionFile.size(), 1);
    EXPECT_EQ(folder.functionFile[0].name, "freeFunc1");
    // One class.
    ASSERT_EQ(folder.classFiles.size(), 1);
    EXPECT_EQ(folder.classFiles[0].name, "TestClass");
    // One nested namespace.
    ASSERT_EQ(folder.namespaceFiles.size(), 1);
    EXPECT_EQ(folder.namespaceFiles[0].name, "NestedNS");
}
