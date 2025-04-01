#include "gtest/gtest.h"
#include "MethodParser.h"
#include "ScaffoldModels.h"
#include "ScaffoldProperties.h"
#include <vector>
#include <string_view>
#include <stdexcept>

using namespace MethodParser;  // Bring MethodParser functions into scope.

TEST(MethodParserTest, ValidMethodProperties) {
    // Test a full method block with return type, parameters, and description.
    std::string methodName = "doSomething";
    std::vector<std::string_view> propertyLines = {
        " | return = int",
        " | parameters = param1:int, param2:float",
        " | description = \"Performs a calculation\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    // Verify method name.
    EXPECT_EQ(methodModel.name, methodName);
    // Verify return type.
    EXPECT_EQ(methodModel.returnType.type, ScaffoldProperties::Types::INT);
    // Verify parameters.
    ASSERT_EQ(methodModel.parameters.size(), 2);
    EXPECT_EQ(methodModel.parameters[0].name, "param1");
    EXPECT_EQ(methodModel.parameters[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(methodModel.parameters[1].name, "param2");
    EXPECT_EQ(methodModel.parameters[1].type.type, ScaffoldProperties::Types::FLOAT);
    // Verify description (quotes should be removed).
    EXPECT_EQ(methodModel.description, "Performs a calculation");
}

TEST(MethodParserTest, OnlyDescriptionProvided) {
    // Test a method block with only a description property.
    std::string methodName = "doSomethingElse";
    std::vector<std::string_view> propertyLines = {
        " | description = \"Just a description\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);

    // Method name should be set.
    EXPECT_EQ(methodModel.name, methodName);
    // Default return type should be void.
    EXPECT_EQ(methodModel.returnType.type, ScaffoldProperties::Types::VOID);
    // No parameters should be present.
    EXPECT_TRUE(methodModel.parameters.empty());
    // Description is set correctly.
    EXPECT_EQ(methodModel.description, "Just a description");
}

TEST(MethodParserTest, WhitespaceTrimming) {
    // Test that extra whitespace in property lines is trimmed properly.
    std::string methodName = "doTest";
    std::vector<std::string_view> propertyLines = {
        "   |  return =   double   ",
        "   |  parameters =  p1: int  ,   p2 :  float   ",
        "   |  description =  \" Test method \"   "
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);

    // Verify that return type is double.
    EXPECT_EQ(methodModel.returnType.type, ScaffoldProperties::Types::DOUBLE);
    // Verify that parameters are correctly parsed.
    ASSERT_EQ(methodModel.parameters.size(), 2);
    EXPECT_EQ(methodModel.parameters[0].name, "p1");
    EXPECT_EQ(methodModel.parameters[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(methodModel.parameters[1].name, "p2");
    EXPECT_EQ(methodModel.parameters[1].type.type, ScaffoldProperties::Types::FLOAT);
    // Verify description trimming.
    EXPECT_EQ(methodModel.description, "Test method");
}

TEST(MethodParserTest, UnrecognizedPropertyThrows) {
    // Test that an unrecognized property in the method block causes an exception.
    std::string methodName = "doSomething";
    std::vector<std::string_view> propertyLines = {
        " | unknown = value"
    };

    EXPECT_THROW({
        parseMethodProperties(methodName, propertyLines);
    }, std::runtime_error);
}

TEST(MethodParserTest, ReturnTypeWithQualifier) {
    // Test that a method with a qualified return type is parsed correctly.
    std::string methodName = "doConstCalc";
    std::vector<std::string_view> propertyLines = {
        " | return = const int",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns a constant int\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, ScaffoldProperties::Types::INT);
    // Check that the return type has the const qualifier.
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(methodModel.returnType.qualifiers,
                                                   ScaffoldProperties::TypeQualifier::CONST));
    EXPECT_FALSE(ScaffoldProperties::hasQualifier(methodModel.returnType.qualifiers,
                                                    ScaffoldProperties::TypeQualifier::VOLATILE));
}

TEST(MethodParserTest, ParameterWithMultipleQualifiers) {
    // Test that parameters with both const and volatile qualifiers are parsed correctly.
    std::string methodName = "doComplexCalc";
    std::vector<std::string_view> propertyLines = {
        " | return = double",
        " | parameters = param1: const volatile int, param2: volatile float",
        " | description = \"Calculates with qualified parameters\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    ASSERT_EQ(methodModel.parameters.size(), 2);
    
    // For param1, expect both const and volatile.
    EXPECT_EQ(methodModel.parameters[0].name, "param1");
    EXPECT_EQ(methodModel.parameters[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(methodModel.parameters[0].type.qualifiers,
                                                   ScaffoldProperties::TypeQualifier::CONST));
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(methodModel.parameters[0].type.qualifiers,
                                                   ScaffoldProperties::TypeQualifier::VOLATILE));
    
    // For param2, expect only volatile.
    EXPECT_EQ(methodModel.parameters[1].name, "param2");
    EXPECT_EQ(methodModel.parameters[1].type.type, ScaffoldProperties::Types::FLOAT);
    EXPECT_FALSE(ScaffoldProperties::hasQualifier(methodModel.parameters[1].type.qualifiers,
                                                    ScaffoldProperties::TypeQualifier::CONST));
    EXPECT_TRUE(ScaffoldProperties::hasQualifier(methodModel.parameters[1].type.qualifiers,
                                                   ScaffoldProperties::TypeQualifier::VOLATILE));
}
