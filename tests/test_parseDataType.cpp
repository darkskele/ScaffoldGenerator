#include "gtest/gtest.h"
#include "PropertiesParser.h"      // Declares parseDataType and related functions.
#include "ScaffoldProperties.h"    // Contains the Types enum, DataType struct, and TypeQualifier definitions.
#include <string_view>

using namespace PropertiesParser;

// ----- Tests for Basic Data Types -----

// Test: "void" should be recognized as the VOID type.
TEST(ParseDataTypeTest, RecognizesVoid) {
    auto dt = parseDataType("void");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::VOID);
    EXPECT_FALSE(dt.customType.has_value());
}

// Test: "int" should be recognized as the INT type.
TEST(ParseDataTypeTest, RecognizesInt) {
    auto dt = parseDataType("int");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_FALSE(dt.customType.has_value());
}

// Test: "uint" should be recognized as the UINT type.
TEST(ParseDataTypeTest, RecognizesUint) {
    auto dt = parseDataType("uint");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::UINT);
    EXPECT_FALSE(dt.customType.has_value());
}

// Test: "long" should be recognized as the LONG type.
TEST(ParseDataTypeTest, RecognizesLong) {
    auto dt = parseDataType("long");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::LONG);
}

// Test: "ulong" should be recognized as the ULONG type.
TEST(ParseDataTypeTest, RecognizesULong) {
    auto dt = parseDataType("ulong");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::ULONG);
}

// Test: "longlong" should be recognized as the LONGLONG type.
TEST(ParseDataTypeTest, RecognizesLongLong) {
    auto dt = parseDataType("longlong");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::LONGLONG);
}

// Test: "ulonglong" should be recognized as the ULONGLONG type.
TEST(ParseDataTypeTest, RecognizesULongLong) {
    auto dt = parseDataType("ulonglong");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::ULONGLONG);
}

// Test: "float" should be recognized as the FLOAT type.
TEST(ParseDataTypeTest, RecognizesFloat) {
    auto dt = parseDataType("float");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::FLOAT);
}

// Test: "double" should be recognized as the DOUBLE type.
TEST(ParseDataTypeTest, RecognizesDouble) {
    auto dt = parseDataType("double");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::DOUBLE);
}

// Test: "bool" should be recognized as the BOOL type.
TEST(ParseDataTypeTest, RecognizesBool) {
    auto dt = parseDataType("bool");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::BOOL);
}

// Test: "string" should be recognized as the STRING type.
TEST(ParseDataTypeTest, RecognizesString) {
    auto dt = parseDataType("string");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::STRING);
}

// Test: "char" should be recognized as the CHAR type.
TEST(ParseDataTypeTest, RecognizesChar) {
    auto dt = parseDataType("char");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::CHAR);
}

// Test: "auto" should be recognized as the AUTO type.
TEST(ParseDataTypeTest, RecognizesAuto) {
    auto dt = parseDataType("auto");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::AUTO);
}

// Test: Extra whitespace should be trimmed.
TEST(ParseDataTypeTest, HandlesWhitespace) {
    auto dt = parseDataType("   int   ");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
}

// Test: Unrecognized type should be treated as a custom type.
TEST(ParseDataTypeTest, CustomTypeReturnsCustom) {
    auto dt = parseDataType("MyCustomType");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::CUSTOM);
    ASSERT_TRUE(dt.customType.has_value());
    EXPECT_EQ(*dt.customType, "MyCustomType");
}

// Test: An empty string returns a custom type with an empty customType value.
TEST(ParseDataTypeTest, EmptyInputReturnsCustomEmpty) {
    auto dt = parseDataType("");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::CUSTOM);
    ASSERT_TRUE(dt.customType.has_value());
    EXPECT_EQ(*dt.customType, "");
}

// ----- Tests for Type Qualifiers -----

// Test: "const int" should have the CONST qualifier.
TEST(ParseDataTypeTest, RecognizesConstQualifier) {
    auto dt = parseDataType("const int");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::CONST));
    EXPECT_FALSE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
}

// Test: "volatile float" should have the VOLATILE qualifier.
TEST(ParseDataTypeTest, RecognizesVolatileQualifier) {
    auto dt = parseDataType("volatile float");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::FLOAT);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
    EXPECT_FALSE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::CONST));
}

// Test: "const volatile double" should have both CONST and VOLATILE qualifiers.
TEST(ParseDataTypeTest, RecognizesConstVolatileQualifier) {
    auto dt = parseDataType("const volatile double");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::DOUBLE);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::CONST));
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
}

// Test: Qualifiers with extra whitespace should be recognized.
TEST(ParseDataTypeTest, RecognizesQualifiersWithWhitespace) {
    auto dt = parseDataType("   volatile   int   ");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(dt.qualifiers, ScaffoldProperties::TypeQualifier::VOLATILE));
}

// ----- Tests for Pointers, References, and Arrays -----

// Test: "int*" should be recognized with a single pointer.
TEST(ParseDataTypeTest, RecognizesPointer) {
    auto dt = parseDataType("int*");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(dt.typeDecl.ptrCount, 1);
    EXPECT_FALSE(dt.typeDecl.isLValReference);
    EXPECT_FALSE(dt.typeDecl.isRValReference);
    EXPECT_TRUE(dt.typeDecl.arrayDimensions.empty());
}

// Test: "int***" should be recognized with multiple pointers.
TEST(ParseDataTypeTest, RecognizesMultiplePointers) {
    auto dt = parseDataType("int***");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(dt.typeDecl.ptrCount, 3);
    EXPECT_FALSE(dt.typeDecl.isLValReference);
    EXPECT_FALSE(dt.typeDecl.isRValReference);
    EXPECT_TRUE(dt.typeDecl.arrayDimensions.empty());
}

// Test: "int&" should be recognized as an lvalue reference.
TEST(ParseDataTypeTest, RecognizesLValueReference) {
    auto dt = parseDataType("int&");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(dt.typeDecl.isLValReference);
    EXPECT_FALSE(dt.typeDecl.isRValReference);
    EXPECT_EQ(dt.typeDecl.ptrCount, 0);
}

// Test: "int&&" should be recognized as an rvalue reference.
TEST(ParseDataTypeTest, RecognizesRValueReference) {
    auto dt = parseDataType("int&&");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(dt.typeDecl.isRValReference);
    EXPECT_FALSE(dt.typeDecl.isLValReference);
    EXPECT_EQ(dt.typeDecl.ptrCount, 0);
}

// Test: "int*&" should be recognized as a combination of pointer and lvalue reference.
TEST(ParseDataTypeTest, RecognizesPointerAndLValueReference) {
    auto dt = parseDataType("int*&");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(dt.typeDecl.ptrCount, 1);
    EXPECT_TRUE(dt.typeDecl.isLValReference);
    EXPECT_FALSE(dt.typeDecl.isRValReference);
}

// Test: "int[10]" should be recognized with an array dimension.
TEST(ParseDataTypeTest, RecognizesArrayDimension) {
    auto dt = parseDataType("int[10]");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    ASSERT_EQ(dt.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(dt.typeDecl.arrayDimensions[0], "10");
}

// Test: "int*[5]" should be recognized with both a pointer and an array dimension.
TEST(ParseDataTypeTest, RecognizesPointerAndArray) {
    auto dt = parseDataType("int*[5]");
    EXPECT_EQ(dt.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(dt.typeDecl.ptrCount, 1);
    ASSERT_EQ(dt.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(dt.typeDecl.arrayDimensions[0], "5");
}
