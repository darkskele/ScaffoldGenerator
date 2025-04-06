#include "NamespaceGenerator.h"
#include "CodeGroupModels.h"
#include "PropertiesModels.h"
#include "GeneratorUtilities.h"  // for dataTypeToString, if needed
#include "ClassGenerator.h"
#include "CallableGenerator.h"
#include "testUtility.h"
#include <gtest/gtest.h>
#include <string>
#include <sstream>

using namespace NamespaceGenerator;
using namespace CodeGroupModels;
using namespace PropertiesModels;



TEST(NamespaceGeneratorTest, GeneratesEmptyNamespace) {
    NamespaceModel ns;
    ns.name = "EmptyNS";
    ns.description = "";

    std::string decl = generateNamespaceDeclaration(ns);
    EXPECT_NE(decl.find("namespace EmptyNS"), std::string::npos);
    EXPECT_NE(decl.find("} // namespace EmptyNS"), std::string::npos);

    std::string def = generateNamespaceDefinition(ns);
    EXPECT_NE(def.find("namespace EmptyNS"), std::string::npos);
    EXPECT_NE(def.find("} // namespace EmptyNS"), std::string::npos);
}

TEST(NamespaceGeneratorTest, GeneratesNamespaceWithDescription) {
    NamespaceModel ns;
    ns.name = "TestNS";
    ns.description = "Test namespace description";

    std::string decl = generateNamespaceDeclaration(ns);
    EXPECT_NE(decl.find("Test namespace description"), std::string::npos);
}

TEST(NamespaceGeneratorTest, GeneratesAnonymousNamespace) {
    NamespaceModel ns;
    ns.name = "";  // Anonymous namespace
    ns.description = "Anonymous test";

    std::string decl = generateNamespaceDeclaration(ns);
    EXPECT_NE(decl.find("namespace {"), std::string::npos);
    std::string def = generateNamespaceDefinition(ns);
    EXPECT_NE(def.find("namespace {"), std::string::npos);
}

TEST(NamespaceGeneratorTest, GeneratesNamespaceWithNestedElements) {
    NamespaceModel ns;
    ns.name = "MyNS";
    ns.description = "Main namespace";

    // Add a dummy class.
    ns.classes.push_back(createDummyClass("DummyClass", "A dummy class"));

    // Add a dummy function.
    ns.functions.push_back(createDummyFunction("dummyFunction", "A dummy function"));

    // Add a nested namespace.
    NamespaceModel nested;
    nested.name = "NestedNS";
    nested.description = "Nested namespace";
    ns.namespaces.push_back(nested);

    std::string decl = generateNamespaceDeclaration(ns);
    EXPECT_NE(decl.find("namespace MyNS"), std::string::npos);
    EXPECT_NE(decl.find("A dummy class"), std::string::npos);
    EXPECT_NE(decl.find("dummyFunction"), std::string::npos);
    EXPECT_NE(decl.find("namespace NestedNS"), std::string::npos);

    std::string def = generateNamespaceDefinition(ns);
    EXPECT_NE(def.find("namespace MyNS"), std::string::npos);
    EXPECT_NE(def.find("DummyClass"), std::string::npos);
    EXPECT_NE(def.find("dummyFunction"), std::string::npos);
    EXPECT_NE(def.find("namespace NestedNS"), std::string::npos);
    // Check that inner definitions are indented (i.e. lines starting with four spaces).
    EXPECT_NE(def.find("    "), std::string::npos);
}

TEST(NamespaceGeneratorTest, GeneratesNamespaceWithMultipleNestedNamespaces) {
    NamespaceModel ns;
    ns.name = "OuterNS";
    ns.description = "Outer namespace";

    // Create several nested namespaces.
    NamespaceModel nested1;
    nested1.name = "InnerNS1";
    nested1.description = "First inner namespace";

    NamespaceModel nested2;
    nested2.name = "InnerNS2";
    nested2.description = "Second inner namespace";

    ns.namespaces.push_back(nested1);
    ns.namespaces.push_back(nested2);

    std::string decl = generateNamespaceDeclaration(ns);
    EXPECT_NE(decl.find("namespace OuterNS"), std::string::npos);
    EXPECT_NE(decl.find("namespace InnerNS1"), std::string::npos);
    EXPECT_NE(decl.find("namespace InnerNS2"), std::string::npos);

    std::string def = generateNamespaceDefinition(ns);
    EXPECT_NE(def.find("namespace OuterNS"), std::string::npos);
    EXPECT_NE(def.find("namespace InnerNS1"), std::string::npos);
    EXPECT_NE(def.find("namespace InnerNS2"), std::string::npos);
}

TEST(NamespaceGeneratorTest, IgnoresExtraWhitespace) {
    // Create a namespace with extra whitespace in the description and name.
    NamespaceModel ns;
    ns.name = "WhitespaceNS";
    ns.description = "   Trimmed description   ";

    std::string decl = generateNamespaceDeclaration(ns);
    // The output should contain the trimmed description.
    EXPECT_NE(decl.find("Trimmed description"), std::string::npos);
}

TEST(NamespaceGeneratorTest, HandlesNamespaceWithNoContent) {
    // A namespace with only a description, no nested classes, functions, or nested namespaces.
    NamespaceModel ns;
    ns.name = "NoContentNS";
    ns.description = "No content";

    std::string decl = generateNamespaceDeclaration(ns);
    // Check that the namespace block is generated.
    EXPECT_NE(decl.find("namespace NoContentNS"), std::string::npos);

    std::string def = generateNamespaceDefinition(ns);
    // Even if empty, the definitions should still wrap the namespace.
    EXPECT_NE(def.find("namespace NoContentNS"), std::string::npos);
}

