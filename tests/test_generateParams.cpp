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
