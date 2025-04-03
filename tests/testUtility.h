#pragma once

#include "ClassModels.h"
#include "CallableModels.h"
#include "PropertiesModels.h"
#include "PropertiesParser.h"
#include <vector>

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

// Helper: Create a dummy ClassModel.
// Updated dummy class helper that includes a default constructor.
static inline ClassModels::ClassModel createDummyClass(const std::string& name, const std::string& desc = "Dummy class") {
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
        { defaultCtor },   // constructors
        std::nullopt,      // no destructor
        {},                // public methods
        {},                // private methods
        {},                // protected methods
        {},                // public members
        {},                // private members
        {},                // protected members
        false,             // hasCopyAssignment
        false              // hasMoveAssignment
    );
}

// Helper: Create a dummy FunctionModel.
static inline CallableModels::FunctionModel createDummyFunction(const std::string& name, const std::string& desc = "Dummy function") {
    // Using "void" as return type; parameters and declSpec are empty.
    PropertiesModels::DataType voidType = PropertiesParser::parseDataType("void");
    return CallableModels::FunctionModel(voidType, name, {}, {}, desc);
}