#include "gtest/gtest.h"
#include "PropertiesParser.h"      // Declares parseDataType
#include "ScaffoldProperties.h"    // Contains the Types enum, DataType struct, and TypeQualifier definitions
#include <string_view>

using namespace PropertiesParser;

TEST(ParseDataTypeTest, RecognizesVoid) {
    auto dt = parseDataType("void");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::VOID);
    EXPECT_FALSE(dt.customType.has_value());
}

TEST(ParseDataTypeTest, RecognizesInt) {
    auto dt = parseDataType("int");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_FALSE(dt.customType.has_value());
}

TEST(ParseDataTypeTest, RecognizesUint) {
    auto dt = parseDataType("uint");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::UINT);
    EXPECT_FALSE(dt.customType.has_value());
}

TEST(ParseDataTypeTest, RecognizesLong) {
    auto dt = parseDataType("long");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::LONG);
}

TEST(ParseDataTypeTest, RecognizesULong) {
    auto dt = parseDataType("ulong");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::ULONG);
}

TEST(ParseDataTypeTest, RecognizesLongLong) {
    auto dt = parseDataType("longlong");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::LONGLONG);
}

TEST(ParseDataTypeTest, RecognizesULongLong) {
    auto dt = parseDataType("ulonglong");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::ULONGLONG);
}

TEST(ParseDataTypeTest, RecognizesFloat) {
    auto dt = parseDataType("float");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::FLOAT);
}

TEST(ParseDataTypeTest, RecognizesDouble) {
    auto dt = parseDataType("double");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::DOUBLE);
}

TEST(ParseDataTypeTest, RecognizesBool) {
    auto dt = parseDataType("bool");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::BOOL);
}

TEST(ParseDataTypeTest, RecognizesString) {
    auto dt = parseDataType("string");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::STRING);
}

TEST(ParseDataTypeTest, RecognizesChar) {
    auto dt = parseDataType("char");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::CHAR);
}

TEST(ParseDataTypeTest, RecognizesAuto) {
    auto dt = parseDataType("auto");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::AUTO);
}

TEST(ParseDataTypeTest, HandlesWhitespace) {
    // Extra spaces should be trimmed.
    auto dt = parseDataType("   int   ");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
}

TEST(ParseDataTypeTest, CustomTypeReturnsCustom) {
    // Unrecognized type should be treated as a custom type.
    auto dt = parseDataType("MyCustomType");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::CUSTOM);
    ASSERT_TRUE(dt.customType.has_value());
    EXPECT_EQ(*dt.customType, "MyCustomType");
}

TEST(ParseDataTypeTest, EmptyInputReturnsCustomEmpty) {
    // An empty string results in a custom type with an empty customType value.
    auto dt = parseDataType("");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::CUSTOM);
    ASSERT_TRUE(dt.customType.has_value());
    EXPECT_EQ(*dt.customType, "");
}

// ----- New Tests for Type Qualifiers -----

TEST(ParseDataTypeTest, RecognizesConstQualifier) {
    auto dt = parseDataType("const int");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::CONST));
    EXPECT_FALSE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
}

TEST(ParseDataTypeTest, RecognizesVolatileQualifier) {
    auto dt = parseDataType("volatile float");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::FLOAT);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
    EXPECT_FALSE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::CONST));
}

TEST(ParseDataTypeTest, RecognizesConstVolatileQualifier) {
    auto dt = parseDataType("const volatile double");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::DOUBLE);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::CONST));
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
}

TEST(ParseDataTypeTest, RecognizesQualifiersWithWhitespace) {
    auto dt = parseDataType("   volatile   int   ");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
}

// Test: Recognizes a single pointer.
TEST(ParseDataTypeTest, RecognizesPointer) {
    auto dt = parseDataType("int*");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(dt.typeDecl.ptrCount, 1);
    EXPECT_FALSE(dt.typeDecl.isLValReference);
    EXPECT_FALSE(dt.typeDecl.isRValReference);
    EXPECT_TRUE(dt.typeDecl.arrayDimensions.empty());
}

// Test: Recognizes multiple pointers.
TEST(ParseDataTypeTest, RecognizesMultiplePointers) {
    auto dt = parseDataType("int***");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(dt.typeDecl.ptrCount, 3);
    EXPECT_FALSE(dt.typeDecl.isLValReference);
    EXPECT_FALSE(dt.typeDecl.isRValReference);
    EXPECT_TRUE(dt.typeDecl.arrayDimensions.empty());
}

// Test: Recognizes an lvalue reference.
TEST(ParseDataTypeTest, RecognizesLValueReference) {
    auto dt = parseDataType("int&");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(dt.typeDecl.isLValReference);
    EXPECT_FALSE(dt.typeDecl.isRValReference);
    EXPECT_EQ(dt.typeDecl.ptrCount, 0);
}

// Test: Recognizes an rvalue reference.
TEST(ParseDataTypeTest, RecognizesRValueReference) {
    auto dt = parseDataType("int&&");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(dt.typeDecl.isRValReference);
    EXPECT_FALSE(dt.typeDecl.isLValReference);
    EXPECT_EQ(dt.typeDecl.ptrCount, 0);
}

// Test: Recognizes combination of pointer and lvalue reference.
TEST(ParseDataTypeTest, RecognizesPointerAndLValueReference) {
    auto dt = parseDataType("int*&");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(dt.typeDecl.ptrCount, 1);
    EXPECT_TRUE(dt.typeDecl.isLValReference);
    EXPECT_FALSE(dt.typeDecl.isRValReference);
}

// Test: Recognizes an array dimension.
TEST(ParseDataTypeTest, RecognizesArrayDimension) {
    auto dt = parseDataType("int[10]");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    ASSERT_EQ(dt.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(dt.typeDecl.arrayDimensions[0], "10");
}

// Test: Recognizes a pointer with an array dimension.
TEST(ParseDataTypeTest, RecognizesPointerAndArray) {
    auto dt = parseDataType("int*[5]");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(dt.typeDecl.ptrCount, 1);
    ASSERT_EQ(dt.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(dt.typeDecl.arrayDimensions[0], "5");
}
