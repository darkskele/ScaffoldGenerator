/**
 * @file ClassGenerator.h
 * @brief Functions to generate C++ class declarations and definitions from DSL models.
 */

#pragma once

#include "ScaffoldModels.h"
#include <string>

/**
 * @namespace ClassGenerator
 * @brief Provides functions to generate a C++ class declaration (header) and definition (implementation)
 *        based on the provided DSL ClassModel.
 */
namespace ClassGenerator
{
    /**
     * @brief Generates the C++ class declaration from a ClassModel.
     *
     * This function generates the header portion of a C++ class based on the provided
     * DSL model. It includes declarations for constructors, assignment operators, methods,
     * and member variables.
     *
     * @param cl The ClassModel containing all DSL class data.
     * @return A string containing the C++ class declaration.
     */
    std::string generateClassDeclaration(const ScaffoldModels::ClassModel& cl);

    /**
     * @brief Generates the C++ class definition from a ClassModel.
     *
     * This function generates the out-of-line definitions for the member functions declared
     * in the class. It includes definitions for constructors, assignment operators, destructors,
     * and methods.
     *
     * @param cl The ClassModel containing all DSL class data.
     * @return A string containing the C++ class definition.
     */
    std::string generateClassDefinition(const ScaffoldModels::ClassModel& cl);
}
