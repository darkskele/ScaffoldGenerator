#pragma once

#include "ScaffoldModels.h"
#include "ScaffoldProperties.h"
#include "PropertiesParser.h"
#include <vector>

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

// Helper: Create a dummy ClassModel.
// Updated dummy class helper that includes a default constructor.
static inline ScaffoldModels::ClassModel createDummyClass(const std::string& name, const std::string& desc = "Dummy class") {
    // Create a default constructor with an empty description.
    std::vector<ScaffoldProperties::Parameter> emptyParams;
    ScaffoldModels::Constructor defaultCtor(
        ScaffoldModels::ConstructorType::COPY, 
        emptyParams, 
        "Copy constructor");
    
    // Construct a class with a single default constructor.
    return ScaffoldModels::ClassModel(
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
static inline ScaffoldModels::FunctionModel createDummyFunction(const std::string& name, const std::string& desc = "Dummy function") {
    // Using "void" as return type; parameters and declSpec are empty.
    ScaffoldProperties::DataType voidType = PropertiesParser::parseDataType("void");
    return ScaffoldModels::FunctionModel(voidType, name, {}, {}, desc);
}