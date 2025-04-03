#include "SpecialMemberFunctionParser.h"
#include "ScaffoldProperties.h"
#include "ScaffoldModels.h"
#include <gtest/gtest.h>
#include <deque>
#include <string_view>

using namespace SpecialMemberFunctionParser;
using namespace ScaffoldModels;
using namespace ScaffoldProperties;

TEST(ConstructorParserTest, ParsesDefaultConstructorWithDescription) {
    std::deque<std::string_view> lines = {
        "| description = \"A default ctor\"",
        "_"  // end of block marker
    };
    auto ctor = parseConstructorProperties("default", lines);
    EXPECT_EQ(ctor.type, ConstructorType::DEFAULT);
    EXPECT_EQ(ctor.description, "A default ctor");
    EXPECT_TRUE(ctor.parameters.empty());
}

TEST(ConstructorParserTest, ParsesCopyConstructorNoParameters) {
    std::deque<std::string_view> lines;  // no properties provided
    auto ctor = parseConstructorProperties("copy", lines);
    EXPECT_EQ(ctor.type, ConstructorType::COPY);
    EXPECT_TRUE(ctor.parameters.empty());
}

TEST(ConstructorParserTest, ParsesMoveConstructorNoParameters) {
    std::deque<std::string_view> lines;  // no properties provided
    auto ctor = parseConstructorProperties("move", lines);
    EXPECT_EQ(ctor.type, ConstructorType::MOVE);
    EXPECT_TRUE(ctor.parameters.empty());
}

TEST(ConstructorParserTest, ThrowsIfCopyConstructorHasParameters) {
    std::deque<std::string_view> lines = {
        "| parameters = x:int",
        "_"  // end of block marker
    };
    EXPECT_THROW({
        parseConstructorProperties("copy", lines);
    }, std::runtime_error);
}

TEST(ConstructorParserTest, ThrowsIfMoveConstructorHasParameters) {
    std::deque<std::string_view> lines = {
        "| parameters = y:string",
        "_"  // end of block marker
    };
    EXPECT_THROW({
        parseConstructorProperties("move", lines);
    }, std::runtime_error);
}

TEST(ConstructorParserTest, ParsesCustomConstructorWithParametersAndDescription) {
    std::deque<std::string_view> lines = {
        "| parameters = x:int, name:string",
        "| description = \"Constructs with id and name\"",
        "_"  // end of block marker
    };

    auto ctor = parseConstructorProperties("custom", lines);
    EXPECT_EQ(ctor.type, ConstructorType::CUSTOM);
    ASSERT_EQ(ctor.parameters.size(), 2);
    EXPECT_EQ(ctor.parameters[0].name, "x");
    EXPECT_EQ(ctor.parameters[0].type.type, Types::INT);
    EXPECT_EQ(ctor.parameters[1].name, "name");
    EXPECT_EQ(ctor.parameters[1].type.type, Types::STRING);
    EXPECT_EQ(ctor.description, "Constructs with id and name");
}

TEST(ConstructorParserTest, ThrowsOnUnknownConstructorType) {
    std::deque<std::string_view> lines;  // empty deque
    EXPECT_THROW({
        parseConstructorProperties("nonsense", lines);
    }, std::runtime_error);
}

TEST(ConstructorParserTest, ThrowsOnUnknownProperty) {
    std::deque<std::string_view> lines = {
        "| madeup = nonsense",
        "_"  // end of block marker
    };
    EXPECT_THROW({
        parseConstructorProperties("custom", lines);
    }, std::runtime_error);
}
