#include "gtest/gtest.h"
#include "CallableGenerator.h"
#include "CallableModels.h"
#include "PropertiesModels.h"
#include "PropertiesParser.h"
#include <vector>
#include <string>
#include <string_view>

using namespace PropertiesModels;
using namespace CallableModels;

// ----- Free Function Declaration Generation Tests ----- //

// Test: Function with no parameters.
TEST(GenerateFunctionDeclarationTest, NoParameters) {
    // Create a FunctionModel with return type "int", no parameters, and a brief description.
    DataType returnType(Types::INT);
    std::vector<Parameter> params; // No parameters.
    DeclartionSpecifier declSpec;  // Default declaration specifiers.
    FunctionModel func(returnType, "doSomething", params, declSpec, "Performs a calculation");
    
    // Generate the declaration string.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: A Doxygen comment block followed by the function declaration.
    std::string expected =
        "/**\n"
        " * @brief Performs a calculation\n"
        " */\n"
        "int doSomething();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with parameters.
TEST(GenerateFunctionDeclarationTest, WithParameters) {
    // Create a FunctionModel with return type "void" and two parameters.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    // Parameter "param1" of type int.
    params.emplace_back(Parameter(DataType(Types::INT), "param1"));
    // Parameter "param2" of type float.
    params.emplace_back(Parameter(DataType(Types::FLOAT), "param2"));
    
    DeclartionSpecifier declSpec;  // Default declaration specifiers.
    FunctionModel func(returnType, "doSomething", params, declSpec, "Does something");
    
    // Generate the function declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output with parameter list "int param1, float param2".
    std::string expected =
        "/**\n"
        " * @brief Does something\n"
        " */\n"
        "void doSomething(int param1, float param2);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function with extra whitespace in description should preserve spaces.
TEST(GenerateFunctionDeclarationTest, WithWhitespaceInDescription) {
    // Create a FunctionModel with a description that has leading and trailing whitespace.
    DataType returnType(Types::DOUBLE);
    std::vector<Parameter> params;
    // Parameter "x" of type int.
    params.emplace_back(Parameter(DataType(Types::INT), "x"));
    DeclartionSpecifier declSpec;
    
    // Description intentionally contains extra spaces.
    FunctionModel func(returnType, "compute", params, declSpec, "  Computes a value  ");
    
    // Generate the function declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output should preserve the whitespace in the description.
    std::string expected =
        "/**\n"
        " * @brief   Computes a value  \n"
        " */\n"
        "double compute(int x);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with a const qualifier.
TEST(GenerateFunctionDeclarationTest, ReturnTypeWithConstQualifier) {
    // Create a FunctionModel with return type "const int".
    DataType returnType(Types::INT, TypeQualifier::CONST);
    std::vector<Parameter> params; // No parameters.
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doSomething", params, declSpec, "Returns a constant int");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: The return type is prefixed with "const".
    std::string expected =
        "/**\n"
        " * @brief Returns a constant int\n"
        " */\n"
        "const int doSomething();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with a const qualifier.
TEST(GenerateFunctionDeclarationTest, ParameterWithConstQualifier) {
    // Create a FunctionModel with one parameter that is const-qualified.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    // Parameter "param1" of type "const float".
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    DeclartionSpecifier declSpec;
    
    FunctionModel func(returnType, "doSomething", params, declSpec, "Takes a constant float parameter");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Parameter type should include "const".
    std::string expected =
        "/**\n"
        " * @brief Takes a constant float parameter\n"
        " */\n"
        "void doSomething(const float param1);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with both const and volatile qualifiers.
TEST(GenerateFunctionDeclarationTest, ParameterWithConstVolatileQualifiers) {
    // Create a FunctionModel with one parameter that is const and volatile.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    // Parameter "param1" of type "const volatile int".
    params.emplace_back(Parameter(DataType(Types::INT, TypeQualifier::CONST | TypeQualifier::VOLATILE), "param1"));
    DeclartionSpecifier declSpec;
    
    FunctionModel func(returnType, "doSomething", params, declSpec, "Takes a const volatile int parameter");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Parameter type should include both "const" and "volatile".
    std::string expected =
        "/**\n"
        " * @brief Takes a const volatile int parameter\n"
        " */\n"
        "void doSomething(const volatile int param1);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with a pointer declarator.
TEST(GenerateFunctionDeclarationTest, ReturnTypeWithPointer) {
    // Create a FunctionModel with return type "int*" (pointer to int).
    DataType returnType(Types::INT);
    returnType.typeDecl.ptrCount = 1;  // Set pointer count to 1.
    std::vector<Parameter> params;     // No parameters.
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doPointer", params, declSpec, "Returns a pointer to int");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Return type should include a pointer symbol.
    std::string expected =
        "/**\n"
        " * @brief Returns a pointer to int\n"
        " */\n"
        "int* doPointer();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with an lvalue reference declarator.
TEST(GenerateFunctionDeclarationTest, ReturnTypeWithLValueReference) {
    // Create a FunctionModel with return type "int&".
    DataType returnType(Types::INT);
    returnType.typeDecl.isLValReference = true; // Set as lvalue reference.
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doLValueRef", params, declSpec, "Returns an lvalue reference to int");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Return type should include an ampersand.
    std::string expected =
        "/**\n"
        " * @brief Returns an lvalue reference to int\n"
        " */\n"
        "int& doLValueRef();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with an rvalue reference declarator.
TEST(GenerateFunctionDeclarationTest, ReturnTypeWithRValueReference) {
    // Create a FunctionModel with return type "int&&".
    DataType returnType(Types::INT);
    returnType.typeDecl.isRValReference = true; // Set as rvalue reference.
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doRValueRef", params, declSpec, "Returns an rvalue reference to int");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Return type should include "&&".
    std::string expected =
        "/**\n"
        " * @brief Returns an rvalue reference to int\n"
        " */\n"
        "int&& doRValueRef();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with an array declarator.
TEST(GenerateFunctionDeclarationTest, ReturnTypeWithArray) {
    // Create a FunctionModel with return type "int[10]".
    DataType returnType(Types::INT);
    // Add an array dimension "10".
    returnType.typeDecl.arrayDimensions.push_back("10");
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doArray", params, declSpec, "Returns an array of 10 ints");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Return type should include the array declarator.
    std::string expected =
        "/**\n"
        " * @brief Returns an array of 10 ints\n"
        " */\n"
        "int[10] doArray();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with a pointer declarator.
TEST(GenerateFunctionDeclarationTest, ParameterWithPointer) {
    // Create a FunctionModel with a parameter "ptr" of type "int*".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1; // Parameter is a pointer.
    params.emplace_back(Parameter(paramType, "ptr"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doParamPointer", params, declSpec, "Takes a pointer parameter");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Parameter type should include a pointer symbol.
    std::string expected =
        "/**\n"
        " * @brief Takes a pointer parameter\n"
        " */\n"
        "void doParamPointer(int* ptr);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with an lvalue reference declarator.
TEST(GenerateFunctionDeclarationTest, ParameterWithLValueReference) {
    // Create a FunctionModel with a parameter "ref" of type "int&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isLValReference = true; // Parameter is an lvalue reference.
    params.emplace_back(Parameter(paramType, "ref"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doParamLValueRef", params, declSpec, "Takes an lvalue reference parameter");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Parameter type should include an ampersand.
    std::string expected =
        "/**\n"
        " * @brief Takes an lvalue reference parameter\n"
        " */\n"
        "void doParamLValueRef(int& ref);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with an rvalue reference declarator.
TEST(GenerateFunctionDeclarationTest, ParameterWithRValueReference) {
    // Create a FunctionModel with a parameter "temp" of type "int&&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isRValReference = true; // Parameter is an rvalue reference.
    params.emplace_back(Parameter(paramType, "temp"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doParamRValueRef", params, declSpec, "Takes an rvalue reference parameter");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Parameter type should include "&&".
    std::string expected =
        "/**\n"
        " * @brief Takes an rvalue reference parameter\n"
        " */\n"
        "void doParamRValueRef(int&& temp);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with a combination of pointer and array declarators.
TEST(GenerateFunctionDeclarationTest, ParameterWithPointerAndArray) {
    // Create a FunctionModel with a parameter "data" of type "int*[5]".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1; // Parameter is a pointer.
    // Add an array dimension "5".
    paramType.typeDecl.arrayDimensions.push_back("5");
    params.emplace_back(Parameter(paramType, "data"));
    DeclartionSpecifier declSpec;
    FunctionModel func(returnType, "doParamPtrArray", params, declSpec, "Takes a pointer and array parameter");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Parameter type should include both pointer and array declarators.
    std::string expected =
        "/**\n"
        " * @brief Takes a pointer and array parameter\n"
        " */\n"
        "void doParamPtrArray(int*[5] data);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function declaration with declaration specifiers.
TEST(GenerateFunctionDeclarationTest, WithDeclarationSpecifiers) {
    // Parse a set of declaration specifiers from a DSL string.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("static constexpr");
    DataType returnType(Types::INT);
    std::vector<Parameter> params; // No parameters.
    FunctionModel func(returnType, "doSomething", params, ds, "Does something");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Declaration specifiers appear before the return type.
    std::string expected =
        "/**\n"
        " * @brief Does something\n"
        " */\n"
        "static constexpr int doSomething();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Function declaration with declaration specifiers and parameters.
TEST(GenerateFunctionDeclarationTest, WithDeclarationSpecifiersAndParameters) {
    // Parse a combination of declaration specifiers from a DSL string.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("constexpr");
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    // Create two parameters: one const-qualified float and one int.
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    params.emplace_back(Parameter(DataType(Types::INT), "param2"));
    FunctionModel func(returnType, "doWork", params, ds, "Does work");
    
    // Generate the declaration.
    std::string generated = CallableGenerator::generateFunctionDeclaration(func);
    
    // Expected output: Specifiers, return type, function name, and parameter list are output in order.
    std::string expected =
        "/**\n"
        " * @brief Does work\n"
        " */\n"
        "constexpr void doWork(const float param1, int param2);\n";
    
    EXPECT_EQ(generated, expected);
}
