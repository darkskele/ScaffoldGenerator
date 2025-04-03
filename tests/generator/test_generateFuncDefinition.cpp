#include "gtest/gtest.h"
#include "CallableGenerator.h"
#include "ScaffoldModels.h"
#include "ScaffoldProperties.h"
#include "PropertiesParser.h"
#include <vector>
#include <string>

using namespace ScaffoldProperties;
using namespace ScaffoldModels;

// ----- Free Function Definition Generation Tests ----- //

// Test: Function with no parameters.
TEST(GenerateFunctionDefinitionTest, NoParameters) {
    // Create a FunctionModel with return type "int", no parameters, and a brief description.
    DataType returnType(Types::INT);
    std::vector<Parameter> params; // No parameters.
    DeclartionSpecifier declSpec;  // Default declaration specifiers.
    FunctionModel func(returnType, "doSomething", params, declSpec, "Does something");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: A declaration with a default function body.
    std::string expected =
        "int doSomething() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with parameters.
TEST(GenerateFunctionDefinitionTest, WithParameters) {
    // Create a FunctionModel with return type "void" and two parameters.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    // Parameter "param1" of type int.
    params.emplace_back(Parameter(DataType(Types::INT), "param1"));
    // Parameter "param2" of type float.
    params.emplace_back(Parameter(DataType(Types::FLOAT), "param2"));
    DeclartionSpecifier declSpec;  // Default declaration specifiers.
    FunctionModel func(returnType, "doSomething", params, declSpec, "Does something");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: A definition with parameter list.
    std::string expected =
        "void doSomething(int param1, float param2) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with return type qualified as const.
TEST(GenerateFunctionDefinitionTest, ReturnTypeWithConstQualifier) {
    // Create a FunctionModel with return type "const int" and no parameters.
    DataType returnType(Types::INT, TypeQualifier::CONST);
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doSomething", params, declSpec, "Returns a constant int");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Return type is prefixed with "const".
    std::string expected =
        "const int doSomething() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a parameter qualified as const.
TEST(GenerateFunctionDefinitionTest, ParameterWithConstQualifier) {
    // Create a FunctionModel with one parameter "param1" of type "const float".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doSomething", params, declSpec, "Takes a constant float parameter");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Parameter type includes "const".
    std::string expected =
        "void doSomething(const float param1) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a parameter qualified as const volatile.
TEST(GenerateFunctionDefinitionTest, ParameterWithConstVolatileQualifiers) {
    // Create a FunctionModel with one parameter "param1" of type "const volatile int".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::INT, TypeQualifier::CONST | TypeQualifier::VOLATILE), "param1"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doSomething", params, declSpec, "Takes a const volatile int parameter");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Parameter type includes both "const" and "volatile".
    std::string expected =
        "void doSomething(const volatile int param1) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a return type pointer.
TEST(GenerateFunctionDefinitionTest, ReturnTypeWithPointer) {
    // Create a FunctionModel with return type "int*" (pointer to int).
    DataType returnType(Types::INT);
    returnType.typeDecl.ptrCount = 1;  // Set pointer count to 1.
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doPointer", params, declSpec, "Returns a pointer to int");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Return type should include a pointer symbol.
    std::string expected =
        "int* doPointer() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a return type lvalue reference.
TEST(GenerateFunctionDefinitionTest, ReturnTypeWithLValueReference) {
    // Create a FunctionModel with return type "int&".
    DataType returnType(Types::INT);
    returnType.typeDecl.isLValReference = true; // Set as lvalue reference.
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doLValueRef", params, declSpec, "Returns an lvalue reference to int");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Return type includes an ampersand.
    std::string expected =
        "int& doLValueRef() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a return type rvalue reference.
TEST(GenerateFunctionDefinitionTest, ReturnTypeWithRValueReference) {
    // Create a FunctionModel with return type "int&&".
    DataType returnType(Types::INT);
    returnType.typeDecl.isRValReference = true; // Set as rvalue reference.
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doRValueRef", params, declSpec, "Returns an rvalue reference to int");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Return type includes "&&".
    std::string expected =
        "int&& doRValueRef() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a return type array.
TEST(GenerateFunctionDefinitionTest, ReturnTypeWithArray) {
    // Create a FunctionModel with return type "int[10]".
    DataType returnType(Types::INT);
    returnType.typeDecl.arrayDimensions.push_back("10");  // Add array dimension.
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doArray", params, declSpec, "Returns an array of 10 ints");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Return type should include the array declarator.
    std::string expected =
        "int[10] doArray() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a parameter that is a pointer.
TEST(GenerateFunctionDefinitionTest, ParameterWithPointer) {
    // Create a FunctionModel with a parameter "ptr" of type "int*".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1;  // Parameter is a pointer.
    params.emplace_back(Parameter(paramType, "ptr"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doParamPointer", params, declSpec, "Takes a pointer parameter");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Parameter type should include a pointer symbol.
    std::string expected =
        "void doParamPointer(int* ptr) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a parameter that is an lvalue reference.
TEST(GenerateFunctionDefinitionTest, ParameterWithLValueReference) {
    // Create a FunctionModel with a parameter "ref" of type "int&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isLValReference = true;  // Parameter is an lvalue reference.
    params.emplace_back(Parameter(paramType, "ref"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doParamLValueRef", params, declSpec, "Takes an lvalue reference parameter");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Parameter type should include an ampersand.
    std::string expected =
        "void doParamLValueRef(int& ref) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a parameter that is an rvalue reference.
TEST(GenerateFunctionDefinitionTest, ParameterWithRValueReference) {
    // Create a FunctionModel with a parameter "temp" of type "int&&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isRValReference = true;  // Parameter is an rvalue reference.
    params.emplace_back(Parameter(paramType, "temp"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doParamRValueRef", params, declSpec, "Takes an rvalue reference parameter");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Parameter type should include "&&".
    std::string expected =
        "void doParamRValueRef(int&& temp) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with a parameter that has both pointer and array declarators.
TEST(GenerateFunctionDefinitionTest, ParameterWithPointerAndArray) {
    // Create a FunctionModel with a parameter "data" of type "int*[5]".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1;  // Parameter is a pointer.
    paramType.typeDecl.arrayDimensions.push_back("5");  // Add an array dimension.
    params.emplace_back(Parameter(paramType, "data"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doParamPtrArray", params, declSpec, "Takes a pointer and array parameter");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Parameter type includes both pointer and array declarators.
    std::string expected =
        "void doParamPtrArray(int*[5] data) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function definition with declaration specifiers.
TEST(GenerateFunctionDefinitionTest, WithDeclarationSpecifiers) {
    // Parse a set of declaration specifiers.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("static inline constexpr");
    DataType returnType(Types::INT);
    std::vector<Parameter> params; // No parameters.
    FunctionModel func(returnType, "doSomething", params, ds, "Does something");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Declaration specifiers appear before the return type.
    std::string expected =
        "static inline constexpr int doSomething() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function definition with declaration specifiers and parameters.
TEST(GenerateFunctionDefinitionTest, WithDeclarationSpecifiersAndParameters) {
    // Parse a combination of declaration specifiers.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("inline constexpr");
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    // Create two parameters: one const-qualified float and one int.
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    params.emplace_back(Parameter(DataType(Types::INT), "param2"));
    FunctionModel func(returnType, "doWork", params, ds, "Does work");
    
    // Generate the free function definition.
    std::string generated = CallableGenerator::generateFunctionDefinition(func);
    
    // Expected output: Specifiers, return type, function name, and parameter list in order.
    std::string expected =
        "inline constexpr void doWork(const float param1, int param2) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}
