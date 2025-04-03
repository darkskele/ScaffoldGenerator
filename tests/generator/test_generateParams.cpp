#include "gtest/gtest.h"
#include "PropertiesGenerator.h"
#include "PropertiesModels.h"
#include <vector>
#include <string>

// Using the PropertiesGenerator namespace.
using namespace PropertiesGenerator;

// Test: When the parameter vector is empty, the generated string should be empty.
TEST(GenerateParameterListTest, EmptyParameters) {
    std::vector<PropertiesModels::Parameter> params;
    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "");
}

// Test: For a single parameter of type int with name "x", expect "int x".
TEST(GenerateParameterListTest, SingleParameter) {
    // Create a parameter "x" of type int.
    PropertiesModels::DataType intType(PropertiesModels::Types::INT);
    PropertiesModels::Parameter param(intType, "x");
    std::vector<PropertiesModels::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int x");
}

// Test: For multiple parameters, the output should correctly separate each parameter with a comma.
TEST(GenerateParameterListTest, MultipleParameters) {
    // Create two parameters: "x" of type int and "y" of type float.
    PropertiesModels::DataType intType(PropertiesModels::Types::INT);
    PropertiesModels::DataType floatType(PropertiesModels::Types::FLOAT);
    PropertiesModels::Parameter param1(intType, "x");
    PropertiesModels::Parameter param2(floatType, "y");
    std::vector<PropertiesModels::Parameter> params = { param1, param2 };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int x, float y");
}

// Test: For a custom type parameter, the generator should output the custom type name.
TEST(GenerateParameterListTest, CustomTypeParameter) {
    // Create a parameter "customParam" with a custom type "MyType".
    PropertiesModels::DataType customType(PropertiesModels::Types::CUSTOM, std::string("MyType"), PropertiesModels::TypeQualifier::NONE);
    PropertiesModels::Parameter param(customType, "customParam");
    std::vector<PropertiesModels::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "MyType customParam");
}

// Test: For a parameter declared as a pointer (int*), the pointer symbol should appear.
TEST(GenerateParameterListTest, PointerParameter) {
    // Create a parameter "ptr" of type "int*".
    PropertiesModels::DataType intType(PropertiesModels::Types::INT);
    intType.typeDecl.ptrCount = 1;  // Set pointer count to 1.
    PropertiesModels::Parameter param(intType, "ptr");
    std::vector<PropertiesModels::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int* ptr");
}

// Test: For a parameter declared as an lvalue reference (int&), the '&' should appear.
TEST(GenerateParameterListTest, LValueReferenceParameter) {
    // Create a parameter "ref" of type "int&".
    PropertiesModels::DataType intType(PropertiesModels::Types::INT);
    intType.typeDecl.isLValReference = true;
    PropertiesModels::Parameter param(intType, "ref");
    std::vector<PropertiesModels::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int& ref");
}

// Test: For a parameter declared as an rvalue reference (int&&), the "&&" should appear.
TEST(GenerateParameterListTest, RValueReferenceParameter) {
    // Create a parameter "temp" of type "int&&".
    PropertiesModels::DataType intType(PropertiesModels::Types::INT);
    intType.typeDecl.isRValReference = true;
    PropertiesModels::Parameter param(intType, "temp");
    std::vector<PropertiesModels::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int&& temp");
}

// Test: For a parameter combining pointer and array declarators (int*[5]), expect both to be included.
TEST(GenerateParameterListTest, PointerAndArrayParameter) {
    // Create a parameter "data" of type "int*[5]".
    PropertiesModels::DataType intType(PropertiesModels::Types::INT);
    intType.typeDecl.ptrCount = 1;
    intType.typeDecl.arrayDimensions.push_back("5");
    PropertiesModels::Parameter param(intType, "data");
    std::vector<PropertiesModels::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "int*[5] data");
}

// Test: For a complex parameter with a qualifier, pointer, and array declarators (const char*[10]), expect all elements to be output.
TEST(GenerateParameterListTest, ComplexParameter) {
    // Create a parameter "buf" of type "const char*[10]".
    PropertiesModels::DataType charType(PropertiesModels::Types::CHAR, PropertiesModels::TypeQualifier::CONST);
    charType.typeDecl.ptrCount = 1;
    charType.typeDecl.arrayDimensions.push_back("10");
    PropertiesModels::Parameter param(charType, "buf");
    std::vector<PropertiesModels::Parameter> params = { param };

    std::string result = generateParameterList(params);
    EXPECT_EQ(result, "const char*[10] buf");
}
