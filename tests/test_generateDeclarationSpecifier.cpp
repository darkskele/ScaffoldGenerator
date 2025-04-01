#include "gtest/gtest.h"
#include "PropertiesGenerator.h"
#include "ScaffoldProperties.h"
#include <string>

using namespace ScaffoldProperties;

// Test with no specifiers set; expected result is an empty string.
TEST(GenerateDeclarationSpecifierTest, EmptySpecifier) {
    DeclartionSpecifier ds{};
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "");
}

// Test with only the 'static' specifier set.
TEST(GenerateDeclarationSpecifierTest, OnlyStatic) {
    DeclartionSpecifier ds{};
    ds.isStatic = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "static ");
}

// Test with only the 'inline' specifier set.
TEST(GenerateDeclarationSpecifierTest, OnlyInline) {
    DeclartionSpecifier ds{};
    ds.isInline = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "inline ");
}

// Test with only the 'constexpr' specifier set.
TEST(GenerateDeclarationSpecifierTest, OnlyConstexpr) {
    DeclartionSpecifier ds{};
    ds.isConstexpr = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "constexpr ");
}

// Test with both 'static' and 'inline' specifiers.
TEST(GenerateDeclarationSpecifierTest, StaticAndInline) {
    DeclartionSpecifier ds{};
    ds.isStatic = true;
    ds.isInline = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "static inline ");
}

// Test with 'static', 'inline', and 'constexpr' specifiers.
TEST(GenerateDeclarationSpecifierTest, StaticInlineConstexpr) {
    DeclartionSpecifier ds{};
    ds.isStatic = true;
    ds.isInline = true;
    ds.isConstexpr = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "static inline constexpr ");
}

// Test with 'static' and 'constexpr' specifiers (without 'inline').
TEST(GenerateDeclarationSpecifierTest, StaticAndConstexpr) {
    DeclartionSpecifier ds{};
    ds.isStatic = true;
    ds.isConstexpr = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "static constexpr ");
}

// Test with 'inline' and 'constexpr' specifiers (without 'static').
TEST(GenerateDeclarationSpecifierTest, InlineAndConstexpr) {
    DeclartionSpecifier ds{};
    ds.isInline = true;
    ds.isConstexpr = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "inline constexpr ");
}
