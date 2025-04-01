#include "gtest/gtest.h"
#include "MethodGenerator.h"
#include "ScaffoldModels.h"
#include "ScaffoldProperties.h"
#include <vector>
#include <string>

using namespace ScaffoldProperties;
using namespace ScaffoldModels;

// Test: Method with no parameters.
TEST(GenerateMethodDefinitionTest, NoParameters) {
    // Create a MethodModel with return type "int", no parameters.
    DataType returnType(Types::INT);
    std::vector<Parameter> params;  // No parameters.
    MethodModel method(returnType, "doSomething", params, "Does something");
    
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
    MethodModel method(returnType, "doSomething", params, "Does something");
    
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
    MethodModel method(returnType, "doSomething", params, "Returns a constant int");
    
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
    
    MethodModel method(returnType, "doSomething", params, "Takes a constant float parameter");
    
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
    
    MethodModel method(returnType, "doSomething", params, "Takes a const volatile int parameter");
    
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
    MethodModel method(returnType, "doPointer", params, "Returns a pointer to int");
    
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
    MethodModel method(returnType, "doLValueRef", params, "Returns an lvalue reference to int");
    
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
    MethodModel method(returnType, "doRValueRef", params, "Returns an rvalue reference to int");
    
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
    MethodModel method(returnType, "doArray", params, "Returns an array of 10 ints");
    
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
    MethodModel method(returnType, "doParamPointer", params, "Takes a pointer parameter");
    
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
    MethodModel method(returnType, "doParamLValueRef", params, "Takes an lvalue reference parameter");
    
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
    MethodModel method(returnType, "doParamRValueRef", params, "Takes an rvalue reference parameter");
    
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
    MethodModel method(returnType, "doParamPtrArray", params, "Takes a pointer and array parameter");
    
    std::string className = "MyClass";
    std::string generated = MethodGenerator::generateMethodDefinition(className, method);
    
    std::string expected =
        "void MyClass::doParamPtrArray(int*[5] data) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}
