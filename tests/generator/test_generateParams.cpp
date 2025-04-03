#include "gtest/gtest.h"
#include "PropertiesGenerator.h"
#include "ScaffoldProperties.h"
#include <vector>
#include <string>

// Using the PropertiesGenerator namespace.
using namespace PropertiesGenerator;

// Test: When the parameter vector is empty, the generated string should be empty.
TEST(GenerateParameterListTest, EmptyParameters) {
    std::vector<ScaffoldProperties::Parameter> params;
    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "");
}

// Test: For a single parameter of type int with name "x", expect "int x".
TEST(GenerateParameterListTest, SingleParameter) {
    // Create a parameter "x" of type int.
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    ScaffoldProperties::Parameter param(intType, "x");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int x");
}

// Test: For multiple parameters, the output should correctly separate each parameter with a comma.
TEST(GenerateParameterListTest, MultipleParameters) {
    // Create two parameters: "x" of type int and "y" of type float.
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    ScaffoldProperties::DataType floatType(ScaffoldProperties::Types::FLOAT);
    ScaffoldProperties::Parameter param1(intType, "x");
    ScaffoldProperties::Parameter param2(floatType, "y");
    std::vector<ScaffoldProperties::Parameter> params = { param1, param2 };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int x, float y");
}

// Test: For a custom type parameter, the generator should output the custom type name.
TEST(GenerateParameterListTest, CustomTypeParameter) {
    // Create a parameter "customParam" with a custom type "MyType".
    ScaffoldProperties::DataType customType(ScaffoldProperties::Types::CUSTOM, std::string("MyType"), ScaffoldProperties::TypeQualifier::NONE);
    ScaffoldProperties::Parameter param(customType, "customParam");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "MyType customParam");
}

// Test: For a parameter declared as a pointer (int*), the pointer symbol should appear.
TEST(GenerateParameterListTest, PointerParameter) {
    // Create a parameter "ptr" of type "int*".
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    intType.typeDecl.ptrCount = 1;  // Set pointer count to 1.
    ScaffoldProperties::Parameter param(intType, "ptr");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int* ptr");
}

// Test: For a parameter declared as an lvalue reference (int&), the '&' should appear.
TEST(GenerateParameterListTest, LValueReferenceParameter) {
    // Create a parameter "ref" of type "int&".
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    intType.typeDecl.isLValReference = true;
    ScaffoldProperties::Parameter param(intType, "ref");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int& ref");
}

// Test: For a parameter declared as an rvalue reference (int&&), the "&&" should appear.
TEST(GenerateParameterListTest, RValueReferenceParameter) {
    // Create a parameter "temp" of type "int&&".
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    intType.typeDecl.isRValReference = true;
    ScaffoldProperties::Parameter param(intType, "temp");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int&& temp");
}

// Test: For a parameter combining pointer and array declarators (int*[5]), expect both to be included.
TEST(GenerateParameterListTest, PointerAndArrayParameter) {
    // Create a parameter "data" of type "int*[5]".
    ScaffoldProperties::DataType intType(ScaffoldProperties::Types::INT);
    intType.typeDecl.ptrCount = 1;
    intType.typeDecl.arrayDimensions.push_back("5");
    ScaffoldProperties::Parameter param(intType, "data");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int*[5] data");
}

// Test: For a complex parameter with a qualifier, pointer, and array declarators (const char*[10]), expect all elements to be output.
TEST(GenerateParameterListTest, ComplexParameter) {
    // Create a parameter "buf" of type "const char*[10]".
    ScaffoldProperties::DataType charType(ScaffoldProperties::Types::CHAR, ScaffoldProperties::TypeQualifier::CONST);
    charType.typeDecl.ptrCount = 1;
    charType.typeDecl.arrayDimensions.push_back("10");
    ScaffoldProperties::Parameter param(charType, "buf");
    std::vector<ScaffoldProperties::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "const char*[10] buf");
}
