#include <gtest/gtest.h>
#include "ProjectParser.h"
#include "LibraryParser.h"
#include "FolderParser.h"
#include "CodeGroupModels.h"  // Provides ProjectModel, LibraryModel, FolderModel, etc.
#include <string_view>
#include <deque>

using namespace ProjectParser;
using namespace CodeGroupModels;

TEST(ProjectParserTest, ParsesEmptyProjectBlock) {
    // A project block with only properties and no nested content.
    // DSL: project header with version and dependency lines, then the end marker.
    std::deque<std::string_view> lines = {
        "| version = 1.0.0",
        "| dependency = Boost, cpp23",
        "_"  // End of project block.
    };
    
    ProjectModel proj = parseProjectBlock("MyProject", lines);
    
    EXPECT_EQ(proj.name, "MyProject");
    EXPECT_EQ(proj.version, "1.0.0");
    ASSERT_EQ(proj.dependencies.size(), 2);
    EXPECT_EQ(proj.dependencies[0], "Boost");
    EXPECT_EQ(proj.dependencies[1], "cpp23");
    EXPECT_TRUE(proj.libraries.empty());
    EXPECT_TRUE(proj.subFolders.empty());
    EXPECT_TRUE(proj.classFiles.empty());
    EXPECT_TRUE(proj.namespaceFiles.empty());
    EXPECT_TRUE(proj.functionFile.empty());
}

TEST(ProjectParserTest, ParsesProjectWithNestedLibrary) {
    // A project block containing a nested library block.
    std::deque<std::string_view> lines = {
        "| version = 2.0.0",
        "| dependency = OpenMP",
        "- library LibInProject:",
        "| version = 1.2.3",
        "| dependency = cpp23",
        "_" , // End library block.
        "_"  // End project block.
    };
    
    ProjectModel proj = parseProjectBlock("ProjectWithLib", lines);
    EXPECT_EQ(proj.name, "ProjectWithLib");
    EXPECT_EQ(proj.version, "2.0.0");
    ASSERT_EQ(proj.libraries.size(), 1);
    EXPECT_EQ(proj.libraries[0].name, "LibInProject");
    EXPECT_EQ(proj.libraries[0].version, "1.2.3");
}

TEST(ProjectParserTest, ParsesMixedNestedContentInProject) {
    // A project block with mixed nested content: a nested library, folder, class, namespace, and free function.
    std::deque<std::string_view> lines = {
        "| version = 3.0.0",
        "| dependency = Boost, cpp23",
        "- library MyLibrary:",
        "| version = 1.0.0",
        "| dependency = cpp23",
        "_" , // End library block.
        "- folder core:",
        "- class CoreClass:",
        "| description = \"Core class in project\"",
        "_",
        "_" , // End folder block.
        "- namespace Utils:",
        "| description = \"Utility namespace\"",
        "- function helperFunc:",
        "| declaration = inline",
        "| return = int",
        "| parameters = value:int",
        "| description = \"Helper function\"",
        "_",
        "_" , // End namespace block.
        // Also declare a free function directly at the project level.
        "- function topLevelFunc:",
        "| declaration = static",
        "| return = void",
        "| parameters = ",
        "| description = \"A project-level free function\"",
        "_",
        "_"  // End project block.
    };
    
    ProjectModel proj = parseProjectBlock("MixedProject", lines);
    EXPECT_EQ(proj.name, "MixedProject");
    EXPECT_EQ(proj.version, "3.0.0");
    
    // The nested library is parsed correctly.
    ASSERT_EQ(proj.libraries.size(), 1);
    EXPECT_EQ(proj.libraries[0].name, "MyLibrary");
    
    // The nested folder is parsed correctly.
    ASSERT_EQ(proj.subFolders.size(), 1);
    EXPECT_EQ(proj.subFolders[0].name, "core");
    
    // The free function declared directly at the project level appears in project functionFile.
    ASSERT_EQ(proj.functionFile.size(), 1);
    EXPECT_EQ(proj.functionFile[0].name, "topLevelFunc");
    
    // The namespace block should have its own free function.
    ASSERT_EQ(proj.namespaceFiles.size(), 1);
    EXPECT_EQ(proj.namespaceFiles[0].name, "Utils");
    ASSERT_EQ(proj.namespaceFiles[0].functions.size(), 1);
    EXPECT_EQ(proj.namespaceFiles[0].functions[0].name, "helperFunc");
}


TEST(ProjectParserTest, ThrowsErrorOnMethodInProject) {
    // A project block that attempts to declare a method should throw an error.
    std::deque<std::string_view> lines = {
        "| version = 1.0.0",
        "| dependency = Boost",
        "- method invalidMethod:",
        "| return = void",
        "| parameters = ",
        "| description = \"Invalid method in project\"",
        "_",
        "_"  // End project block.
    };
    
    EXPECT_THROW({
        ProjectModel proj = parseProjectBlock("BadProject", lines);
    }, std::runtime_error);
}

TEST(ProjectParserTest, ThrowsOnPropertyAfterPropertySectionInProject) {
    // After the initial property section, property lines are not allowed.
    std::deque<std::string_view> lines = {
        "| version = 1.0.0",
        "| dependency = cpp23",
        "- class SomeClass:",
        "| description = \"A class in project\"",
        "| version = 2.0",  // Invalid property inside nested block.
        "_",
        "_"  // End project block.
    };
    
    EXPECT_THROW({
        ProjectModel proj = parseProjectBlock("ProjectBadProperty", lines);
    }, std::runtime_error);
}

TEST(ProjectParserTest, IgnoresTrailingGarbage) {
    // Trailing non-DSL text should be ignored.
    std::deque<std::string_view> lines = {
        "| version = 1.5.0",
        "| dependency = cpp23",
        "- class TrailingClass:",
        "| description = \"A class in project\"",
        "_",
        "_" , // End project block.
        "non DSL text",
        "- something irrelevant:"
    };
    
    ProjectModel proj = parseProjectBlock("ProjectTrailing", lines);
    ASSERT_EQ(proj.classFiles.size(), 1);
    EXPECT_EQ(proj.classFiles[0].name, "TrailingClass");
}

TEST(ProjectParserTest, ThrowsErrorOnNestedProjectKeyword) {
    // A project block that incorrectly contains another project keyword.
    std::deque<std::string_view> lines = {
        "| version = 1.0.0",
        "| dependency = Boost",
        "- project NestedProject:",  // This is not allowed.
        "| version = 2.0.0",
        "| dependency = cpp23",
        "_",
        "_"  // End project block.
    };
    
    EXPECT_THROW({
        ProjectModel proj = parseProjectBlock("MainProject", lines);
    }, std::runtime_error);
}