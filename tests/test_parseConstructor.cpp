#include "SpecialMemberFunctionParser.h"
#include "ScaffoldProperties.h"
#include "ScaffoldModels.h"
#include <gtest/gtest.h>

using namespace SpecialMemberFunctionParser;
using namespace ScaffoldModels;
using namespace ScaffoldProperties;

TEST(ConstructorParserTest, ParsesDefaultConstructorWithDescription) {
    auto ctor = parseConstructorProperties("default", { "| description = \"A default ctor\"" });
    EXPECT_EQ(ctor.type, ConstructorType::DEFAULT);
    EXPECT_EQ(ctor.description, "A default ctor");
    EXPECT_TRUE(ctor.parameters.empty());
}

TEST(ConstructorParserTest, ParsesCopyConstructorNoParameters) {
    auto ctor = parseConstructorProperties("copy", {});
    EXPECT_EQ(ctor.type, ConstructorType::COPY);
    EXPECT_TRUE(ctor.parameters.empty());
}

TEST(ConstructorParserTest, ParsesMoveConstructorNoParameters) {
    auto ctor = parseConstructorProperties("move", {});
    EXPECT_EQ(ctor.type, ConstructorType::MOVE);
    EXPECT_TRUE(ctor.parameters.empty());
}

TEST(ConstructorParserTest, ThrowsIfCopyConstructorHasParameters) {
    EXPECT_THROW({
        parseConstructorProperties("copy", { "| parameters = x:int" });
    }, std::runtime_error);
}

TEST(ConstructorParserTest, ThrowsIfMoveConstructorHasParameters) {
    EXPECT_THROW({
        parseConstructorProperties("move", { "| parameters = y:string" });
    }, std::runtime_error);
}

TEST(ConstructorParserTest, ParsesCustomConstructorWithParametersAndDescription) {
    std::vector<std::string_view> lines = {
        "| parameters = x:int, name:string",
        "| description = \"Constructs with id and name\""
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
    EXPECT_THROW({
        parseConstructorProperties("nonsense", {});
    }, std::runtime_error);
}

TEST(ConstructorParserTest, ThrowsOnUnknownProperty) {
    EXPECT_THROW({
        parseConstructorProperties("custom", { "| madeup = nonsense" });
    }, std::runtime_error);
}
