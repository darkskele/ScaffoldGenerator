#include <gtest/gtest.h>
#include "SpecialMemberGenerator.h"
#include "ClassModels.h"
#include "PropertiesModels.h"
#include "PropertiesGenerator.h"
#include <stdexcept>
#include <vector>

TEST(SpecialMemberGeneratorDefinitionTest, CustomConstructorDefinitionWithMembers) {
    // Arrange: Create a custom constructor with parameters.
    std::string className = "MyClass";
    std::vector<PropertiesModels::Parameter> customParams = {
        // Construct parameters using the DataType and Parameter constructors.
        PropertiesModels::Parameter(PropertiesModels::DataType(PropertiesModels::Types::INT), "param1"),
        PropertiesModels::Parameter(PropertiesModels::DataType(PropertiesModels::Types::FLOAT), "param2")
    };
    // Create a custom constructor model.
    ClassModels::Constructor ctor(ClassModels::ConstructorType::CUSTOM, customParams, "Custom constructor");

    // Provide member parameters to be initialized in the initializer list.
    std::vector<PropertiesModels::Parameter> publicMembers = {
        PropertiesModels::Parameter(PropertiesModels::DataType(PropertiesModels::Types::INT), "a")
    };
    std::vector<PropertiesModels::Parameter> privateMembers = {
        PropertiesModels::Parameter(PropertiesModels::DataType(PropertiesModels::Types::INT), "b")
    };
    std::vector<PropertiesModels::Parameter> protectedMembers = {
        PropertiesModels::Parameter(PropertiesModels::DataType(PropertiesModels::Types::INT), "c")
    };

    // Act: Generate the constructor definition.
    std::string def = SpecialMemberGenerator::generateConstructorDefinition(className, ctor,
        publicMembers, privateMembers, protectedMembers);

    // Assert:
    // For custom constructors, we expect the definition to begin with:
    // "MyClass::MyClass(" + PropertiesGenerator::generateParameterList(customParams) + ") : "
    // followed by the initializer list of members separated by commas and then a body.
    std::string paramList = PropertiesGenerator::generateParameterList(customParams);
    std::string expected = "MyClass::MyClass(" + paramList + ") : a(), b(), c()\n{\n}\n";
    EXPECT_EQ(def, expected);
}

TEST(SpecialMemberGeneratorDefinitionTest, CopyConstructorDefinitionWithMembers) {
    // Arrange: Create a copy constructor model.
    std::string className = "MyClass";
    ClassModels::Constructor ctor(ClassModels::ConstructorType::COPY, {}, "Copy constructor");

    // Provide a single public member for initialization.
    std::vector<PropertiesModels::Parameter> publicMembers = {
        PropertiesModels::Parameter(PropertiesModels::DataType(PropertiesModels::Types::INT), "x")
    };
    std::vector<PropertiesModels::Parameter> privateMembers; // empty
    std::vector<PropertiesModels::Parameter> protectedMembers; // empty

    // Act: Generate the copy constructor definition.
    std::string def = SpecialMemberGenerator::generateConstructorDefinition(className, ctor,
        publicMembers, privateMembers, protectedMembers);

    // Assert:
    // Expected signature for copy constructor plus initializer list for the member "x".
    std::string expected = "MyClass::MyClass(const MyClass& other) : x()\n{\n}\n";
    EXPECT_EQ(def, expected);
}

TEST(SpecialMemberGeneratorDefinitionTest, MoveConstructorDefinitionWithMembers) {
    // Arrange: Create a move constructor model.
    std::string className = "MyClass";
    ClassModels::Constructor ctor(ClassModels::ConstructorType::MOVE, {}, "Move constructor");

    // Provide a single public member for initialization.
    std::vector<PropertiesModels::Parameter> publicMembers = {
        PropertiesModels::Parameter(PropertiesModels::DataType(PropertiesModels::Types::INT), "x")
    };
    std::vector<PropertiesModels::Parameter> privateMembers; // empty
    std::vector<PropertiesModels::Parameter> protectedMembers; // empty

    // Act: Generate the move constructor definition.
    std::string def = SpecialMemberGenerator::generateConstructorDefinition(className, ctor,
        publicMembers, privateMembers, protectedMembers);

    // Assert:
    // Expected signature for move constructor plus initializer list for the member "x".
    std::string expected = "MyClass::MyClass(MyClass&& other) noexcept : x()\n{\n}\n";
    EXPECT_EQ(def, expected);
}

TEST(SpecialMemberGeneratorDefinitionTest, DefaultConstructorDefinitionReturnsEmpty) {
    // Arrange: Create a default constructor model.
    std::string className = "MyClass";
    ClassModels::Constructor ctor(ClassModels::ConstructorType::DEFAULT, {}, "Default constructor");

    // For default constructors, the out-of-line definition is auto-generated by the compiler.
    // Provide empty member lists.
    std::vector<PropertiesModels::Parameter> publicMembers;
    std::vector<PropertiesModels::Parameter> privateMembers;
    std::vector<PropertiesModels::Parameter> protectedMembers;

    // Act: Generate the constructor definition.
    std::string def = SpecialMemberGenerator::generateConstructorDefinition(className, ctor,
        publicMembers, privateMembers, protectedMembers);

    // Assert: The generated definition should be an empty string.
    EXPECT_EQ(def, "");
}

TEST(SpecialMemberGeneratorDefinitionTest, ThrowsOnUnknownConstructorType) {
    // Arrange: Create a constructor model with an invalid constructor type.
    std::string className = "MyClass";
    ClassModels::Constructor invalidCtor(static_cast<ClassModels::ConstructorType>(999), {}, "Invalid constructor");

    std::vector<PropertiesModels::Parameter> publicMembers;
    std::vector<PropertiesModels::Parameter> privateMembers;
    std::vector<PropertiesModels::Parameter> protectedMembers;

    // Act & Assert: Expect a runtime_error to be thrown for the unrecognised constructor type.
    EXPECT_THROW(
        SpecialMemberGenerator::generateConstructorDefinition(className, invalidCtor,
            publicMembers, privateMembers, protectedMembers),
        std::runtime_error
    );
}
