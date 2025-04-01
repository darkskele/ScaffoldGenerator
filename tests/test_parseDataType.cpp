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
