#include "gtest/gtest.h"
#include "CallableGenerator.h"
#include "CallableModels.h"
#include "PropertiesModels.h"
#include "PropertiesGenerator.h"
#include "PropertiesParser.h"
#include <vector>
#include <string>

using namespace PropertiesModels;
using namespace CallableModels;

// ----- Method Definition Generation Tests ----- //

// Test: Method with no parameters.
TEST(GenerateMethodDefinitionTest, NoParameters) {
    // Create a MethodModel with return type "int" and no parameters.
    DataType returnType(Types::INT);
    std::vector<Parameter> params;  // No parameters.
    DeclartionSpecifier declSpec;   // Default declaration specifiers.
    MethodModel method(returnType, "doSomething", params, declSpec, "Does something");
    
    // Owning class name.
    std::string className = "MyClass";
    // Generate the method definition.
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Declaration with fully qualified name and default method body.
    std::string expected = 
        "int MyClass::doSomething() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with parameters.
TEST(GenerateMethodDefinitionTest, WithParameters) {
    // Create a MethodModel with return type "void" and two parameters.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    // Parameter "param1" of type int.
    params.emplace_back(Parameter(DataType(Types::INT), "param1"));
    // Parameter "param2" of type float.
    params.emplace_back(Parameter(DataType(Types::FLOAT), "param2"));
    
    DeclartionSpecifier declSpec;   // Default declaration specifiers.
    MethodModel method(returnType, "doSomething", params, declSpec, "Does something");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Method definition with parameter list.
    std::string expected = 
        "void MyClass::doSomething(int param1, float param2) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with return type qualified as const.
TEST(GenerateMethodDefinitionTest, ReturnTypeWithConstQualifier) {
    // Create a MethodModel with return type "const int" and no parameters.
    DataType returnType(Types::INT, TypeQualifier::CONST);
    std::vector<Parameter> params;  // No parameters.
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Returns a constant int");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Return type prefixed with "const".
    std::string expected =
        "const int MyClass::doSomething() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a parameter qualified as const.
TEST(GenerateMethodDefinitionTest, ParameterWithConstQualifier) {
    // Create a MethodModel with one parameter "param1" of type "const float".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Takes a constant float parameter");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Parameter type includes "const".
    std::string expected =
        "void MyClass::doSomething(const float param1) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a parameter qualified as const volatile.
TEST(GenerateMethodDefinitionTest, ParameterWithConstVolatileQualifiers) {
    // Create a MethodModel with one parameter "param1" of type "const volatile int".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::INT, TypeQualifier::CONST | TypeQualifier::VOLATILE), "param1"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Takes a const volatile int parameter");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Parameter type includes both "const" and "volatile".
    std::string expected =
        "void MyClass::doSomething(const volatile int param1) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a return type pointer.
TEST(GenerateMethodDefinitionTest, ReturnTypeWithPointer) {
    // Create a MethodModel with return type "int*" (pointer to int).
    DataType returnType(Types::INT);
    returnType.typeDecl.ptrCount = 1;  // Set pointer level.
    std::vector<Parameter> params;     // No parameters.
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doPointer", params, declSpec, "Returns a pointer to int");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Return type includes a pointer symbol.
    std::string expected =
        "int* MyClass::doPointer() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a return type lvalue reference.
TEST(GenerateMethodDefinitionTest, ReturnTypeWithLValueReference) {
    // Create a MethodModel with return type "int&".
    DataType returnType(Types::INT);
    returnType.typeDecl.isLValReference = true;  // Set lvalue reference.
    std::vector<Parameter> params;               // No parameters.
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doLValueRef", params, declSpec, "Returns an lvalue reference to int");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Return type includes "&".
    std::string expected =
        "int& MyClass::doLValueRef() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a return type rvalue reference.
TEST(GenerateMethodDefinitionTest, ReturnTypeWithRValueReference) {
    // Create a MethodModel with return type "int&&".
    DataType returnType(Types::INT);
    returnType.typeDecl.isRValReference = true;  // Set rvalue reference.
    std::vector<Parameter> params;               // No parameters.
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doRValueRef", params, declSpec, "Returns an rvalue reference to int");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Return type includes "&&".
    std::string expected =
        "int&& MyClass::doRValueRef() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a return type array.
TEST(GenerateMethodDefinitionTest, ReturnTypeWithArray) {
    // Create a MethodModel with return type "int[10]".
    DataType returnType(Types::INT);
    returnType.typeDecl.arrayDimensions.push_back("10");  // Add array dimension.
    std::vector<Parameter> params;                         // No parameters.
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doArray", params, declSpec, "Returns an array of 10 ints");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Return type includes array declarator.
    std::string expected =
        "int[10] MyClass::doArray() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a parameter that is a pointer.
TEST(GenerateMethodDefinitionTest, ParameterWithPointer) {
    // Create a MethodModel with a parameter "ptr" of type "int*".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1;  // Parameter is a pointer.
    params.emplace_back(Parameter(paramType, "ptr"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamPointer", params, declSpec, "Takes a pointer parameter");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Parameter type includes pointer symbol.
    std::string expected =
        "void MyClass::doParamPointer(int* ptr) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a parameter that is an lvalue reference.
TEST(GenerateMethodDefinitionTest, ParameterWithLValueReference) {
    // Create a MethodModel with a parameter "ref" of type "int&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isLValReference = true;  // Parameter is an lvalue reference.
    params.emplace_back(Parameter(paramType, "ref"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamLValueRef", params, declSpec, "Takes an lvalue reference parameter");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Parameter type includes "&".
    std::string expected =
        "void MyClass::doParamLValueRef(int& ref) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a parameter that is an rvalue reference.
TEST(GenerateMethodDefinitionTest, ParameterWithRValueReference) {
    // Create a MethodModel with a parameter "temp" of type "int&&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isRValReference = true;  // Parameter is an rvalue reference.
    params.emplace_back(Parameter(paramType, "temp"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamRValueRef", params, declSpec, "Takes an rvalue reference parameter");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Parameter type includes "&&".
    std::string expected =
        "void MyClass::doParamRValueRef(int&& temp) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with a parameter that has both pointer and array declarators.
TEST(GenerateMethodDefinitionTest, ParameterWithPointerAndArray) {
    // Create a MethodModel with a parameter "data" of type "int*[5]".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1;  // Parameter is a pointer.
    paramType.typeDecl.arrayDimensions.push_back("5");  // Add an array dimension.
    params.emplace_back(Parameter(paramType, "data"));
    DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamPtrArray", params, declSpec, "Takes a pointer and array parameter");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Parameter type includes both pointer and array declarators.
    std::string expected =
        "void MyClass::doParamPtrArray(int*[5] data) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method definition with declaration specifiers.
TEST(GenerateMethodDefinitionTest, WithDeclarationSpecifiers) {
    // Create a MethodModel with declaration specifiers: static inline constexpr.
    // Parse the specifiers from a DSL string.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("static inline constexpr");
    DataType returnType(Types::INT);
    std::vector<Parameter> params;  // No parameters.
    MethodModel method(returnType, "doSomething", params, ds, "Does something");
    
    std::string className = "MyClass";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Declaration specifiers appear before the return type.
    std::string expected =
        "static inline constexpr int MyClass::doSomething() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method definition with declaration specifiers and parameters.
TEST(GenerateMethodDefinitionTest, WithDeclarationSpecifiersAndParameters) {
    // Create a MethodModel with declaration specifiers: inline constexpr.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("inline constexpr");
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    // Create two parameters: one const-qualified float and one int.
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    params.emplace_back(Parameter(DataType(Types::INT), "param2"));
    MethodModel method(returnType, "doWork", params, ds, "Does work");
    
    std::string className = "Worker";
    std::string generated = CallableGenerator::generateMethodDefinition(className, method);
    
    // Expected output: Specifiers, return type, fully qualified name, and parameter list in order.
    std::string expected =
        "inline constexpr void Worker::doWork(const float param1, int param2) {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n";
    
    EXPECT_EQ(generated, expected);
}
