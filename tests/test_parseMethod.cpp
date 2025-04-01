#include "gtest/gtest.h"
#include "MethodParser.h"
#include "ScaffoldModels.h"
#include "ScaffoldProperties.h"
#include "PropertiesGenerator.h"
#include <vector>
#include <string_view>
#include <stdexcept>

using namespace MethodParser;  // Bring MethodParser functions into scope.
using namespace ScaffoldProperties;

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

// Test: Return type with a single pointer.
TEST(MethodParserTest, ReturnTypeWithPointer) {
    std::string methodName = "doPointerCalc";
    std::vector<std::string_view> propertyLines = {
        " | return = int*",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns a pointer to int\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(methodModel.returnType.typeDecl.ptrCount, 1);
    EXPECT_FALSE(methodModel.returnType.typeDecl.isLValReference);
    EXPECT_FALSE(methodModel.returnType.typeDecl.isRValReference);
    EXPECT_TRUE(methodModel.returnType.typeDecl.arrayDimensions.empty());
}

// Test: Return type with an lvalue reference.
TEST(MethodParserTest, ReturnTypeWithLValueReference) {
    std::string methodName = "doReferenceCalc";
    std::vector<std::string_view> propertyLines = {
        " | return = int&",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns an lvalue reference to int\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(methodModel.returnType.typeDecl.isLValReference);
    EXPECT_FALSE(methodModel.returnType.typeDecl.isRValReference);
    EXPECT_EQ(methodModel.returnType.typeDecl.ptrCount, 0);
    EXPECT_TRUE(methodModel.returnType.typeDecl.arrayDimensions.empty());
}

// Test: Return type with an rvalue reference.
TEST(MethodParserTest, ReturnTypeWithRValueReference) {
    std::string methodName = "doRValueCalc";
    std::vector<std::string_view> propertyLines = {
        " | return = int&&",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns an rvalue reference to int\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, ScaffoldProperties::Types::INT);
    EXPECT_TRUE(methodModel.returnType.typeDecl.isRValReference);
    EXPECT_FALSE(methodModel.returnType.typeDecl.isLValReference);
    EXPECT_EQ(methodModel.returnType.typeDecl.ptrCount, 0);
    EXPECT_TRUE(methodModel.returnType.typeDecl.arrayDimensions.empty());
}

// Test: Return type with an array dimension.
TEST(MethodParserTest, ReturnTypeWithArray) {
    std::string methodName = "doArrayCalc";
    std::vector<std::string_view> propertyLines = {
        " | return = int[10]",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns an array of 10 ints\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, ScaffoldProperties::Types::INT);
    ASSERT_EQ(methodModel.returnType.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(methodModel.returnType.typeDecl.arrayDimensions[0], "10");
}

// Test: Parameter with a combination of pointer and array dimension.
TEST(MethodParserTest, ParameterWithPointerAndArray) {
    std::string methodName = "doParamTest";
    std::vector<std::string_view> propertyLines = {
        " | return = void",
        " | parameters = param1: int*[5]",
        " | description = \"Parameter with pointer and array dimension\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    ASSERT_EQ(methodModel.parameters.size(), 1);
    
    EXPECT_EQ(methodModel.parameters[0].name, "param1");
    EXPECT_EQ(methodModel.parameters[0].type.type, ScaffoldProperties::Types::INT);
    EXPECT_EQ(methodModel.parameters[0].type.typeDecl.ptrCount, 1);
    ASSERT_EQ(methodModel.parameters[0].type.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(methodModel.parameters[0].type.typeDecl.arrayDimensions[0], "5");
}

// Test: Declaration specifiers are parsed from the DSL method block.
TEST(MethodParserTest, DeclarationSpecifiersParsed) {
    std::string methodName = "doSomething";
    std::vector<std::string_view> propertyLines = {
        " | declaration = static inline constexpr",
        " | return = int",
        " | parameters = param1:int, param2:float",
        " | description = \"Does something\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    // Verify that all specifiers are set.
    EXPECT_TRUE(methodModel.declSpec.isStatic);
    EXPECT_TRUE(methodModel.declSpec.isInline);
    EXPECT_TRUE(methodModel.declSpec.isConstexpr);
}
