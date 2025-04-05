#include <gtest/gtest.h>
#include "ClassGenerator.h"
#include "ClassModels.h"
#include "PropertiesModels.h"
#include <string>
#include <vector>
#include <optional>

// Helper functions to create empty vectors.
inline std::vector<CallableModels::MethodModel> makeEmptyMethods() {
    return std::vector<CallableModels::MethodModel>{};
}

inline std::vector<PropertiesModels::Parameter> makeEmptyMembers() {
    return std::vector<PropertiesModels::Parameter>{};
}

inline std::vector<ClassModels::Constructor> makeEmptyCtors() {
    return std::vector<ClassModels::Constructor>{};
}

// Test: Minimal class definition should produce an empty string.
TEST(ClassGeneratorDefinitionTest, MinimalClassDefinition)
{
    ClassModels::ClassModel cl(
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
    PropertiesModels::Parameter param(
        PropertiesModels::DataType(PropertiesModels::Types::INT), "id");
    std::vector<ClassModels::Constructor> ctors = {
        ClassModels::Constructor(ClassModels::ConstructorType::CUSTOM, { param }, "Custom constructor")
    };

    // Create one public method.
    CallableModels::MethodModel pubMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "publicMethod",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "Public method"
    );
    std::vector<CallableModels::MethodModel> pubMethods = { pubMethod };

    // No destructor or assignments.
    ClassModels::ClassModel cl(
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
    // TODO: Implement TestClass construtor logic.
    throw std::runtime_error("Not implemented");
}

void TestClass::publicMethod() {
    // TODO: Implement publicMethod logic.
    throw std::runtime_error("Not implemented");
}

)expected";

    std::string output = ClassGenerator::generateClassDefinition(cl);
    EXPECT_EQ(output, expected);
}

// Test: Class with destructor and both assignment operators.
TEST(ClassGeneratorDefinitionTest, ClassWithDestructorAndAssignments)
{
    ClassModels::Destructor dtor("Default destructor");
    // When assignment flags are true, definitions for both assignments are generated.
    ClassModels::ClassModel cl(
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
    // TODO: Implement DestructClass copy assignment logic.
    throw std::runtime_error("Not implemented");
}

DestructClass& DestructClass::operator=(DestructClass&& other) noexcept {
    // TODO: Implement DestructClass move assignment logic.
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
    CallableModels::MethodModel pubMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "publicMethod",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "Public method"
    );
    std::vector<CallableModels::MethodModel> pubMethods = { pubMethod };

    // Private: one method.
    CallableModels::MethodModel privMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "privateMethod",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "Private method"
    );
    std::vector<CallableModels::MethodModel> privMethods = { privMethod };

    // Protected: one method.
    CallableModels::MethodModel protMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "protectedMethod",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "Protected method"
    );
    std::vector<CallableModels::MethodModel> protMethods = { protMethod };

    ClassModels::ClassModel cl(
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
    // TODO: Implement publicMethod logic.
    throw std::runtime_error("Not implemented");
}

void ComplexClass::privateMethod() {
    // TODO: Implement privateMethod logic.
    throw std::runtime_error("Not implemented");
}

void ComplexClass::protectedMethod() {
    // TODO: Implement protectedMethod logic.
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
    PropertiesModels::Parameter ctorParam(
        PropertiesModels::DataType(PropertiesModels::Types::INT), "id");
    std::vector<ClassModels::Constructor> ctors = {
        ClassModels::Constructor(ClassModels::ConstructorType::CUSTOM, { ctorParam }, "Custom constructor")
    };

    CallableModels::MethodModel pubMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "publicMethod",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "Public method"
    );
    PropertiesModels::Parameter pubMember(
        PropertiesModels::DataType(PropertiesModels::Types::FLOAT), "value");

    // Private: one method.
    CallableModels::MethodModel privMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "privateHelper",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "Private helper"
    );

    // Protected: one member.
    PropertiesModels::Parameter protMember(
        PropertiesModels::DataType(PropertiesModels::Types::INT), "counter");

    ClassModels::ClassModel cl(
        "FullClass",
        "Fully featured class",
        ctors,
        ClassModels::Destructor("Default destructor"),
        std::vector<CallableModels::MethodModel>{ pubMethod },
        std::vector<CallableModels::MethodModel>{ privMethod },
        makeEmptyMethods(),  // no protected methods
        std::vector<PropertiesModels::Parameter>{ pubMember },
        makeEmptyMembers(),  // no private members
        std::vector<PropertiesModels::Parameter>{ protMember },
        true,
        true
    );

    std::string expected =
    "FullClass::FullClass(int id) : value(), counter()\n"
    "{\n"
    "    // TODO: Implement FullClass construtor logic.\n"
    "    throw std::runtime_error(\"Not implemented\");\n"
    "}\n\n"
    "FullClass& FullClass::operator=(const FullClass& other) {\n"
    "    // TODO: Implement FullClass copy assignment logic.\n"
    "    throw std::runtime_error(\"Not implemented\");\n"
    "}\n\n"
    "FullClass& FullClass::operator=(FullClass&& other) noexcept {\n"
    "    // TODO: Implement FullClass move assignment logic.\n"
    "    throw std::runtime_error(\"Not implemented\");\n"
    "}\n\n"
    "void FullClass::publicMethod() {\n"
    "    // TODO: Implement publicMethod logic.\n"
    "    throw std::runtime_error(\"Not implemented\");\n"
    "}\n\n"
    "void FullClass::privateHelper() {\n"
    "    // TODO: Implement privateHelper logic.\n"
    "    throw std::runtime_error(\"Not implemented\");\n"
    "}\n\n";


    std::string output = ClassGenerator::generateClassDefinition(cl);
    EXPECT_EQ(output, expected);
}
