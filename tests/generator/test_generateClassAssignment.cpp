#include <gtest/gtest.h>
#include "SpecialMemberGenerator.h"
#include <string>

// Test for generating the move assignment declaration.
// Expected output: "MyClass& operator=(MyClass&& other) noexcept;"
TEST(AssignmentOperatorGeneratorTest, GenerateMoveAssignmentDeclaration) {
    std::string className = "MyClass";
    std::string expected = "    MyClass& operator=(MyClass&& other) noexcept;";
    
    // Call the generator function.
    std::string output = SpecialMemberGenerator::generateMoveAssignmentDeclaration(className);
    
    // Verify the generated string matches the expected declaration.
    EXPECT_EQ(output, expected);
}

// Test for generating the move assignment definition.
// Expected output (with newlines and indentation exactly as generated):
// "MyClass& MyClass::operator=(MyClass&& other) noexcept {\n"
// "    // Placeholder implementation: move assignment operator is not implemented.\n"
// "    throw std::runtime_error(\"Not implemented\");\n"
// "}\n"
TEST(AssignmentOperatorGeneratorTest, GenerateMoveAssignmentDefinition) {
    std::string className = "MyClass";
    std::string expected = "MyClass& MyClass::operator=(MyClass&& other) noexcept {\n"
                           "    // Placeholder implementation: move assignment operator is not implemented.\n"
                           "    throw std::runtime_error(\"Not implemented\");\n"
                           "}\n";
    
    // Call the generator function.
    std::string output = SpecialMemberGenerator::generateMoveAssignmentDefinition(className);
    
    // Verify the generated string matches the expected definition.
    EXPECT_EQ(output, expected);
}

// Test for generating the copy assignment declaration.
// Expected output: "MyClass& operator=(const MyClass& other);"
TEST(AssignmentOperatorGeneratorTest, GenerateCopyAssignmentDeclaration) {
    std::string className = "MyClass";
    std::string expected = "    MyClass& operator=(const MyClass& other);";
    
    // Call the generator function.
    std::string output = SpecialMemberGenerator::generateCopyAssignmentDeclaration(className);
    
    // Verify the generated string matches the expected declaration.
    EXPECT_EQ(output, expected);
}

// Test for generating the copy assignment definition.
// Expected output (with newlines and indentation exactly as generated):
// "MyClass& MyClass::operator=(const MyClass& other) {\n"
// "    // Placeholder implementation: copy assignment operator is not implemented.\n"
// "    throw std::runtime_error(\"Not implemented\");\n"
// "}\n"
TEST(AssignmentOperatorGeneratorTest, GenerateCopyAssignmentDefinition) {
    std::string className = "MyClass";
    std::string expected = "MyClass& MyClass::operator=(const MyClass& other) {\n"
                           "    // Placeholder implementation: copy assignment operator is not implemented.\n"
                           "    throw std::runtime_error(\"Not implemented\");\n"
                           "}\n";
    
    // Call the generator function.
    std::string output = SpecialMemberGenerator::generateCopyAssignmentDefinition(className);
    
    // Verify the generated string matches the expected definition.
    EXPECT_EQ(output, expected);
}
