#include "gtest/gtest.h"
#include "PropertiesGenerator.h"
#include "ScaffoldProperties.h"
#include <string>

using namespace ScaffoldProperties;

TEST(GenerateDeclarationSpecifierTest, EmptySpecifier) {
    DeclartionSpecifier ds{};
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "");
}

TEST(GenerateDeclarationSpecifierTest, OnlyStatic) {
    DeclartionSpecifier ds{};
    ds.isStatic = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "static ");
}

TEST(GenerateDeclarationSpecifierTest, OnlyInline) {
    DeclartionSpecifier ds{};
    ds.isInline = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "inline ");
}

TEST(GenerateDeclarationSpecifierTest, OnlyConstexpr) {
    DeclartionSpecifier ds{};
    ds.isConstexpr = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "constexpr ");
}

TEST(GenerateDeclarationSpecifierTest, StaticAndInline) {
    DeclartionSpecifier ds{};
    ds.isStatic = true;
    ds.isInline = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "static inline ");
}

TEST(GenerateDeclarationSpecifierTest, StaticInlineConstexpr) {
    DeclartionSpecifier ds{};
    ds.isStatic = true;
    ds.isInline = true;
    ds.isConstexpr = true;
    std::string result = PropertiesGenerator::generateDeclarationSpecifier(ds);
    EXPECT_EQ(result, "static inline constexpr ");
}
