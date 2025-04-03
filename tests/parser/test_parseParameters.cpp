#include "gtest/gtest.h"
#include "PropertiesParser.h"      // Declares parseParameters
#include "ScaffoldProperties.h"    // Contains the Parameter struct and Types enum
#include <stdexcept>
#include <vector>
#include <string_view>

using namespace PropertiesParser;

// Test: Parses two parameters separated by a comma.
TEST(ParseParametersTest, ParsesMultipleParameters) {
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

// Test: Extra spaces around tokens should be trimmed.
TEST(ParseParametersTest, HandlesWhitespace) {
    std::string_view input = "   param1:int  ,   param2:float   ";
    auto params = parseParameters(input);
    EXPECT_EQ(params.size(), 2);

    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);

    EXPECT_EQ(params[1].name, "param2");
    EXPECT_EQ(params[1].type.type, ScaffoldProperties::Types::FLOAT);
}

// Test: A single parameter is parsed correctly.
TEST(ParseParametersTest, SingleParameter) {
    std::string_view input = "param1:bool";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::BOOL);
}

// Test: Empty input returns an empty parameter vector.
TEST(ParseParametersTest, EmptyInputReturnsEmptyVector) {
    std::string_view input = "";
    auto params = parseParameters(input);
    EXPECT_TRUE(params.empty());
}

// Test: Malformed parameter (missing colon) should throw an exception.
TEST(ParseParametersTest, MalformedParameterThrows) {
    std::string_view input = "param1int";
    EXPECT_THROW({
        parseParameters(input);
    }, std::runtime_error);
}

// Test: Trailing comma should not produce an extra parameter.
TEST(ParseParametersTest, TrailingCommaHandledGracefully) {
    std::string_view input = "param1:int, param2:float,";
    auto params = parseParameters(input);
    EXPECT_EQ(params.size(), 2);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[1].name, "param2");
}

// Test: Recognizes a parameter with a const qualifier.
TEST(ParseParametersTest, RecognizesConstParameter) {
    std::string_view input = "param1: const int";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(params[0].type.qualifiers, ScaffoldProperties::TypeQualifier::CONST));
    EXPECT_FALSE(ScaffoldProperties::hasQualifier(params[0].type.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
}

// Test: Recognizes a parameter with a volatile qualifier.
TEST(ParseParametersTest, RecognizesVolatileParameter) {
    std::string_view input = "param1: volatile float";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::FLOAT);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(params[0].type.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
    EXPECT_FALSE(ScaffoldProperties::hasQualifier(params[0].type.qualifiers, ScaffoldProperties::TypeQualifier::CONST));
}

// Test: Recognizes a parameter with both const and volatile qualifiers.
TEST(ParseParametersTest, RecognizesConstVolatileParameter) {
    std::string_view input = "param1: const volatile double";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::DOUBLE);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(params[0].type.qualifiers, ScaffoldProperties::TypeQualifier::CONST));
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(params[0].type.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
}

// Test: Recognizes a parameter with a single pointer.
TEST(ParseParametersTest, RecognizesPointerParameter) {
    std::string_view input = "param1: int*";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(params[0].type.typeDecl.ptrCount, 1);
    EXPECT_FALSE(params[0].type.typeDecl.isLValReference);
    EXPECT_FALSE(params[0].type.typeDecl.isRValReference);
    EXPECT_TRUE(params[0].type.typeDecl.arrayDimensions.empty());
}

// Test: Recognizes a parameter with multiple pointers.
TEST(ParseParametersTest, RecognizesMultiplePointersParameter) {
    std::string_view input = "param1: int***";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(params[0].type.typeDecl.ptrCount, 3);
    EXPECT_FALSE(params[0].type.typeDecl.isLValReference);
    EXPECT_FALSE(params[0].type.typeDecl.isRValReference);
    EXPECT_TRUE(params[0].type.typeDecl.arrayDimensions.empty());
}

// Test: Recognizes a parameter with an lvalue reference.
TEST(ParseParametersTest, RecognizesLValueReferenceParameter) {
    std::string_view input = "param1: int&";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(params[0].type.typeDecl.isLValReference);
    EXPECT_FALSE(params[0].type.typeDecl.isRValReference);
    EXPECT_EQ(params[0].type.typeDecl.ptrCount, 0);
    EXPECT_TRUE(params[0].type.typeDecl.arrayDimensions.empty());
}

// Test: Recognizes a parameter with an rvalue reference.
TEST(ParseParametersTest, RecognizesRValueReferenceParameter) {
    std::string_view input = "param1: int&&";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(params[0].type.typeDecl.isRValReference);
    EXPECT_FALSE(params[0].type.typeDecl.isLValReference);
    EXPECT_EQ(params[0].type.typeDecl.ptrCount, 0);
    EXPECT_TRUE(params[0].type.typeDecl.arrayDimensions.empty());
}

// Test: Recognizes a parameter combining pointer and lvalue reference.
TEST(ParseParametersTest, RecognizesPointerAndLValueReferenceParameter) {
    std::string_view input = "param1: int*&";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(params[0].type.typeDecl.ptrCount, 1);
    EXPECT_TRUE(params[0].type.typeDecl.isLValReference);
    EXPECT_FALSE(params[0].type.typeDecl.isRValReference);
    EXPECT_TRUE(params[0].type.typeDecl.arrayDimensions.empty());
}

// Test: Recognizes a parameter with an array dimension.
TEST(ParseParametersTest, RecognizesArrayParameter) {
    std::string_view input = "param1: int[10]";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(params[0].type.typeDecl.ptrCount, 0);
    EXPECT_FALSE(params[0].type.typeDecl.isLValReference);
    EXPECT_FALSE(params[0].type.typeDecl.isRValReference);
    ASSERT_EQ(params[0].type.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(params[0].type.typeDecl.arrayDimensions[0], "10");
}

// Test: Recognizes a parameter combining pointer and array.
TEST(ParseParametersTest, RecognizesPointerAndArrayParameter) {
    std::string_view input = "param1: int*[5]";
    auto params = parseParameters(input);
    ASSERT_EQ(params.size(), 1);
    EXPECT_EQ(params[0].name, "param1");
    EXPECT_EQ(params[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(params[0].type.typeDecl.ptrCount, 1);
    EXPECT_FALSE(params[0].type.typeDecl.isLValReference);
    EXPECT_FALSE(params[0].type.typeDecl.isRValReference);
    ASSERT_EQ(params[0].type.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(params[0].type.typeDecl.arrayDimensions[0], "5");
}
