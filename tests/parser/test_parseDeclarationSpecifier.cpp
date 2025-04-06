#include "gtest/gtest.h"
#include "PropertiesParser.h"      // Declares parseDeclarationSpecifier
#include "PropertiesModels.h"    // Contains DeclartionSpecifier definition
#include "ParserUtilities.h"       // For trimming if needed
#include <string_view>

using namespace PropertiesModels;
using namespace PropertiesParser;

// Test: When given an empty string, no declaration specifiers should be set.
TEST(ParseDeclarationSpecifierTest, EmptyString) {
    std::string_view input = "";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_FALSE(decl.isStatic);
    EXPECT_FALSE(decl.isInline);
    EXPECT_FALSE(decl.isConstexpr);
}

// Test: When input is "static", only the static specifier should be set.
TEST(ParseDeclarationSpecifierTest, OnlyStatic) {
    std::string_view input = "static";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_TRUE(decl.isStatic);
    EXPECT_FALSE(decl.isInline);
    EXPECT_FALSE(decl.isConstexpr);
}

// Test: When input is "inline", only the inline specifier should be set.
TEST(ParseDeclarationSpecifierTest, OnlyInline) {
    std::string_view input = "inline";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_FALSE(decl.isStatic);
    EXPECT_TRUE(decl.isInline);
    EXPECT_FALSE(decl.isConstexpr);
}

// Test: When input is "constexpr", only the constexpr specifier should be set.
TEST(ParseDeclarationSpecifierTest, OnlyConstexpr) {
    std::string_view input = "constexpr";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_FALSE(decl.isStatic);
    EXPECT_FALSE(decl.isInline);
    EXPECT_TRUE(decl.isConstexpr);
}

// Test: When input is "static inline", both static and inline specifiers should be set.
TEST(ParseDeclarationSpecifierTest, StaticInline) {
    std::string_view input = "static inline";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_TRUE(decl.isStatic);
    EXPECT_TRUE(decl.isInline);
    EXPECT_FALSE(decl.isConstexpr);
}

// Test: When input is "inline constexpr static", all specifiers should be set, regardless of order.
TEST(ParseDeclarationSpecifierTest, InlineConstexprStatic) {
    std::string_view input = "inline constexpr static";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_TRUE(decl.isStatic);
    EXPECT_TRUE(decl.isInline);
    EXPECT_TRUE(decl.isConstexpr);
}

// Test: Mixed whitespace should be correctly trimmed and parsed.
TEST(ParseDeclarationSpecifierTest, MixedWhitespace) {
    std::string_view input = "  static   constexpr    inline   ";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_TRUE(decl.isStatic);
    EXPECT_TRUE(decl.isInline);
    EXPECT_TRUE(decl.isConstexpr);
}
