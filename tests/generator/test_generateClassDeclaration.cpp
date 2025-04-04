#include <gtest/gtest.h>
#include "ClassGenerator.h"
#include "ClassModels.h"
#include "PropertiesModels.h"
#include "testUtility.h"
#include <string>
#include <vector>
#include <optional>

// Test: Minimal class declaration (no constructors, destructor, methods, or members).
TEST(ClassGeneratorDeclarationTest, MinimalClassDeclaration)
{
    // Arrange: Construct a minimal ClassModel.
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

    std::string expected = R"expected(/**
 * @class EmptyClass
 * @brief 
 */
class EmptyClass {
public:
};
)expected";

    // Act:
    std::string output = ClassGenerator::generateClassDeclaration(cl);

    // Assert:
    EXPECT_EQ(output, expected);
}

// Test: Public class with one custom constructor, one public method, and one public member.
TEST(ClassGeneratorDeclarationTest, PublicClassWithMembersMethodsAndConstructors)
{
    // Arrange:
    // Create one custom constructor with one parameter.
    PropertiesModels::Parameter param(
        PropertiesModels::DataType(PropertiesModels::Types::INT), "id");
    std::vector<ClassModels::Constructor> ctors = {
        ClassModels::Constructor(ClassModels::ConstructorType::CUSTOM, { param }, "Custom constructor")
    };

    // Create one public method.
    CallableModels::MethodModel pubMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID), // return type
        "publicMethod",                         // name
        std::vector<PropertiesModels::Parameter>{}, // parameters (empty)
        PropertiesModels::DeclartionSpecifier(),      // declaration specifier
        "Public method"                         // description (used in Doxygen comment)
    );
    std::vector<CallableModels::MethodModel> pubMethods = { pubMethod };

    // One public member: a member of type FLOAT.
    PropertiesModels::Parameter pubMember(
        PropertiesModels::DataType(PropertiesModels::Types::FLOAT), "value");
    std::vector<PropertiesModels::Parameter> pubMembers = { pubMember };

    // No destructor or assignments (flags false).
    ClassModels::ClassModel cl(
        "TestClass",                // name
        "Test class",               // description
        ctors,                      // constructors
        std::nullopt,               // destructor
        pubMethods,                 // public methods
        makeEmptyMethods(),         // private methods
        makeEmptyMethods(),         // protected methods
        pubMembers,                 // public members
        makeEmptyMembers(),         // private members
        makeEmptyMembers(),         // protected members
        false,                      // hasCopyAssignment
        false                       // hasMoveAssignment
    );

    std::string expected = R"expected(/**
 * @class TestClass
 * @brief Test class
 */
class TestClass {
public:
    TestClass(int id);
    /**
     * @brief Public method
     */
    void publicMethod();
    float value;
};
)expected";

    // Act:
    std::string output = ClassGenerator::generateClassDeclaration(cl);

    // Assert:
    EXPECT_EQ(output, expected);
}

// Test: Class with private and protected sections.
TEST(ClassGeneratorDeclarationTest, ClassWithPrivateAndProtectedSections)
{
    // Arrange:
    // Public section: one method and one member.
    CallableModels::MethodModel pubMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "publicMethod",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "Public method"
    );
    PropertiesModels::Parameter pubMember(
        PropertiesModels::DataType(PropertiesModels::Types::INT), "publicMember");

    // Private section: one method and one member.
    CallableModels::MethodModel privMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "privateMethod",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "Private method"
    );
    PropertiesModels::Parameter privMember(
        PropertiesModels::DataType(PropertiesModels::Types::INT), "privateMember");

    // Protected section: one method and one member.
    CallableModels::MethodModel protMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "protectedMethod",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "Protected method"
    );
    PropertiesModels::Parameter protMember(
        PropertiesModels::DataType(PropertiesModels::Types::INT), "protectedMember");

    std::vector<CallableModels::MethodModel> pubMethods = { pubMethod };
    std::vector<PropertiesModels::Parameter> pubMembers = { pubMember };
    std::vector<CallableModels::MethodModel> privMethods = { privMethod };
    std::vector<PropertiesModels::Parameter> privMembers = { privMember };
    std::vector<CallableModels::MethodModel> protMethods = { protMethod };
    std::vector<PropertiesModels::Parameter> protMembers = { protMember };

    // Flags false so no assignment operators are output.
    ClassModels::ClassModel cl(
        "ComplexClass",
        "A class with multiple access specifiers",
        makeEmptyCtors(),          // no constructors
        std::nullopt,              // no destructor
        pubMethods,                // public methods
        privMethods,               // private methods
        protMethods,               // protected methods
        pubMembers,                // public members
        privMembers,               // private members
        protMembers,               // protected members
        false,
        false
    );

    std::string expected = R"expected(/**
 * @class ComplexClass
 * @brief A class with multiple access specifiers
 */
class ComplexClass {
public:
    /**
     * @brief Public method
     */
    void publicMethod();
    int publicMember;
private:
    /**
     * @brief Private method
     */
    void privateMethod();
    int privateMember;
protected:
    /**
     * @brief Protected method
     */
    void protectedMethod();
    int protectedMember;
};
)expected";

    // Act:
    std::string output = ClassGenerator::generateClassDeclaration(cl);

    // Assert:
    EXPECT_EQ(output, expected);
}

// Test: Class with destructor and assignment operators.
TEST(ClassGeneratorDeclarationTest, ClassWithDestructorAndAssignments)
{
    // Arrange:
    ClassModels::Destructor dtor("Default destructor");
    // Flags true so destructor and assignment operators appear.
    ClassModels::ClassModel cl(
        "DestructClass",
        "Class with destructor and assignments",
        makeEmptyCtors(),        // no constructors
        dtor,                    // destructor provided
        makeEmptyMethods(),      // public methods
        makeEmptyMethods(),      // private methods
        makeEmptyMethods(),      // protected methods
        makeEmptyMembers(),      // public members
        makeEmptyMembers(),      // private members
        makeEmptyMembers(),      // protected members
        true,                    // has copy assignment
        true                     // has move assignment
    );

    std::string expected = R"expected(/**
 * @class DestructClass
 * @brief Class with destructor and assignments
 */
class DestructClass {
public:
    ~DestructClass() = default;
    DestructClass& operator=(const DestructClass& other);
    DestructClass& operator=(DestructClass&& other) noexcept;
};
)expected";

    // Act:
    std::string output = ClassGenerator::generateClassDeclaration(cl);

    // Assert:
    EXPECT_EQ(output, expected);
}

// Test: Fully featured class with all sections.
TEST(ClassGeneratorDeclarationTest, ClassWithAllSectionsAndSpacing)
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

    std::string expected = R"expected(/**
 * @class FullClass
 * @brief Fully featured class
 */
class FullClass {
public:
    FullClass(int id);
    ~FullClass() = default;
    FullClass& operator=(const FullClass& other);
    FullClass& operator=(FullClass&& other) noexcept;
    /**
     * @brief Public method
     */
    void publicMethod();
    float value;
private:
    /**
     * @brief Private helper
     */
    void privateHelper();
protected:
    int counter;
};
)expected";

    // Act:
    std::string output = ClassGenerator::generateClassDeclaration(cl);

    // Assert:
    EXPECT_EQ(output, expected);
}

// Test: Class with only copy assignment.
TEST(ClassGeneratorDeclarationTest, ClassWithOnlyCopyAssignment)
{
    // Arrange:
    ClassModels::ClassModel cl(
        "CopyOnlyClass",
        "Class with only copy assignment",
        makeEmptyCtors(),
        std::nullopt,
        makeEmptyMethods(),
        makeEmptyMethods(),
        makeEmptyMethods(),
        makeEmptyMembers(),
        makeEmptyMembers(),
        makeEmptyMembers(),
        true,   // has copy assignment enabled
        false   // has move assignment disabled
    );

    std::string expected = R"expected(/**
 * @class CopyOnlyClass
 * @brief Class with only copy assignment
 */
class CopyOnlyClass {
public:
    CopyOnlyClass& operator=(const CopyOnlyClass& other);
};
)expected";

    // Act:
    std::string output = ClassGenerator::generateClassDeclaration(cl);

    // Assert:
    EXPECT_EQ(output, expected);
}

// Test: Class with empty description and whitespace in method description.
TEST(ClassGeneratorDeclarationTest, ClassWithEmptyDescriptionAndWhitespace)
{
    // Arrange:
    CallableModels::MethodModel pubMethod(
        PropertiesModels::DataType(PropertiesModels::Types::VOID),
        "specialMethod",
        std::vector<PropertiesModels::Parameter>{},
        PropertiesModels::DeclartionSpecifier(),
        "   A method with extra whitespace   "  // extra whitespace preserved by generator
    );
    std::vector<CallableModels::MethodModel> pubMethods = { pubMethod };

    ClassModels::ClassModel cl(
        "WhitespaceClass",
        "", // empty description
        makeEmptyCtors(),
        std::nullopt,
        pubMethods,
        makeEmptyMethods(),
        makeEmptyMethods(),
        makeEmptyMembers(),
        makeEmptyMembers(),
        makeEmptyMembers(),
        false,
        false
    );

    std::string expected = R"expected(/**
 * @class WhitespaceClass
 * @brief 
 */
class WhitespaceClass {
public:
    /**
     * @brief    A method with extra whitespace   
     */
    void specialMethod();
};
)expected";

    // Act:
    std::string output = ClassGenerator::generateClassDeclaration(cl);

    // Assert:
    EXPECT_EQ(output, expected);
}
