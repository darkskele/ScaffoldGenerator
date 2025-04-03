#include "gtest/gtest.h"
#include "CallableParser.h"            // Declares parseMethodProperties
#include "ScaffoldModels.h"          // Contains the MethodModel struct
#include "ScaffoldProperties.h"      // Contains type definitions (Types, DataType, etc.)
#include <deque>
#include <string_view>
#include <stdexcept>

using namespace CallableParser;  // Bring CallableParser functions into scope.
using namespace ScaffoldProperties;

// Test: Valid method block with return type, parameters, and description.
TEST(CallableParserMethodTest, ValidMethodProperties) {
    std::string methodName = "doSomething";
    std::deque<std::string_view> propertyLines = {
        " | return = int",
        " | parameters = param1:int, param2:float",
        " | description = \"Performs a calculation\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    // Verify method name.
    EXPECT_EQ(methodModel.name, methodName);
    // Verify return type is int.
    EXPECT_EQ(methodModel.returnType.type, Types::INT);
    // Verify that two parameters were parsed.
    ASSERT_EQ(methodModel.parameters.size(), 2);
    // Check first parameter.
    EXPECT_EQ(methodModel.parameters[0].name, "param1");
    EXPECT_EQ(methodModel.parameters[0].type.type, Types::INT);
    // Check second parameter.
    EXPECT_EQ(methodModel.parameters[1].name, "param2");
    EXPECT_EQ(methodModel.parameters[1].type.type, Types::FLOAT);
    // Verify description with quotes removed.
    EXPECT_EQ(methodModel.description, "Performs a calculation");
}

// Test: Method block with only a description provided.
TEST(CallableParserMethodTest, OnlyDescriptionProvided) {
    std::string methodName = "doSomethingElse";
    std::deque<std::string_view> propertyLines = {
        " | description = \"Just a description\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);

    // Check method name.
    EXPECT_EQ(methodModel.name, methodName);
    // Default return type should be void.
    EXPECT_EQ(methodModel.returnType.type, Types::VOID);
    // Expect no parameters.
    EXPECT_TRUE(methodModel.parameters.empty());
    // Check description.
    EXPECT_EQ(methodModel.description, "Just a description");
}

// Test: Extra whitespace in property lines is trimmed properly.
TEST(CallableParserMethodTest, WhitespaceTrimming) {
    std::string methodName = "doTest";
    std::deque<std::string_view> propertyLines = {
        "   |  return =   double   ",
        "   |  parameters =  p1: int  ,   p2 :  float   ",
        "   |  description =  \" Test method \"   "
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);

    // Check that return type is double.
    EXPECT_EQ(methodModel.returnType.type, Types::DOUBLE);
    // Verify two parameters are parsed.
    ASSERT_EQ(methodModel.parameters.size(), 2);
    EXPECT_EQ(methodModel.parameters[0].name, "p1");
    EXPECT_EQ(methodModel.parameters[0].type.type, Types::INT);
    EXPECT_EQ(methodModel.parameters[1].name, "p2");
    EXPECT_EQ(methodModel.parameters[1].type.type, Types::FLOAT);
    // Check that description is trimmed.
    EXPECT_EQ(methodModel.description, "Test method");
}

// Test: An unrecognized property should throw a runtime error.
TEST(CallableParserMethodTest, UnrecognizedPropertyThrows) {
    std::string methodName = "doSomething";
    std::deque<std::string_view> propertyLines = {
        " | unknown = value"
    };

    EXPECT_THROW({
        parseMethodProperties(methodName, propertyLines);
    }, std::runtime_error);
}

// Test: Return type with a const qualifier is parsed correctly.
TEST(CallableParserMethodTest, ReturnTypeWithQualifier) {
    std::string methodName = "doConstCalc";
    std::deque<std::string_view> propertyLines = {
        " | return = const int",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns a constant int\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, Types::INT);
    // Check that the return type has the const qualifier.
    EXPECT_TRUE(hasQualifier(methodModel.returnType.qualifiers, TypeQualifier::CONST));
    EXPECT_FALSE(hasQualifier(methodModel.returnType.qualifiers, TypeQualifier::VOLATILE));
}

// Test: Parameter with both const and volatile qualifiers is parsed correctly.
TEST(CallableParserMethodTest, ParameterWithMultipleQualifiers) {
    std::string methodName = "doComplexCalc";
    std::deque<std::string_view> propertyLines = {
        " | return = double",
        " | parameters = param1: const volatile int, param2: volatile float",
        " | description = \"Calculates with qualified parameters\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    ASSERT_EQ(methodModel.parameters.size(), 2);
    
    // For param1, expect both const and volatile.
    EXPECT_EQ(methodModel.parameters[0].name, "param1");
    EXPECT_EQ(methodModel.parameters[0].type.type, Types::INT);
    EXPECT_TRUE(hasQualifier(methodModel.parameters[0].type.qualifiers, TypeQualifier::CONST));
    EXPECT_TRUE(hasQualifier(methodModel.parameters[0].type.qualifiers, TypeQualifier::VOLATILE));
    
    // For param2, expect only volatile.
    EXPECT_EQ(methodModel.parameters[1].name, "param2");
    EXPECT_EQ(methodModel.parameters[1].type.type, Types::FLOAT);
    EXPECT_FALSE(hasQualifier(methodModel.parameters[1].type.qualifiers, TypeQualifier::CONST));
    EXPECT_TRUE(hasQualifier(methodModel.parameters[1].type.qualifiers, TypeQualifier::VOLATILE));
}

// Test: Return type with a single pointer declarator.
TEST(CallableParserMethodTest, ReturnTypeWithPointer) {
    std::string methodName = "doPointerCalc";
    std::deque<std::string_view> propertyLines = {
        " | return = int*",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns a pointer to int\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, Types::INT);
    EXPECT_EQ(methodModel.returnType.typeDecl.ptrCount, 1);
    EXPECT_FALSE(methodModel.returnType.typeDecl.isLValReference);
    EXPECT_FALSE(methodModel.returnType.typeDecl.isRValReference);
    EXPECT_TRUE(methodModel.returnType.typeDecl.arrayDimensions.empty());
}

// Test: Return type with an lvalue reference.
TEST(CallableParserMethodTest, ReturnTypeWithLValueReference) {
    std::string methodName = "doReferenceCalc";
    std::deque<std::string_view> propertyLines = {
        " | return = int&",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns an lvalue reference to int\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, Types::INT);
    EXPECT_TRUE(methodModel.returnType.typeDecl.isLValReference);
    EXPECT_FALSE(methodModel.returnType.typeDecl.isRValReference);
    EXPECT_EQ(methodModel.returnType.typeDecl.ptrCount, 0);
    EXPECT_TRUE(methodModel.returnType.typeDecl.arrayDimensions.empty());
}

// Test: Return type with an rvalue reference.
TEST(CallableParserMethodTest, ReturnTypeWithRValueReference) {
    std::string methodName = "doRValueCalc";
    std::deque<std::string_view> propertyLines = {
        " | return = int&&",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns an rvalue reference to int\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, Types::INT);
    EXPECT_TRUE(methodModel.returnType.typeDecl.isRValReference);
    EXPECT_FALSE(methodModel.returnType.typeDecl.isLValReference);
    EXPECT_EQ(methodModel.returnType.typeDecl.ptrCount, 0);
    EXPECT_TRUE(methodModel.returnType.typeDecl.arrayDimensions.empty());
}

// Test: Return type with an array dimension.
TEST(CallableParserMethodTest, ReturnTypeWithArray) {
    std::string methodName = "doArrayCalc";
    std::deque<std::string_view> propertyLines = {
        " | return = int[10]",
        " | parameters = param1:int, param2:float",
        " | description = \"Returns an array of 10 ints\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    EXPECT_EQ(methodModel.returnType.type, Types::INT);
    ASSERT_EQ(methodModel.returnType.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(methodModel.returnType.typeDecl.arrayDimensions[0], "10");
}

// Test: Parameter with a combination of pointer and array declarators.
TEST(CallableParserMethodTest, ParameterWithPointerAndArray) {
    std::string methodName = "doParamTest";
    std::deque<std::string_view> propertyLines = {
        " | return = void",
        " | parameters = param1: int*[5]",
        " | description = \"Parameter with pointer and array dimension\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    ASSERT_EQ(methodModel.parameters.size(), 1);
    
    EXPECT_EQ(methodModel.parameters[0].name, "param1");
    EXPECT_EQ(methodModel.parameters[0].type.type, Types::INT);
    EXPECT_EQ(methodModel.parameters[0].type.typeDecl.ptrCount, 1);
    ASSERT_EQ(methodModel.parameters[0].type.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(methodModel.parameters[0].type.typeDecl.arrayDimensions[0], "5");
}

// Test: Declaration specifiers are parsed correctly from the DSL.
TEST(CallableParserMethodTest, DeclarationSpecifiersParsed) {
    std::string methodName = "doSomething";
    std::deque<std::string_view> propertyLines = {
        " | declaration = static inline constexpr",
        " | return = int",
        " | parameters = param1:int, param2:float",
        " | description = \"Does something\""
    };

    auto methodModel = parseMethodProperties(methodName, propertyLines);
    
    // Verify that all declaration specifiers are set.
    EXPECT_TRUE(methodModel.declSpec.isStatic);
    EXPECT_TRUE(methodModel.declSpec.isInline);
    EXPECT_TRUE(methodModel.declSpec.isConstexpr);
}
