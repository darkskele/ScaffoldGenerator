#include <gtest/gtest.h>
#include "SpecialMemberGenerator.h"
#include <string>

// Test for generating the default destructor declaration.
// Expected output: "    ~MyClass() = default;"
TEST(DestructorGeneratorTest, GenerateDefaultDestructorDeclaration) {
    std::string className = "MyClass";
    std::string expected = "    ~MyClass() = default;";
    
    // Call the generator function.
    std::string output = SpecialMemberGenerator::generateDestructorDeclaration(className);
    
    // Verify that the generated string exactly matches the expected declaration.
    EXPECT_EQ(output, expected);
}

// Test for generating the default destructor definition.
// For default destructors, no out-of-line definition is needed, so we expect an empty string.
TEST(DestructorGeneratorTest, GenerateDefaultDestructorDefinition) {
    std::string className = "MyClass";
    std::string expected = "";
    
    // Call the generator function.
    std::string output = SpecialMemberGenerator::generateDestructorDefinition(className);
    
    // Verify that the generated definition is an empty string.
    EXPECT_EQ(output, expected);
}
