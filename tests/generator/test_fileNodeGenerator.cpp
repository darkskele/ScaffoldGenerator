// File: TESTileNodeGenerator.cpp
#include "gtest/gtest.h"
#include "FileNodeGenerator.h"

// Include DSL model headers.
#include "ClassModels.h"
#include "CodeGroupModels.h"
#include "CallableModels.h"
#include "PropertiesModels.h"

// Helper function to create a minimal ClassModels::ClassModel.
ClassModels::ClassModel createTestClassModel(const std::string &className) {
    return ClassModels::ClassModel(
        className,
        "Test class " + className,  // description
        {},                         // constructors
        std::nullopt,               // destructor
        {}, {}, {},                 // public, private, protected methods
        {}, {}, {},                 // public, private, protected members
        false,                      // hasCopyAssignment
        false                       // hasMoveAssignment
    );
}

// Helper function to create a minimal CodeGroupModels::NamespaceModel.
CodeGroupModels::NamespaceModel createTestNamespaceModel(const std::string &nsName) {
    CodeGroupModels::NamespaceModel ns;
    ns.name = nsName;
    ns.description = "Test namespace " + nsName;
    return ns;
}

// Helper function to create a minimal CallableModels::FunctionModel.
CallableModels::FunctionModel createTestFunctionModel(const std::string &funcName) {
    PropertiesModels::DataType dt(PropertiesModels::Types::INT);
    PropertiesModels::DeclartionSpecifier ds; // defaults
    return CallableModels::FunctionModel(dt, funcName, {}, ds, "Test function " + funcName);
}

// Test fixture for FileNodeGenerator.
class FileNodeGeneratorSpecializationsTest : public ::testing::Test {
protected:
    // Common setup/teardown can go here if needed.
};

// Test for FileNode with a ClassModels::ClassModel.
TEST(FileNodeGeneratorSpecializationsTest, ClassFileNodeGeneratesCorrectContent) {
    ClassModels::ClassModel testClass = createTestClassModel("TestClass");

    // Create a FileNode with base path "MyProject/core" and file name "TestClass".
    FileNodeGenerator::FileNode<ClassModels::ClassModel> fileNode("MyProject/core", "TestClass", testClass);

    GeneratedFiles files = fileNode.generateFiles();

    // Expected base file path.
    EXPECT_EQ(files.baseFilePath, "MyProject/core/TestClass");

    // Adjust expected strings to match actual output from your generators.
    // For example, suppose ClassGenerator produces the following output:
    std::string expectedHeader =
        "/**\n"
        " * @class TestClass\n"
        " * @brief Test class TestClass\n"
        " */\n"
        "class TestClass {\npublic:\n};\n";
    // And if the source content is empty (or as defined by your generator):
    std::string expectedSource = ""; // or actual generated source if any

    EXPECT_EQ(files.headerContent, expectedHeader);
    EXPECT_EQ(files.sourceContent, expectedSource);
}

// Test for FileNode with a CodeGroupModels::NamespaceModel.
TEST(FileNodeGeneratorSpecializationsTest, NamespaceFileNodeGeneratesCorrectContent) {
    CodeGroupModels::NamespaceModel testNS = createTestNamespaceModel("TestNamespace");

    // Create a FileNode with base path "MyProject/utils" and file name "TestNamespace".
    FileNodeGenerator::FileNode<CodeGroupModels::NamespaceModel> fileNode("MyProject/utils", "TestNamespace", testNS);

    GeneratedFiles files = fileNode.generateFiles();

    EXPECT_EQ(files.baseFilePath, "MyProject/utils/TestNamespace");

    // Adjust expected strings to match actual output from NamespaceGenerator.
    std::string expectedHeader =
        "/**\n"
        " * @brief Test namespace TestNamespace\n"
        " */\n"
        "namespace TestNamespace {\n} // namespace TestNamespace\n";
    std::string expectedSource =
        "namespace TestNamespace {\n} // namespace TestNamespace\n";

    EXPECT_EQ(files.headerContent, expectedHeader);
    EXPECT_EQ(files.sourceContent, expectedSource);
}

// Test for FileNode with a vector of CallableModels::FunctionModel.
TEST(FileNodeGeneratorSpecializationsTest, FunctionFileNodeGeneratesCorrectContentForVector) {
    CallableModels::FunctionModel func1 = createTestFunctionModel("funcOne");
    CallableModels::FunctionModel func2 = createTestFunctionModel("funcTwo");

    std::vector<CallableModels::FunctionModel> funcVec = { func1, func2 };

    // Create a FileNode with base path "MyProject/libs" and file name "libsFuncs".
    FileNodeGenerator::FileNode<std::vector<CallableModels::FunctionModel>> fileNode("MyProject/libs", "libsFuncs", funcVec);

    GeneratedFiles files = fileNode.generateFiles();

    EXPECT_EQ(files.baseFilePath, "MyProject/libs/libsFuncs");

    // Adjust expected strings to match actual output from CallableGenerator.
    std::string expectedHeader =
        "/**\n"
        " * @brief Test function funcOne\n"
        " */\n"
        "int funcOne();\n\n"
        "/**\n"
        " * @brief Test function funcTwo\n"
        " */\n"
        "int funcTwo();\n\n";
    std::string expectedSource =
        "int funcOne() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n\n"
        "int funcTwo() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n\n";

    EXPECT_EQ(files.headerContent, expectedHeader);
    EXPECT_EQ(files.sourceContent, expectedSource);
}

// Test that getBasePath returns the correct base path.
TEST(FileNodeGeneratorSpecializationsTest, GetBasePathReturnsCorrectValue) {
    ClassModels::ClassModel testClass = createTestClassModel("AnotherTestClass");
    FileNodeGenerator::FileNode<ClassModels::ClassModel> fileNode("MyProject/lib", "AnotherTestClass", testClass);
    EXPECT_EQ(fileNode.getBasePath(), "MyProject/lib");
}

// Test for FileNode with an empty vector of CallableModels::FunctionModel.
TEST(FileNodeGeneratorSpecializationsTest, EmptyFunctionVectorGeneratesEmptyContent) {
    std::vector<CallableModels::FunctionModel> emptyFuncVec;
    
    // Create a FileNode with base path "MyProject/empty" and file name "emptyFuncs".
    FileNodeGenerator::FileNode<std::vector<CallableModels::FunctionModel>> fileNode("MyProject/empty", "emptyFuncs", emptyFuncVec);
    
    GeneratedFiles files = fileNode.generateFiles();
    
    // Expect base file path to be computed correctly.
    EXPECT_EQ(files.baseFilePath, "MyProject/empty/emptyFuncs");
    
    // Expect both header and source contents to be empty.
    EXPECT_EQ(files.headerContent, "");
    EXPECT_EQ(files.sourceContent, "");
}

// Test for FileNode with a single function in the vector.
TEST(FileNodeGeneratorSpecializationsTest, SingleFunctionVectorGeneratesCorrectContent) {
    CallableModels::FunctionModel singleFunc = createTestFunctionModel("singleFunc");
    std::vector<CallableModels::FunctionModel> funcVec = { singleFunc };
    
    FileNodeGenerator::FileNode<std::vector<CallableModels::FunctionModel>> fileNode("MyProject/single", "singleFuncs", funcVec);
    
    GeneratedFiles files = fileNode.generateFiles();
    
    EXPECT_EQ(files.baseFilePath, "MyProject/single/singleFuncs");
    
    // Expected output for a single function should be exactly that of the stubs.
    std::string expectedHeader =
        "/**\n"
        " * @brief Test function singleFunc\n"
        " */\n"
        "int singleFunc();\n\n";
    std::string expectedSource =
        "int singleFunc() {\n"
        "    throw std::runtime_error(\"Not implemented\");\n"
        "}\n\n";
    
    EXPECT_EQ(files.headerContent, expectedHeader);
    EXPECT_EQ(files.sourceContent, expectedSource);
}

// Test for FileNode with a complex base path.
TEST(FileNodeGeneratorSpecializationsTest, ComplexBasePathGeneratesCorrectFilePath) {
    ClassModels::ClassModel testClass = createTestClassModel("ComplexClass");
    
    // Base path with multiple levels.
    FileNodeGenerator::FileNode<ClassModels::ClassModel> fileNode("MyProject/module/submodule", "ComplexClass", testClass);
    GeneratedFiles files = fileNode.generateFiles();
    
    // Expected base file path.
    EXPECT_EQ(files.baseFilePath, "MyProject/module/submodule/ComplexClass");
}

// Test for FileNode when the DSL model has an empty name.
TEST(FileNodeGeneratorSpecializationsTest, EmptyModelNameGeneratesCorrectOutput) {
    // Create a ClassModel with an empty name.
    ClassModels::ClassModel emptyClass(
        "",                                // empty name
        "Test class with empty name",      // description
        {},                                // constructors
        std::nullopt,                      // destructor
        {}, {}, {},                        // methods
        {}, {}, {},                        // members
        false, false                       // assignment flags
    );
    
    FileNodeGenerator::FileNode<ClassModels::ClassModel> fileNode("MyProject/empty", "", emptyClass);
    GeneratedFiles files = fileNode.generateFiles();
    
    // Even with an empty file name, the base file path should correctly combine.
    EXPECT_EQ(files.baseFilePath, "MyProject/empty/");
    
    // Expected output is generated by ClassGenerator. Adjust the expected string if your generator
    // handles empty names differently (e.g., by emitting warnings or defaults).
    std::string expectedHeader =
        "/**\n"
        " * @class \n"
        " * @brief Test class with empty name\n"
        " */\n"
        "class  {\npublic:\n};\n";
    EXPECT_EQ(files.headerContent, expectedHeader);
}

// Test for FileNode when the base path has extra whitespace.
TEST(FileNodeGeneratorSpecializationsTest, WhitespaceInBasePathPreserved) {
    ClassModels::ClassModel testClass = createTestClassModel("WhitespaceClass");
    
    // Base path includes extra whitespace.
    FileNodeGenerator::FileNode<ClassModels::ClassModel> fileNode(" MyProject/whitespace ", "WhitespaceClass", testClass);
    GeneratedFiles files = fileNode.generateFiles();
    
    // Expected base file path should preserve the whitespace.
    EXPECT_EQ(files.baseFilePath, " MyProject/whitespace /WhitespaceClass");
}
