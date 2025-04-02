#pragma once

#include "ScaffoldModels.h"
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
