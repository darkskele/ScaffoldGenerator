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

// Additional tests could be added here for edge cases or custom types.
