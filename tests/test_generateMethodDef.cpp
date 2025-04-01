#include "gtest/gtest.h"
#include "MethodGenerator.h"
#include "ScaffoldModels.h"
#include "ScaffoldProperties.h"
#include "PropertiesGenerator.h"
#include "PropertiesParser.h"
#include <vector>
#include <string>

using namespace ScaffoldProperties;
using namespace ScaffoldModels;

// Test: Method with no parameters.
TEST(GenerateMethodDefinitionTest, NoParameters) {
    // Create a MethodModel with return type "int", no parameters.
    DataType returnType(Types::INT);
    std::vector<Parameter> params;  // No parameters.
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Does something");
    
    // Use "MyClass" as the owning class.
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    // Expected output:
    // "int MyClass::doSomething() {\n    throw std::runtime_error(\"Not implemented\");\n}\n"
    std::string expected = "int MyClass::doSomething() {\n    throw std::runtime_error(\"Not implemented\");\n}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with parameters.
TEST(GenerateMethodDefinitionTest, WithParameters) {
    // Create a MethodModel with return type "void" and two parameters.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::INT), "param1"));
    params.emplace_back(Parameter(DataType(Types::FLOAT), "param2"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Does something");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    // Expected output:
    // "void MyClass::doSomething(int param1, float param2) {\n    throw std::runtime_error(\"Not implemented\");\n}\n"
    std::string expected = "void MyClass::doSomething(int param1, float param2) {\n    throw std::runtime_error(\"Not implemented\");\n}\n";
    
    EXPECT_EQ(generated, expected);
}

TEST(GenerateMethodDefinitionTest, ReturnTypeWithConstQualifier) {
    // Create a MethodModel with return type "const int" (qualified with const) and no parameters.
    DataType returnType(Types::INT, TypeQualifier::CONST);
    std::vector<Parameter> params;  // No parameters.
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Returns a constant int");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    // Expected output:
    // "const int MyClass::doSomething() {\n    throw std::runtime_error(\"Not implemented\");\n}\n"
    std::string expected =
        "const int MyClass::doSomething() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

TEST(GenerateMethodDefinitionTest, ParameterWithConstQualifier) {
    // Create a MethodModel with one parameter that is const-qualified.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Takes a constant float parameter");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    // Expected output:
    // "void MyClass::doSomething(const float param1) {\n    throw std::runtime_error(\"Not implemented\");\n}\n"
    std::string expected =
        "void MyClass::doSomething(const float param1) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

TEST(GenerateMethodDefinitionTest, ParameterWithConstVolatileQualifiers) {
    // Create a MethodModel with one parameter that is both const and volatile.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::INT, TypeQualifier::CONST | TypeQualifier::VOLATILE), "param1"));
    DeclartionSpecifier declSpec;
    
    MethodModel method(returnType, "doSomething", params, declSpec, "Takes a const volatile int parameter");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    // Expected output:
    // "void MyClass::doSomething(const volatile int param1) {\n    throw std::runtime_error(\"Not implemented\");\n}\n"
    std::string expected =
        "void MyClass::doSomething(const volatile int param1) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with a pointer declarator.
TEST(GenerateMethodDefinitionTest, ReturnTypeWithPointer) {
    // Create a MethodModel with return type "int*" (pointer to int).
    DataType returnType(Types::INT);
    returnType.typeDecl.ptrCount = 1;
    std::vector<Parameter> params;  // No parameters.
    DeclartionSpecifier declSpec;

    MethodModel method(returnType, "doPointer", params, declSpec, "Returns a pointer to int");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    std::string expected =
        "int* MyClass::doPointer() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with an lvalue reference declarator.
TEST(GenerateMethodDefinitionTest, ReturnTypeWithLValueReference) {
    // Create a MethodModel with return type "int&".
    DataType returnType(Types::INT);
    returnType.typeDecl.isLValReference = true;
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doLValueRef", params, declSpec, "Returns an lvalue reference to int");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    std::string expected =
        "int& MyClass::doLValueRef() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with an rvalue reference declarator.
TEST(GenerateMethodDefinitionTest, ReturnTypeWithRValueReference) {
    // Create a MethodModel with return type "int&&".
    DataType returnType(Types::INT);
    returnType.typeDecl.isRValReference = true;
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;

    MethodModel method(returnType, "doRValueRef", params, declSpec, "Returns an rvalue reference to int");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    std::string expected =
        "int&& MyClass::doRValueRef() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with an array declarator.
TEST(GenerateMethodDefinitionTest, ReturnTypeWithArray) {
    // Create a MethodModel with return type "int[10]".
    DataType returnType(Types::INT);
    returnType.typeDecl.arrayDimensions.push_back("10");
    std::vector<Parameter> params;
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doArray", params, declSpec, "Returns an array of 10 ints");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    std::string expected =
        "int[10] MyClass::doArray() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with a pointer declarator.
TEST(GenerateMethodDefinitionTest, ParameterWithPointer) {
    // Create a MethodModel with a parameter "ptr" of type "int*".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1;
    params.emplace_back(Parameter(paramType, "ptr"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamPointer", params, declSpec, "Takes a pointer parameter");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    std::string expected =
        "void MyClass::doParamPointer(int* ptr) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with an lvalue reference declarator.
TEST(GenerateMethodDefinitionTest, ParameterWithLValueReference) {
    // Create a MethodModel with a parameter "ref" of type "int&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isLValReference = true;
    params.emplace_back(Parameter(paramType, "ref"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamLValueRef", params, declSpec, "Takes an lvalue reference parameter");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    std::string expected =
        "void MyClass::doParamLValueRef(int& ref) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with an rvalue reference declarator.
TEST(GenerateMethodDefinitionTest, ParameterWithRValueReference) {
    // Create a MethodModel with a parameter "temp" of type "int&&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isRValReference = true;
    params.emplace_back(Parameter(paramType, "temp"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamRValueRef", params, declSpec, "Takes an rvalue reference parameter");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    std::string expected =
        "void MyClass::doParamRValueRef(int&& temp) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with a combination of pointer and array declarators.
TEST(GenerateMethodDefinitionTest, ParameterWithPointerAndArray) {
    // Create a MethodModel with a parameter "data" of type "int*[5]".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1;
    paramType.typeDecl.arrayDimensions.push_back("5");
    params.emplace_back(Parameter(paramType, "data"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamPtrArray", params, declSpec, "Takes a pointer and array parameter");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    std::string expected =
        "void MyClass::doParamPtrArray(int*[5] data) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method definition with declaration specifiers.
TEST(GenerateMethodDefinitionTest, WithDeclarationSpecifiers) {
    // Create a method with declaration specifiers: static inline constexpr.
    // Parse the specifiers from a DSL string.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("static inline constexpr");
    DataType returnType(Types::INT);
    std::vector<Parameter> params; // No parameters.
    MethodModel method(returnType, "doSomething", params, ds, "Does something");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    // Expected output (without extra indentation in the definition):
    std::string expected =
        "static inline constexpr int MyClass::doSomething() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method definition with declaration specifiers and parameters.
TEST(GenerateMethodDefinitionTest, WithDeclarationSpecifiersAndParameters) {
    // Create a method with declaration specifiers: inline constexpr.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("inline constexpr");
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    params.emplace_back(Parameter(DataType(Types::INT), "param2"));
    MethodModel method(returnType, "doWork", params, ds, "Does work");
    
    std::string className = "Worker";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    // Expected output:
    std::string expected =
        "inline constexpr void Worker::doWork(const float param1, int param2) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}
