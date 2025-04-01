#include "gtest/gtest.h"
#include "PropertiesParser.h"      // Declares parseDeclarationSpecifier
#include "ScaffoldProperties.h"    // Contains DeclartionSpecifier definition
#include "ParserUtilities.h"       // For trimming if needed
#include <string_view>

using namespace ScaffoldProperties;
using namespace PropertiesParser;

TEST(ParseDeclarationSpecifierTest, EmptyString) {
    std::string_view input = "";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_FALSE(decl.isStatic);
    EXPECT_FALSE(decl.isInline);
    EXPECT_FALSE(decl.isConstexpr);
}

TEST(ParseDeclarationSpecifierTest, OnlyStatic) {
    std::string_view input = "static";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_TRUE(decl.isStatic);
    EXPECT_FALSE(decl.isInline);
    EXPECT_FALSE(decl.isConstexpr);
}

TEST(ParseDeclarationSpecifierTest, OnlyInline) {
    std::string_view input = "inline";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_FALSE(decl.isStatic);
    EXPECT_TRUE(decl.isInline);
    EXPECT_FALSE(decl.isConstexpr);
}

TEST(ParseDeclarationSpecifierTest, OnlyConstexpr) {
    std::string_view input = "constexpr";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_FALSE(decl.isStatic);
    EXPECT_FALSE(decl.isInline);
    EXPECT_TRUE(decl.isConstexpr);
}

TEST(ParseDeclarationSpecifierTest, StaticInline) {
    std::string_view input = "static inline";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_TRUE(decl.isStatic);
    EXPECT_TRUE(decl.isInline);
    EXPECT_FALSE(decl.isConstexpr);
}

TEST(ParseDeclarationSpecifierTest, InlineConstexprStatic) {
    std::string_view input = "inline constexpr static";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_TRUE(decl.isStatic);
    EXPECT_TRUE(decl.isInline);
    EXPECT_TRUE(decl.isConstexpr);
}

TEST(ParseDeclarationSpecifierTest, MixedWhitespace) {
    std::string_view input = "  static   constexpr    inline   ";
    DeclartionSpecifier decl = parseDeclarationSpecifier(input);
    EXPECT_TRUE(decl.isStatic);
    EXPECT_TRUE(decl.isInline);
    EXPECT_TRUE(decl.isConstexpr);
}
