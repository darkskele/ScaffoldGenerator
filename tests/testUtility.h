#pragma once

#include "ClassModels.h"
#include "CallableModels.h"
#include "PropertiesModels.h"
#include "PropertiesParser.h"
#include "CodeGroupModels.h"
#include "IFileWriter.h"

#include <vector>

// Helper functions to create empty vectors.
inline std::vector<CallableModels::MethodModel> makeEmptyMethods()
{
    return std::vector<CallableModels::MethodModel>{};
}

inline std::vector<PropertiesModels::Parameter> makeEmptyMembers()
{
    return std::vector<PropertiesModels::Parameter>{};
}

inline std::vector<ClassModels::Constructor> makeEmptyCtors()
{
    return std::vector<ClassModels::Constructor>{};
}

// Helper: Create a dummy ClassModel.
// Updated dummy class helper that includes a default constructor.
static inline ClassModels::ClassModel createDummyClass(const std::string &name, const std::string &desc = "Dummy class")
{
    // Create a default constructor with an empty description.
    std::vector<PropertiesModels::Parameter> emptyParams;
    ClassModels::Constructor defaultCtor(
        ClassModels::ConstructorType::COPY,
        emptyParams,
        "Copy constructor");

    // Construct a class with a single default constructor.
    return ClassModels::ClassModel(
        name,
        desc,
        {defaultCtor}, // constructors
        std::nullopt,  // no destructor
        {},            // public methods
        {},            // private methods
        {},            // protected methods
        {},            // public members
        {},            // private members
        {},            // protected members
        false,         // hasCopyAssignment
        false          // hasMoveAssignment
    );
}

// Helper: Create a dummy FunctionModel.
static inline CallableModels::FunctionModel createDummyFunction(const std::string &name, const std::string &desc = "Dummy function")
{
    // Using "void" as return type; parameters and declSpec are empty.
    PropertiesModels::DataType voidType = PropertiesParser::parseDataType("void");
    return CallableModels::FunctionModel(voidType, name, {}, {}, desc);
}

static inline CodeGroupModels::NamespaceModel createDummyNamespace(const std::string &name, const std::string &desc = "Dummy namespace")
{
    using namespace CodeGroupModels;

    // Create some dummy classes
    std::vector<ClassModels::ClassModel> dummyClasses = {
        createDummyClass("AlphaClass"),
        createDummyClass("BetaClass")};

    // Create some dummy functions
    std::vector<CallableModels::FunctionModel> dummyFunctions = {
        createDummyFunction("doSomething"),
        createDummyFunction("calculateSomething")};

    // Optional: Add a nested namespace for testing deeper recursion
    NamespaceModel nestedNamespace;
    nestedNamespace.name = "Inner";
    nestedNamespace.description = "Nested inner namespace";
    nestedNamespace.classes = {createDummyClass("InnerClass")};
    nestedNamespace.functions = {createDummyFunction("innerFunction")};
    nestedNamespace.namespaces = {}; // No deeper nesting here

    return NamespaceModel{
        name,
        desc,
        dummyClasses,
        dummyFunctions,
        {nestedNamespace}};
}

class TestFileWriter : public IFileWriter
{
public:
    struct FileWrite
    {
        std::string type;     ///< The type of file written ("header" or "source").
        std::string filePath; ///< The file path that was written.
        std::string content;  ///< The content that was written.
    };

    std::vector<FileWrite> calls;

    void writeHeaderFile(const std::string &filePath, const std::string &content) override
    {
        calls.push_back({"header", filePath, content});
    }

    void writeSourceFile(const std::string &filePath, const std::string &content) override
    {
        calls.push_back({"source", filePath, content});
    }
};