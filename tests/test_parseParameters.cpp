#include "gtest/gtest.h"
#include "PropertiesParser.h"      // Declares parseParameters
#include "ScaffoldProperties.h"    // Contains the Parameter struct and Types enum
#include <stdexcept>
#include <vector>
#include <string_view>

using namespace PropertiesParser;

TEST(ParseParametersTest, ParsesMultipleParameters) {
    // Basic test with two parameters.
    std::string_view input = "param1:int, param2:float";
    auto params = parseParameters(input);
    EXPECT_EQ(params.size(), 2);

    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_FALSE(params[0].type.customType.has_value());

    EXPECT_EQ(params[1].name, "param2");
    EXPECT_EQ(params[1].type.type, ScaffoldProperties::Types::FLOAT);
    EXPECT_FALSE(params[1].type.customType.has_value());
}

TEST(ParseParametersTest, HandlesWhitespace) {
    // Extra spaces around tokens should be trimmed.
    std::string_view input = "   param1:int  ,   param2:float   ";
    auto params = parseParameters(input);
    EXPECT_EQ(params.size(), 2);

    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);

    EXPECT_EQ(params[1].name, "param2");
    EXPECT_EQ(params[1].type.type, ScaffoldProperties::Types::FLOAT);
}

TEST(ParseParametersTest, SingleParameter) {
    // Only one parameter should be parsed.
    std::string_view input = "param1:bool";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::BOOL);
}

TEST(ParseParametersTest, EmptyInputReturnsEmptyVector) {
    // Empty input should return an empty vector.
    std::string_view input = "";
    auto params = parseParameters(input);
    EXPECT_TRUE(params.empty());
}

TEST(ParseParametersTest, MalformedParameterThrows) {
    // Missing colon should throw an exception.
    std::string_view input = "param1int";
    EXPECT_THROW({
        parseParameters(input);
    }, std::runtime_error);
}

TEST(ParseParametersTest, TrailingCommaHandledGracefully) {
    // A trailing comma should not create an extra parameter.
    std::string_view input = "param1:int, param2:float,";
    auto params = parseParameters(input);
    EXPECT_EQ(params.size(), 2);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[1].name, "param2");
}
