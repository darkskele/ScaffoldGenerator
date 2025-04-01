#include "gtest/gtest.h"
#include "PropertiesGenerator.h"
#include "ScaffoldProperties.h"
#include <vector>
#include <string>

// Using the PropertiesGenerator namespace.
using namespace PropertiesGenerator;

// Test for an empty parameter vector.
TEST(GenerateParameterListTest, EmptyParameters) {
    std::vector<ScaffoldProperties::Parameter> params;
    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "");
}

// Test for a single parameter.
TEST(GenerateParameterListTest, SingleParameter) {
    // Create a parameter "x" of type int.
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    ScaffoldProperties::Parameter param(intType, "x");
    std::vector<ScaffoldProperties::Parameter> params = { param };
    
    std::string result = generateParameterList(params);
    // Expect the output to be "int x" if GeneratorUtilities::dataTypeToString returns "int"
    EXPECT_EQ(result, "int x");
}

// Test for multiple parameters.
TEST(GenerateParameterListTest, MultipleParameters) {
    // Create two parameters: "x" of type int and "y" of type float.
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    ScaffoldProperties::DataType floatType(ScaffoldProperties::Types::FLOAT);
    ScaffoldProperties::Parameter param1(intType, "x");
    ScaffoldProperties::Parameter param2(floatType, "y");
    std::vector<ScaffoldProperties::Parameter> params = { param1, param2 };
    
    std::string result = generateParameterList(params);
    // Expect the output to be "int x, float y"
    EXPECT_EQ(result, "int x, float y");
}

// Test for a custom type parameter.
TEST(GenerateParameterListTest, CustomTypeParameter) {
    // Create a parameter "customParam" with type CUSTOM and custom type name "MyType".
    ScaffoldProperties::DataType customType(ScaffoldProperties::Types::CUSTOM, std::string("MyType"), ScaffoldProperties::TypeQualifier::NONE);
    ScaffoldProperties::Parameter param(customType, "customParam");
    std::vector<ScaffoldProperties::Parameter> params = { param };
    
    std::string result = generateParameterList(params);
    // Expect the output to be "MyType customParam"
    EXPECT_EQ(result, "MyType customParam");
}

// Test: Parameter with a pointer declarator.
TEST(GenerateParameterListTest, PointerParameter) {
    // Create a parameter "ptr" of type "int*".
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    // Manually set pointer count in the type declaration.
    intType.typeDecl.ptrCount = 1;
    ScaffoldProperties::Parameter param(intType, "ptr");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    // Expected output: "int* ptr"
    EXPECT_EQ(result, "int* ptr");
}

// Test: Parameter with an lvalue reference declarator.
TEST(GenerateParameterListTest, LValueReferenceParameter) {
    // Create a parameter "ref" of type "int&".
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    intType.typeDecl.isLValReference = true;
    ScaffoldProperties::Parameter param(intType, "ref");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    // Expected output: "int& ref"
    EXPECT_EQ(result, "int& ref");
}

// Test: Parameter with an rvalue reference declarator.
TEST(GenerateParameterListTest, RValueReferenceParameter) {
    // Create a parameter "temp" of type "int&&".
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    intType.typeDecl.isRValReference = true;
    ScaffoldProperties::Parameter param(intType, "temp");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    // Expected output: "int&& temp"
    EXPECT_EQ(result, "int&& temp");
}

// Test: Parameter with a combination of pointer and array declarators.
TEST(GenerateParameterListTest, PointerAndArrayParameter) {
    // Create a parameter "data" of type "int*[5]".
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    intType.typeDecl.ptrCount = 1;
    // Assume arrayDimensions is a vector of strings and we push "5".
    intType.typeDecl.arrayDimensions.push_back("5");
    ScaffoldProperties::Parameter param(intType, "data");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    // Expected output: "int*[5] data"
    EXPECT_EQ(result, "int*[5] data");
}

// Test: Parameter with pointer, qualifier, and array.
TEST(GenerateParameterListTest, ComplexParameter) {
    // Create a parameter "buf" of type "const char*[10]".
    ScaffoldProperties::DataType charType(ScaffoldProperties::Types::CHAR, ScaffoldProperties::TypeQualifier::CONST);
    charType.typeDecl.ptrCount = 1;
    charType.typeDecl.arrayDimensions.push_back("10");
    ScaffoldProperties::Parameter param(charType, "buf");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    // Expected output: "const char*[10] buf"
    EXPECT_EQ(result, "const char*[10] buf");
}

