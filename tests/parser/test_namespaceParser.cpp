#include <gtest/gtest.h>
#include "NamespaceParser.h"
#include "CodeGroupModels.h" 
#include <string_view>
#include <optional>
#include <vector>
#include <deque>

using namespace NamespaceParser;
using namespace CodeGroupModels;

TEST(NamespaceParserTest, ParsesBasicNamespaceWithDescription) {
    std::deque<std::string_view> lines = {
        "| description = \"A basic namespace\"",
        "_"
    };

    NamespaceModel ns = parseNamespaceBlock(std::make_optional("TestNamespace"), lines);

    EXPECT_EQ(ns.name, "TestNamespace");
    EXPECT_EQ(ns.description, "A basic namespace");
    EXPECT_TRUE(ns.classes.empty());
    EXPECT_TRUE(ns.functions.empty());
    EXPECT_TRUE(ns.namespaces.empty());
}

TEST(NamespaceParserTest, ParsesAnonymousNamespace) {
    std::deque<std::string_view> lines = {
        "| description = \"Anonymous namespace\"",
        "_"
    };

    NamespaceModel ns = parseNamespaceBlock(std::nullopt, lines);

    // Anonymous namespaces yield an empty name.
    EXPECT_EQ(ns.name, "");
    EXPECT_EQ(ns.description, "Anonymous namespace");
}

TEST(NamespaceParserTest, ParsesNamespaceWithNestedClassAndFunction) {
    std::deque<std::string_view> lines = {
        "| description = \"Namespace with nested class and function\"",
        "- class MyClass:",
        "| description = \"A class in the namespace\"",
        "_",
        "- function freeFunc:",
        "| declaration = inline",
        "| return = int",
        "| parameters = param1:int",
        "| description = \"A free function\"",
        "_",
        "_" // End namespace block.
    };

    NamespaceModel ns = parseNamespaceBlock(std::make_optional("MyNamespace"), lines);

    EXPECT_EQ(ns.name, "MyNamespace");
    EXPECT_EQ(ns.description, "Namespace with nested class and function");
    ASSERT_EQ(ns.classes.size(), 1);
    EXPECT_EQ(ns.classes[0].name, "MyClass");
    ASSERT_EQ(ns.functions.size(), 1);
    EXPECT_EQ(ns.functions[0].name, "freeFunc");
}

TEST(NamespaceParserTest, ParsesNestedNamespace) {
    std::deque<std::string_view> lines = {
        "| description = \"Outer namespace\"",
        "- namespace InnerNamespace:",
        "| description = \"Inner namespace description\"",
        "- function innerFunc:",
        "| return = void",
        "| parameters = ",
        "| description = \"Inner function\"",
        "_",
        "_" , // End inner namespace block.
        "_"  // End outer namespace block.
    };

    NamespaceModel ns = parseNamespaceBlock(std::make_optional("OuterNamespace"), lines);

    EXPECT_EQ(ns.name, "OuterNamespace");
    EXPECT_EQ(ns.description, "Outer namespace");
    ASSERT_EQ(ns.namespaces.size(), 1);
    NamespaceModel innerNS = ns.namespaces[0];
    EXPECT_EQ(innerNS.name, "InnerNamespace");
    EXPECT_EQ(innerNS.description, "Inner namespace description");
    ASSERT_EQ(innerNS.functions.size(), 1);
    EXPECT_EQ(innerNS.functions[0].name, "innerFunc");
}

TEST(NamespaceParserTest, ThrowsErrorOnMethodInNamespace) {
    std::deque<std::string_view> lines = {
        "| description = \"Namespace with invalid method\"",
        "- method invalidMethod:",
        "| return = void",
        "| parameters = ",
        "| description = \"This method should not be allowed\"",
        "_",
        "_" // End namespace block.
    };

    EXPECT_THROW({
        NamespaceModel ns = parseNamespaceBlock(std::make_optional("BadNamespace"), lines);
    }, std::runtime_error);
}

TEST(NamespaceParserTest, ThrowsOnUnknownNamespaceProperty) {
    std::deque<std::string_view> lines = {
        "| nonsense = true",
        "_"
    };

    EXPECT_THROW({
        NamespaceModel ns = parseNamespaceBlock(std::make_optional("TestNamespace"), lines);
    }, std::runtime_error);
}

TEST(NamespaceParserTest, ParsesMixedNestedContent) {
    std::deque<std::string_view> lines = {
        "| description = \"Mixed content namespace\"",
        "- function freeFunc1:",
        "| declaration = inline",
        "| return = int",
        "| parameters = param1:int",
        "| description = \"First free function\"",
        "_",
        "- class TestClass:",
        "| description = \"Nested class\"",
        "- constructor default:",
        "| description = \"Default constructor for TestClass\"",
        "_",
        "- method doSomething:",
        "| return = void",
        "| parameters = ",
        "| description = \"A method in TestClass\"",
        "_",
        "_", // End class block.
        "- namespace NestedNS:",
        "| description = \"A nested namespace\"",
        "- function nestedFunc:",
        "| return = void",
        "| parameters = ",
        "| description = \"Nested namespace function\"",
        "_",
        "_" , // End nested namespace block.
        "_"  // End outer namespace block.
    };

    NamespaceModel ns = parseNamespaceBlock(std::make_optional("MixedNS"), lines);
    EXPECT_EQ(ns.name, "MixedNS");
    EXPECT_EQ(ns.description, "Mixed content namespace");
    ASSERT_EQ(ns.functions.size(), 1);
    EXPECT_EQ(ns.functions[0].name, "freeFunc1");
    ASSERT_EQ(ns.classes.size(), 1);
    EXPECT_EQ(ns.classes[0].name, "TestClass");
    ASSERT_EQ(ns.namespaces.size(), 1);
    EXPECT_EQ(ns.namespaces[0].name, "NestedNS");
    ASSERT_EQ(ns.namespaces[0].functions.size(), 1);
    EXPECT_EQ(ns.namespaces[0].functions[0].name, "nestedFunc");
}

TEST(NamespaceParserTest, IgnoresTrailingGarbage) {
    std::deque<std::string_view> lines = {
        "| description = \"Namespace with trailing garbage\"",
        "_",
        "non DSL text",
        "- something irrelevant:"
    };

    NamespaceModel ns = parseNamespaceBlock(std::make_optional("GarbageNS"), lines);
    EXPECT_EQ(ns.description, "Namespace with trailing garbage");
}

TEST(NamespaceParserTest, ParsesEmptyNamespaceBlock) {
    std::deque<std::string_view> lines = { "_" };

    NamespaceModel ns = parseNamespaceBlock(std::make_optional("EmptyNS"), lines);
    EXPECT_EQ(ns.name, "EmptyNS");
    EXPECT_EQ(ns.description, "");
    EXPECT_TRUE(ns.classes.empty());
    EXPECT_TRUE(ns.functions.empty());
    EXPECT_TRUE(ns.namespaces.empty());
}
