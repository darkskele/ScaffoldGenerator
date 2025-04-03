#include <gtest/gtest.h>
#include "LibraryParser.h"
#include "ScaffoldModels.h"  // Provides LibraryModel (which inherits from FolderModel) and related models.
#include <string_view>
#include <deque>

using namespace LibraryParser;
using namespace ScaffoldModels;

TEST(LibraryParserTest, ParsesEmptyLibraryBlock) {
    // A library block with only properties and no nested content.
    // DSL: library header with version and dependency lines, then the end marker.
    std::deque<std::string_view> lines = {
        "| version = 1.0.0",
        "| dependency = Boost, cpp20",
        "_"  // End of library block.
    };
    
    LibraryModel lib = parseLibraryBlock("EmptyLib", lines);
    
    EXPECT_EQ(lib.name, "EmptyLib");
    EXPECT_EQ(lib.version, "1.0.0");
    ASSERT_EQ(lib.dependencies.size(), 2);
    EXPECT_EQ(lib.dependencies[0], "Boost");
    EXPECT_EQ(lib.dependencies[1], "cpp20");
    EXPECT_TRUE(lib.subFolders.empty());
    EXPECT_TRUE(lib.classFiles.empty());
    EXPECT_TRUE(lib.namespaceFiles.empty());
    EXPECT_TRUE(lib.functionFiles.empty());
}

TEST(LibraryParserTest, ParsesLibraryWithNestedClass) {
    // A library block containing a nested class block.
    std::deque<std::string_view> lines = {
        "| version = 2.1.0",
        "| dependency = cpp20",
        "- class MyClass:",
        "| description = \"A class in library\"",
        "_",
        "_"  // End library block.
    };
    
    LibraryModel lib = parseLibraryBlock("LibWithClass", lines);
    EXPECT_EQ(lib.name, "LibWithClass");
    EXPECT_EQ(lib.version, "2.1.0");
    ASSERT_EQ(lib.classFiles.size(), 1);
    EXPECT_EQ(lib.classFiles[0].name, "MyClass");
    EXPECT_TRUE(lib.subFolders.empty());
    EXPECT_TRUE(lib.namespaceFiles.empty());
    EXPECT_TRUE(lib.functionFiles.empty());
}

TEST(LibraryParserTest, ParsesLibraryWithNestedNamespace) {
    // A library block containing a nested namespace block.
    std::deque<std::string_view> lines = {
        "| version = 3.0.0",
        "| dependency = OpenMP",
        "- namespace MyNamespace:",
        "| description = \"A namespace in library\"",
        "_",
        "_"  // End library block.
    };
    
    LibraryModel lib = parseLibraryBlock("LibWithNS", lines);
    EXPECT_EQ(lib.name, "LibWithNS");
    EXPECT_EQ(lib.version, "3.0.0");
    ASSERT_EQ(lib.namespaceFiles.size(), 1);
    EXPECT_EQ(lib.namespaceFiles[0].name, "MyNamespace");
}

TEST(LibraryParserTest, ParsesLibraryWithFunctionGroup) {
    // A library block containing consecutive free function blocks.
    std::deque<std::string_view> lines = {
        "| version = 1.2.3",
        "| dependency = Boost",
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
        "_"  // End library block.
    };
    
    LibraryModel lib = parseLibraryBlock("LibWithFunctions", lines);
    EXPECT_EQ(lib.name, "LibWithFunctions");
    ASSERT_EQ(lib.functionFiles.size(), 2);
    EXPECT_EQ(lib.functionFiles[0].name, "funcOne");
    EXPECT_EQ(lib.functionFiles[1].name, "funcTwo");
}

TEST(LibraryParserTest, ParsesLibraryWithNestedFolder) {
    // A library block containing a nested folder block.
    std::deque<std::string_view> lines = {
        "| version = 0.9.0",
        "| dependency = cpp20, OpenMP",
        "- folder SubFolder:",
        "- class SubClass:",
        "| description = \"Class in subfolder\"",
        "_",
        "_" , // End nested folder block.
        "_"  // End library block.
    };
    
    LibraryModel lib = parseLibraryBlock("LibWithFolder", lines);
    EXPECT_EQ(lib.name, "LibWithFolder");
    ASSERT_EQ(lib.subFolders.size(), 1);
    FolderModel subFolder = lib.subFolders[0];
    EXPECT_EQ(subFolder.name, "SubFolder");
    ASSERT_EQ(subFolder.classFiles.size(), 1);
    EXPECT_EQ(subFolder.classFiles[0].name, "SubClass");
}

TEST(LibraryParserTest, ThrowsErrorOnMethodInLibrary) {
    // A library block that attempts to declare a method, which is not allowed.
    std::deque<std::string_view> lines = {
        "| version = 1.0.0",
        "| dependency = Boost",
        "- method invalidMethod:",
        "| return = void",
        "| parameters = ",
        "| description = \"Invalid method in library\"",
        "_",
        "_"  // End library block.
    };
    
    EXPECT_THROW({
        LibraryModel lib = parseLibraryBlock("BadLib", lines);
    }, std::runtime_error);
}

TEST(LibraryParserTest, ThrowsOnPropertyInLibraryAfterPropertySection) {
    // After the initial property section, property lines are not allowed.
    std::deque<std::string_view> lines = {
        "| version = 1.0.0",
        "| dependency = cpp20",
        "- class SomeClass:",
        "| description = \"A class in library\"",
        "| version = 2.0",  // Invalid property inside nested block.
        "_",
        "_"  // End library block.
    };
    
    EXPECT_THROW({
        LibraryModel lib = parseLibraryBlock("LibBadProperty", lines);
    }, std::runtime_error);
}

TEST(LibraryParserTest, IgnoresTrailingGarbage) {
    // Trailing non-DSL text should be ignored.
    std::deque<std::string_view> lines = {
        "| version = 1.5.0",
        "| dependency = cpp20",
        "- class TrailingClass:",
        "| description = \"A class in library\"",
        "_",
        "_" , // End library block.
        "non DSL text",
        "- something irrelevant:"
    };
    
    LibraryModel lib = parseLibraryBlock("LibTrailing", lines);
    ASSERT_EQ(lib.classFiles.size(), 1);
    EXPECT_EQ(lib.classFiles[0].name, "TrailingClass");
}

TEST(LibraryParserTest, ParsesMixedNestedContentInLibrary) {
    // A library block with mixed nested content: free functions, a class, and a nested namespace.
    std::deque<std::string_view> lines = {
        "| version = 2.0.0",
        "| dependency = Boost, cpp20",
        "- function freeFunc1:",
        "| declaration = inline",
        "| return = int",
        "| parameters = param1:int",
        "| description = \"First free function\"",
        "_",
        "- class TestClass:",
        "| description = \"A class in library\"",
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
        "| description = \"A nested namespace in library\"",
        "- function nestedFunc:",
        "| return = void",
        "| parameters = ",
        "| description = \"Nested function\"",
        "_",
        "_" , // End nested namespace block.
        "_"  // End library block.
    };
    
    LibraryModel lib = parseLibraryBlock("MixedLib", lines);
    EXPECT_EQ(lib.name, "MixedLib");
    // One free function.
    ASSERT_EQ(lib.functionFiles.size(), 1);
    EXPECT_EQ(lib.functionFiles[0].name, "freeFunc1");
    // One class.
    ASSERT_EQ(lib.classFiles.size(), 1);
    EXPECT_EQ(lib.classFiles[0].name, "TestClass");
    // One nested namespace.
    ASSERT_EQ(lib.namespaceFiles.size(), 1);
    EXPECT_EQ(lib.namespaceFiles[0].name, "NestedNS");
}

TEST(LibraryParserTest, ThrowsErrorOnNestedLibrary) {
    // A library block that attempts to nest another library should throw an error.
    std::deque<std::string_view> lines = {
        "| version = 1.0.0",
        "| dependency = Boost",
        "- library NestedLib:",
        "| version = 0.1.0",
        "| dependency = cpp20",
        "_",
        "_"  // End library block.
    };
    
    EXPECT_THROW({
        LibraryModel lib = parseLibraryBlock("LibWithNestedLib", lines);
    }, std::runtime_error);
}
