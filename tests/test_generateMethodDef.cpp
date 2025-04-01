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


