#include "gtest/gtest.h"
#include "MethodGenerator.h"
#include "ScaffoldModels.h"
#include "ScaffoldProperties.h"
#include "PropertiesParser.h"
#include <vector>
#include <string>
#include <string_view>

using namespace ScaffoldProperties;
using namespace ScaffoldModels;

// ----- Method Declaration Generation Tests ----- //

// Test: Method with no parameters.
TEST(GenerateMethodDeclarationTest, NoParameters) {
    // Create a MethodModel with return type "int", no parameters, and a description.
    DataType returnType(Types::INT);
    std::vector<Parameter> params; // Empty parameters.
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Performs a calculation");
    
    // Generate the declaration.
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    // Expected output (4-space indent for the Doxygen comment and declaration):
    std::string expected =
        "    /**\n"
        "     * @brief Performs a calculation\n"
        "     */\n"
        "    int doSomething();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with parameters.
TEST(GenerateMethodDeclarationTest, WithParameters) {
    // Create a MethodModel with return type "void" and two parameters.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::INT), "param1"));
    params.emplace_back(Parameter(DataType(Types::FLOAT), "param2"));
    
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Does something");
    
    // Generate the declaration.
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    // Expected parameter list: "int param1, float param2"
    std::string expected =
        "    /**\n"
        "     * @brief Does something\n"
        "     */\n"
        "    void doSomething(int param1, float param2);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method with extra whitespace in description should be preserved as-is.
TEST(GenerateMethodDeclarationTest, WithWhitespaceInDescription) {
    // Create a method model where the description includes extra spaces.
    DataType returnType(Types::DOUBLE);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::INT), "x"));
    ScaffoldProperties::DeclartionSpecifier declSpec;
    
    // Description intentionally has leading/trailing spaces.
    MethodModel method(returnType, "compute", params, declSpec, "  Computes a value  ");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    // Expected: The description is output exactly as provided.
    std::string expected =
        "    /**\n"
        "     * @brief   Computes a value  \n"
        "     */\n"
        "    double compute(int x);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with a const qualifier.
TEST(GenerateMethodDeclarationTest, ReturnTypeWithConstQualifier) {
    // Method with a return type "const int".
    DataType returnType(Types::INT, TypeQualifier::CONST);
    std::vector<Parameter> params; // No parameters.
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doSomething", params, declSpec, "Returns a constant int");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Returns a constant int\n"
        "     */\n"
        "    const int doSomething();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with a const qualifier.
TEST(GenerateMethodDeclarationTest, ParameterWithConstQualifier) {
    // Method with one parameter that is const-qualified.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    ScaffoldProperties::DeclartionSpecifier declSpec;
    
    MethodModel method(returnType, "doSomething", params, declSpec, "Takes a constant float parameter");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Takes a constant float parameter\n"
        "     */\n"
        "    void doSomething(const float param1);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with const and volatile qualifiers.
TEST(GenerateMethodDeclarationTest, ParameterWithConstVolatileQualifiers) {
    // Method with one parameter that is both const and volatile.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::INT, TypeQualifier::CONST | TypeQualifier::VOLATILE), "param1"));
    ScaffoldProperties::DeclartionSpecifier declSpec;
    
    MethodModel method(returnType, "doSomething", params, declSpec, "Takes a const volatile int parameter");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Takes a const volatile int parameter\n"
        "     */\n"
        "    void doSomething(const volatile int param1);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with a pointer declarator.
TEST(GenerateMethodDeclarationTest, ReturnTypeWithPointer) {
    // Create a MethodModel with return type "int*" (pointer to int) and no parameters.
    DataType returnType(Types::INT);
    returnType.typeDecl.ptrCount = 1;
    std::vector<Parameter> params; // No parameters.
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doPointer", params, declSpec, "Returns a pointer to int");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Returns a pointer to int\n"
        "     */\n"
        "    int* doPointer();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with an lvalue reference declarator.
TEST(GenerateMethodDeclarationTest, ReturnTypeWithLValueReference) {
    // Create a MethodModel with return type "int&" and no parameters.
    DataType returnType(Types::INT);
    returnType.typeDecl.isLValReference = true;
    std::vector<Parameter> params;
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doLValueRef", params, declSpec, "Returns an lvalue reference to int");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Returns an lvalue reference to int\n"
        "     */\n"
        "    int& doLValueRef();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with an rvalue reference declarator.
TEST(GenerateMethodDeclarationTest, ReturnTypeWithRValueReference) {
    // Create a MethodModel with return type "int&&" and no parameters.
    DataType returnType(Types::INT);
    returnType.typeDecl.isRValReference = true;
    std::vector<Parameter> params;
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doRValueRef", params, declSpec, "Returns an rvalue reference to int");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Returns an rvalue reference to int\n"
        "     */\n"
        "    int&& doRValueRef();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Return type with an array declarator.
TEST(GenerateMethodDeclarationTest, ReturnTypeWithArray) {
    // Create a MethodModel with return type "int[10]".
    DataType returnType(Types::INT);
    returnType.typeDecl.arrayDimensions.push_back("10");
    std::vector<Parameter> params;
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doArray", params, declSpec, "Returns an array of 10 ints");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Returns an array of 10 ints\n"
        "     */\n"
        "    int[10] doArray();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with a pointer declarator.
TEST(GenerateMethodDeclarationTest, ParameterWithPointer) {
    // Create a MethodModel with a parameter "ptr" of type "int*".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1;
    params.emplace_back(Parameter(paramType, "ptr"));
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamPointer", params, declSpec, "Takes a pointer parameter");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Takes a pointer parameter\n"
        "     */\n"
        "    void doParamPointer(int* ptr);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with an lvalue reference declarator.
TEST(GenerateMethodDeclarationTest, ParameterWithLValueReference) {
    // Create a MethodModel with a parameter "ref" of type "int&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isLValReference = true;
    params.emplace_back(Parameter(paramType, "ref"));
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamLValueRef", params, declSpec, "Takes an lvalue reference parameter");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Takes an lvalue reference parameter\n"
        "     */\n"
        "    void doParamLValueRef(int& ref);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with an rvalue reference declarator.
TEST(GenerateMethodDeclarationTest, ParameterWithRValueReference) {
    // Create a MethodModel with a parameter "temp" of type "int&&".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.isRValReference = true;
    params.emplace_back(Parameter(paramType, "temp"));
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamRValueRef", params, declSpec, "Takes an rvalue reference parameter");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Takes an rvalue reference parameter\n"
        "     */\n"
        "    void doParamRValueRef(int&& temp);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Parameter with a combination of pointer and array declarators.
TEST(GenerateMethodDeclarationTest, ParameterWithPointerAndArray) {
    // Create a MethodModel with a parameter "data" of type "int*[5]".
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    DataType paramType(Types::INT);
    paramType.typeDecl.ptrCount = 1;
    paramType.typeDecl.arrayDimensions.push_back("5");
    params.emplace_back(Parameter(paramType, "data"));
    ScaffoldProperties::DeclartionSpecifier declSpec;
    MethodModel method(returnType, "doParamPtrArray", params, declSpec, "Takes a pointer and array parameter");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Takes a pointer and array parameter\n"
        "     */\n"
        "    void doParamPtrArray(int*[5] data);\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method declaration with declaration specifiers.
TEST(GenerateMethodDeclarationTest, WithDeclarationSpecifiers) {
    // Parse the declaration specifiers from a DSL string.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("static inline constexpr");
    DataType returnType(Types::INT);
    std::vector<Parameter> params; // No parameters.
    MethodModel method(returnType, "doSomething", params, ds, "Does something");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    // Expected output: Specifiers precede the return type.
    std::string expected =
        "    /**\n"
        "     * @brief Does something\n"
        "     */\n"
        "    static inline constexpr int doSomething();\n";
    
    EXPECT_EQ(generated, expected);
}

// Test: Method declaration with declaration specifiers and parameters.
TEST(GenerateMethodDeclarationTest, WithDeclarationSpecifiersAndParameters) {
    // Parse a combination of declaration specifiers.
    DeclartionSpecifier ds = PropertiesParser::parseDeclarationSpecifier("inline constexpr");
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    // Create two parameters: one const-qualified float and one int.
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    params.emplace_back(Parameter(DataType(Types::INT), "param2"));
    MethodModel method(returnType, "doWork", params, ds, "Does work");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    // Expected output: Specifiers, return type, and parameter list are output in order.
    std::string expected =
        "    /**\n"
        "     * @brief Does work\n"
        "     */\n"
        "    inline constexpr void doWork(const float param1, int param2);\n";
    
    EXPECT_EQ(generated, expected);
}
