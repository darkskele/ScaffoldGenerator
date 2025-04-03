#include <gtest/gtest.h>
#include "ClassGenerator.h"
#include "ScaffoldModels.h"
#include "ScaffoldProperties.h"
#include <string>
#include <vector>
#include <optional>

// Helper functions to create empty vectors.
inline std::vector<ScaffoldModels::MethodModel> makeEmptyMethods() {
    return std::vector<ScaffoldModels::MethodModel>{};
}

inline std::vector<ScaffoldProperties::Parameter> makeEmptyMembers() {
    return std::vector<ScaffoldProperties::Parameter>{};
}

inline std::vector<ScaffoldModels::Constructor> makeEmptyCtors() {
    return std::vector<ScaffoldModels::Constructor>{};
}

// Test: Minimal class definition should produce an empty string.
TEST(ClassGeneratorDefinitionTest, MinimalClassDefinition)
{
    ScaffoldModels::ClassModel cl(
        "EmptyClass",             // name
        "",                       // description
        makeEmptyCtors(),         // constructors
        std::nullopt,             // destructor
        makeEmptyMethods(),       // public methods
        makeEmptyMethods(),       // private methods
        makeEmptyMethods(),       // protected methods
        makeEmptyMembers(),       // public members
        makeEmptyMembers(),       // private members
        makeEmptyMembers(),       // protected members
        false,                    // hasCopyAssignment
        false                     // hasMoveAssignment
    );
    
    std::string expected = "";
    std::string output = ClassGenerator::generateClassDefinition(cl);
    EXPECT_EQ(output, expected);
}

// Test: Public class with one custom constructor and one public method.
TEST(ClassGeneratorDefinitionTest, PublicClassWithMembersMethodsAndConstructors)
{
    // Create one custom constructor with one parameter.
    ScaffoldProperties::Parameter param(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::INT), "id");
    std::vector<ScaffoldModels::Constructor> ctors = {
        ScaffoldModels::Constructor(ScaffoldModels::ConstructorType::CUSTOM, { param }, "Custom constructor")
    };

    // Create one public method.
    ScaffoldModels::MethodModel pubMethod(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::VOID),
        "publicMethod",
        std::vector<ScaffoldProperties::Parameter>{},
        ScaffoldProperties::DeclartionSpecifier(),
        "Public method"
    );
    std::vector<ScaffoldModels::MethodModel> pubMethods = { pubMethod };

    // No destructor or assignments.
    ScaffoldModels::ClassModel cl(
        "TestClass",                // name
        "Test class",               // description
        ctors,                      // constructors
        std::nullopt,               // destructor
        pubMethods,                 // public methods
        makeEmptyMethods(),         // private methods
        makeEmptyMethods(),         // protected methods
        makeEmptyMembers(),         // public members
        makeEmptyMembers(),         // private members
        makeEmptyMembers(),         // protected members
        false,                      // hasCopyAssignment
        false                       // hasMoveAssignment
    );

    // Expected output: note that the custom constructor definition preserves the generator’s spacing.
    std::string expected = R"expected(TestClass::TestClass(int id)
{
}

void TestClass::publicMethod() {
    throw std::runtime_error("Not implemented");
}

)expected";

    std::string output = ClassGenerator::generateClassDefinition(cl);
    EXPECT_EQ(output, expected);
}

// Test: Class with destructor and both assignment operators.
TEST(ClassGeneratorDefinitionTest, ClassWithDestructorAndAssignments)
{
    ScaffoldModels::Destructor dtor("Default destructor");
    // When assignment flags are true, definitions for both assignments are generated.
    ScaffoldModels::ClassModel cl(
        "DestructClass",
        "Class with destructor and assignments",
        makeEmptyCtors(),         // no constructors
        dtor,                     // destructor provided (default destructor => no definition)
        makeEmptyMethods(),       // public methods
        makeEmptyMethods(),       // private methods
        makeEmptyMethods(),       // protected methods
        makeEmptyMembers(),       // public members
        makeEmptyMembers(),       // private members
        makeEmptyMembers(),       // protected members
        true,                     // has copy assignment
        true                      // has move assignment
    );

    std::string expected = R"expected(DestructClass& DestructClass::operator=(const DestructClass& other) {
    // Placeholder implementation: copy assignment operator is not implemented.
    throw std::runtime_error("Not implemented");
}

DestructClass& DestructClass::operator=(DestructClass&& other) noexcept {
    // Placeholder implementation: move assignment operator is not implemented.
    throw std::runtime_error("Not implemented");
}

)expected";

    std::string output = ClassGenerator::generateClassDefinition(cl);
    EXPECT_EQ(output, expected);
}

// Test: Class with private and protected method definitions.
TEST(ClassGeneratorDefinitionTest, ClassWithPrivateAndProtectedSections)
{
    // Public: one method.
    ScaffoldModels::MethodModel pubMethod(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::VOID),
        "publicMethod",
        std::vector<ScaffoldProperties::Parameter>{},
        ScaffoldProperties::DeclartionSpecifier(),
        "Public method"
    );
    std::vector<ScaffoldModels::MethodModel> pubMethods = { pubMethod };

    // Private: one method.
    ScaffoldModels::MethodModel privMethod(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::VOID),
        "privateMethod",
        std::vector<ScaffoldProperties::Parameter>{},
        ScaffoldProperties::DeclartionSpecifier(),
        "Private method"
    );
    std::vector<ScaffoldModels::MethodModel> privMethods = { privMethod };

    // Protected: one method.
    ScaffoldModels::MethodModel protMethod(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::VOID),
        "protectedMethod",
        std::vector<ScaffoldProperties::Parameter>{},
        ScaffoldProperties::DeclartionSpecifier(),
        "Protected method"
    );
    std::vector<ScaffoldModels::MethodModel> protMethods = { protMethod };

    ScaffoldModels::ClassModel cl(
        "ComplexClass",
        "A class with private and protected methods",
        makeEmptyCtors(),        // no constructors
        std::nullopt,            // no destructor
        pubMethods,              // public methods
        privMethods,             // private methods
        protMethods,             // protected methods
        makeEmptyMembers(),      // public members
        makeEmptyMembers(),      // private members
        makeEmptyMembers(),      // protected members
        false,
        false
    );

    // Note: Our generator does not prepend a return type if declSpec is empty.
    std::string expected = R"expected(void ComplexClass::publicMethod() {
    throw std::runtime_error("Not implemented");
}

void ComplexClass::privateMethod() {
    throw std::runtime_error("Not implemented");
}

void ComplexClass::protectedMethod() {
    throw std::runtime_error("Not implemented");
}

)expected";

    std::string output = ClassGenerator::generateClassDefinition(cl);
    EXPECT_EQ(output, expected);
}

// Test: Fully featured class with all sections.
TEST(ClassGeneratorDefinitionTest, ClassWithAllSectionsAndSpacing)
{
    // Arrange:
    // Public: one custom constructor, one public method, one public member.
    ScaffoldProperties::Parameter ctorParam(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::INT), "id");
    std::vector<ScaffoldModels::Constructor> ctors = {
        ScaffoldModels::Constructor(ScaffoldModels::ConstructorType::CUSTOM, { ctorParam }, "Custom constructor")
    };

    ScaffoldModels::MethodModel pubMethod(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::VOID),
        "publicMethod",
        std::vector<ScaffoldProperties::Parameter>{},
        ScaffoldProperties::DeclartionSpecifier(),
        "Public method"
    );
    ScaffoldProperties::Parameter pubMember(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::FLOAT), "value");

    // Private: one method.
    ScaffoldModels::MethodModel privMethod(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::VOID),
        "privateHelper",
        std::vector<ScaffoldProperties::Parameter>{},
        ScaffoldProperties::DeclartionSpecifier(),
        "Private helper"
    );

    // Protected: one member.
    ScaffoldProperties::Parameter protMember(
        ScaffoldProperties::DataType(ScaffoldProperties::Types::INT), "counter");

    ScaffoldModels::ClassModel cl(
        "FullClass",
        "Fully featured class",
        ctors,
        ScaffoldModels::Destructor("Default destructor"),
        std::vector<ScaffoldModels::MethodModel>{ pubMethod },
        std::vector<ScaffoldModels::MethodModel>{ privMethod },
        makeEmptyMethods(),  // no protected methods
        std::vector<ScaffoldProperties::Parameter>{ pubMember },
        makeEmptyMembers(),  // no private members
        std::vector<ScaffoldProperties::Parameter>{ protMember },
        true,
        true
    );

    std::string expected = R"expected(FullClass::FullClass(int id) : value(), counter()
{
}

FullClass& FullClass::operator=(const FullClass& other) {
    // Placeholder implementation: copy assignment operator is not implemented.
    throw std::runtime_error("Not implemented");
}

FullClass& FullClass::operator=(FullClass&& other) noexcept {
    // Placeholder implementation: move assignment operator is not implemented.
    throw std::runtime_error("Not implemented");
}

void FullClass::publicMethod() {
    throw std::runtime_error("Not implemented");
}

void FullClass::privateHelper() {
    throw std::runtime_error("Not implemented");
}

)expected";

    std::string output = ClassGenerator::generateClassDefinition(cl);
    EXPECT_EQ(output, expected);
}
