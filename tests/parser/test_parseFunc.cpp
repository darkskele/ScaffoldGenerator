#include "gtest/gtest.h"
#include "CallableParser.h"            // Declares parseFunctionProperties
#include "ScaffoldModels.h"            // Contains the FunctionModel struct and base CallableModel
#include "ScaffoldProperties.h"        // Contains type definitions (Types, DataType, etc.)
#include "PropertiesParser.h"          // For parsing declaration specifiers and data types
#include <vector>
#include <string_view>
#include <stdexcept>

using namespace CallableParser;  // Bring CallableParser functions into scope.
using namespace ScaffoldProperties;

// ----- Function Definition Parsing Tests ----- //

// Test: Valid function block with return type, parameters, and description.
TEST(CallableParserFunctionTest, ValidFunctionProperties) {
    std::string functionName = "computeValue";
    std::vector<std::string_view> propertyLines = {
        " | return = int",
        " | parameters = a:int, b:int",
        " | description = \"Computes a value\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);
    
    // Verify function name.
    EXPECT_EQ(functionModel.name, functionName);
    // Verify return type is int.
    EXPECT_EQ(functionModel.returnType.type, Types::INT);
    // Verify that two parameters were parsed.
    ASSERT_EQ(functionModel.parameters.size(), 2);
    // Check first parameter.
    EXPECT_EQ(functionModel.parameters[0].name, "a");
    EXPECT_EQ(functionModel.parameters[0].type.type, Types::INT);
    // Check second parameter.
    EXPECT_EQ(functionModel.parameters[1].name, "b");
    EXPECT_EQ(functionModel.parameters[1].type.type, Types::INT);
    // Verify description with quotes removed.
    EXPECT_EQ(functionModel.description, "Computes a value");
}

// Test: Function block with only a description provided.
TEST(CallableParserFunctionTest, OnlyDescriptionProvided) {
    std::string functionName = "doNothing";
    std::vector<std::string_view> propertyLines = {
        " | description = \"Does nothing\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);

    // Check function name.
    EXPECT_EQ(functionModel.name, functionName);
    // Default return type should be void.
    EXPECT_EQ(functionModel.returnType.type, Types::VOID);
    // Expect no parameters.
    EXPECT_TRUE(functionModel.parameters.empty());
    // Check description.
    EXPECT_EQ(functionModel.description, "Does nothing");
}

// Test: Extra whitespace in property lines is trimmed properly.
TEST(CallableParserFunctionTest, WhitespaceTrimming) {
    std::string functionName = "testFunction";
    std::vector<std::string_view> propertyLines = {
        "   |  return =   double   ",
        "   |  parameters =  x:int  ,   y:float   ",
        "   |  description =  \" Test function \"   "
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);

    // Check that return type is double.
    EXPECT_EQ(functionModel.returnType.type, Types::DOUBLE);
    // Verify two parameters are parsed.
    ASSERT_EQ(functionModel.parameters.size(), 2);
    EXPECT_EQ(functionModel.parameters[0].name, "x");
    EXPECT_EQ(functionModel.parameters[0].type.type, Types::INT);
    EXPECT_EQ(functionModel.parameters[1].name, "y");
    EXPECT_EQ(functionModel.parameters[1].type.type, Types::FLOAT);
    // Check that description is trimmed.
    EXPECT_EQ(functionModel.description, "Test function");
}

// Test: An unrecognized property should throw a runtime error.
TEST(CallableParserFunctionTest, UnrecognizedPropertyThrows) {
    std::string functionName = "invalidFunction";
    std::vector<std::string_view> propertyLines = {
        " | unknown = value"
    };

    EXPECT_THROW({
        parseFunctionProperties(functionName, propertyLines);
    }, std::runtime_error);
}

// Test: Return type with a const qualifier is parsed correctly.
TEST(CallableParserFunctionTest, ReturnTypeWithQualifier) {
    std::string functionName = "computeConst";
    std::vector<std::string_view> propertyLines = {
        " | return = const int",
        " | parameters = a:int, b:float",
        " | description = \"Returns a constant int\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);
    
    EXPECT_EQ(functionModel.returnType.type, Types::INT);
    // Check that the return type has the const qualifier.
    EXPECT_TRUE(hasQualifier(functionModel.returnType.qualifiers, TypeQualifier::CONST));
    EXPECT_FALSE(hasQualifier(functionModel.returnType.qualifiers, TypeQualifier::VOLATILE));
}

// Test: Parameter with both const and volatile qualifiers is parsed correctly.
TEST(CallableParserFunctionTest, ParameterWithMultipleQualifiers) {
    std::string functionName = "complexCompute";
    std::vector<std::string_view> propertyLines = {
        " | return = double",
        " | parameters = a: const volatile int, b: volatile float",
        " | description = \"Computes with qualified parameters\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);
    ASSERT_EQ(functionModel.parameters.size(), 2);
    
    // For a, expect both const and volatile.
    EXPECT_EQ(functionModel.parameters[0].name, "a");
    EXPECT_EQ(functionModel.parameters[0].type.type, Types::INT);
    EXPECT_TRUE(hasQualifier(functionModel.parameters[0].type.qualifiers, TypeQualifier::CONST));
    EXPECT_TRUE(hasQualifier(functionModel.parameters[0].type.qualifiers, TypeQualifier::VOLATILE));
    
    // For b, expect only volatile.
    EXPECT_EQ(functionModel.parameters[1].name, "b");
    EXPECT_EQ(functionModel.parameters[1].type.type, Types::FLOAT);
    EXPECT_FALSE(hasQualifier(functionModel.parameters[1].type.qualifiers, TypeQualifier::CONST));
    EXPECT_TRUE(hasQualifier(functionModel.parameters[1].type.qualifiers, TypeQualifier::VOLATILE));
}

// Test: Return type with a pointer declarator.
TEST(CallableParserFunctionTest, ReturnTypeWithPointer) {
    std::string functionName = "getPointer";
    std::vector<std::string_view> propertyLines = {
        " | return = int*",
        " | parameters = a:int, b:float",
        " | description = \"Returns a pointer to int\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);
    
    EXPECT_EQ(functionModel.returnType.type, Types::INT);
    EXPECT_EQ(functionModel.returnType.typeDecl.ptrCount, 1);
    EXPECT_FALSE(functionModel.returnType.typeDecl.isLValReference);
    EXPECT_FALSE(functionModel.returnType.typeDecl.isRValReference);
    EXPECT_TRUE(functionModel.returnType.typeDecl.arrayDimensions.empty());
}

// Test: Return type with an lvalue reference.
TEST(CallableParserFunctionTest, ReturnTypeWithLValueReference) {
    std::string functionName = "getLValueRef";
    std::vector<std::string_view> propertyLines = {
        " | return = int&",
        " | parameters = a:int, b:float",
        " | description = \"Returns an lvalue reference to int\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);
    
    EXPECT_EQ(functionModel.returnType.type, Types::INT);
    EXPECT_TRUE(functionModel.returnType.typeDecl.isLValReference);
    EXPECT_FALSE(functionModel.returnType.typeDecl.isRValReference);
    EXPECT_EQ(functionModel.returnType.typeDecl.ptrCount, 0);
    EXPECT_TRUE(functionModel.returnType.typeDecl.arrayDimensions.empty());
}

// Test: Return type with an rvalue reference.
TEST(CallableParserFunctionTest, ReturnTypeWithRValueReference) {
    std::string functionName = "getRValueRef";
    std::vector<std::string_view> propertyLines = {
        " | return = int&&",
        " | parameters = a:int, b:float",
        " | description = \"Returns an rvalue reference to int\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);
    
    EXPECT_EQ(functionModel.returnType.type, Types::INT);
    EXPECT_TRUE(functionModel.returnType.typeDecl.isRValReference);
    EXPECT_FALSE(functionModel.returnType.typeDecl.isLValReference);
    EXPECT_EQ(functionModel.returnType.typeDecl.ptrCount, 0);
    EXPECT_TRUE(functionModel.returnType.typeDecl.arrayDimensions.empty());
}

// Test: Return type with an array dimension.
TEST(CallableParserFunctionTest, ReturnTypeWithArray) {
    std::string functionName = "getArray";
    std::vector<std::string_view> propertyLines = {
        " | return = int[10]",
        " | parameters = a:int, b:float",
        " | description = \"Returns an array of 10 ints\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);
    
    EXPECT_EQ(functionModel.returnType.type, Types::INT);
    ASSERT_EQ(functionModel.returnType.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(functionModel.returnType.typeDecl.arrayDimensions[0], "10");
}

// Test: Parameter with a combination of pointer and array declarators.
TEST(CallableParserFunctionTest, ParameterWithPointerAndArray) {
    std::string functionName = "processData";
    std::vector<std::string_view> propertyLines = {
        " | return = void",
        " | parameters = data: int*[5]",
        " | description = \"Parameter with pointer and array dimension\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);
    ASSERT_EQ(functionModel.parameters.size(), 1);
    
    EXPECT_EQ(functionModel.parameters[0].name, "data");
    EXPECT_EQ(functionModel.parameters[0].type.type, Types::INT);
    EXPECT_EQ(functionModel.parameters[0].type.typeDecl.ptrCount, 1);
    ASSERT_EQ(functionModel.parameters[0].type.typeDecl.arrayDimensions.size(), 1);
    EXPECT_EQ(functionModel.parameters[0].type.typeDecl.arrayDimensions[0], "5");
}

// Test: Declaration specifiers are parsed correctly from the DSL.
TEST(CallableParserFunctionTest, DeclarationSpecifiersParsed) {
    std::string functionName = "doSomething";
    std::vector<std::string_view> propertyLines = {
        " | declaration = static inline constexpr",
        " | return = int",
        " | parameters = a:int, b:float",
        " | description = \"Does something\""
    };

    auto functionModel = parseFunctionProperties(functionName, propertyLines);
    
    // Verify that all declaration specifiers are set.
    EXPECT_TRUE(functionModel.declSpec.isStatic);
    EXPECT_TRUE(functionModel.declSpec.isInline);
    EXPECT_TRUE(functionModel.declSpec.isConstexpr);
}
