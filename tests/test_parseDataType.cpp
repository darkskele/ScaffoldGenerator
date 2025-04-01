#include "gtest/gtest.h"
#include "PropertiesParser.h"      // Declares parseDataType
#include "ScaffoldProperties.h"    // Contains the Types enum and DataType struct
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
