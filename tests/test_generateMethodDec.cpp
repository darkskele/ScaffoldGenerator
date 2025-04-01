#include "gtest/gtest.h"
#include "MethodGenerator.h"
#include "ScaffoldModels.h"
#include "ScaffoldProperties.h"
#include <vector>
#include <string>

using namespace ScaffoldProperties;
using namespace ScaffoldModels;

// Test: Method with no parameters.
TEST(GenerateMethodDeclarationTest, NoParameters) {
    // Create a MethodModel with return type "int", no parameters, and a description.
    DataType returnType(Types::INT);
    std::vector<Parameter> params; // Empty parameters
    MethodModel method(returnType, "doSomething", params, "Performs a calculation");
    
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
    
    MethodModel method(returnType, "doSomething", params, "Does something");
    
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
    
    // Description intentionally has leading/trailing spaces.
    MethodModel method(returnType, "compute", params, "  Computes a value  ");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    // Expected: The description is output exactly as provided.
    std::string expected =
        "    /**\n"
        "     * @brief   Computes a value  \n"
        "     */\n"
        "    double compute(int x);\n";
    
    EXPECT_EQ(generated, expected);
}

TEST(GenerateMethodDeclarationTest, ReturnTypeWithConstQualifier) {
    // Method with a return type "const int".
    DataType returnType(Types::INT, TypeQualifier::CONST);
    std::vector<Parameter> params; // No parameters.
    MethodModel method(returnType, "doSomething", params, "Returns a constant int");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Returns a constant int\n"
        "     */\n"
        "    const int doSomething();\n";
    
    EXPECT_EQ(generated, expected);
}

TEST(GenerateMethodDeclarationTest, ParameterWithConstQualifier) {
    // Method with one parameter that is const-qualified.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::FLOAT, TypeQualifier::CONST), "param1"));
    
    MethodModel method(returnType, "doSomething", params, "Takes a constant float parameter");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Takes a constant float parameter\n"
        "     */\n"
        "    void doSomething(const float param1);\n";
    
    EXPECT_EQ(generated, expected);
}

TEST(GenerateMethodDeclarationTest, ParameterWithConstVolatileQualifiers) {
    // Method with one parameter that is both const and volatile.
    DataType returnType(Types::VOID);
    std::vector<Parameter> params;
    params.emplace_back(Parameter(DataType(Types::INT, TypeQualifier::CONST | TypeQualifier::VOLATILE), "param1"));
    
    MethodModel method(returnType, "doSomething", params, "Takes a const volatile int parameter");
    
    std::string generated = MethodGenerator::generateMethodDeclaration(method);
    
    std::string expected =
        "    /**\n"
        "     * @brief Takes a const volatile int parameter\n"
        "     */\n"
        "    void doSomething(const volatile int param1);\n";
    
    EXPECT_EQ(generated, expected);
}
