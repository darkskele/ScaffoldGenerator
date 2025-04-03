/**
 * @file PropertiesGenerator.h
 * @brief Contains pure functions to generate keyword properties.
 *
 * This file defines a set of composable, pure functions designed to generate string representations
 * of keyword properties from a DSL file. The functions herein leverage modern C++20 features such as
 * std::format for type-safe string formatting, ensuring clarity, efficiency, and robust error handling.
 *
 * The primary functionalities provided include:
 *  - Generating a parameter list string from a vector of parameters.
 *  - Converting a declaration specifier object into its string representation.
 *
 * @note These functions follow modern C++ best practices and are designed to be composable and pure,
 *       facilitating easy unit testing and maintenance.
 */

#pragma once

#include "PropertiesModels.h"

#include <vector>
#include <string>

/**
 * @namespace PropertiesGenerator
 * @brief Encapsulates pure functions that generate string representations for scope properties.
 *
 * This namespace provides functionality to translate DSL-defined properties into C++ code-friendly strings.
 * The implementations are designed to be efficient and maintainable while leveraging C++20 features.
 */
namespace PropertiesGenerator
{
    /**
     * @brief Generates a comma-separated list of parameters from a vector.
     *
     * This function iterates over a vector of parameters and converts each parameter into a "type name"
     * string using GeneratorUtilities. The parameters are concatenated into a single, comma-separated string.
     *
     * @param params A vector containing the parameter objects to be formatted.
     * @return A std::string representing the complete parameter list.
     *
     * @note Utilizes std::format for efficient and type-safe string formatting.
     */
    std::string generateParameterList(const std::vector<PropertiesModels::Parameter> &params);

    /**
     * @brief Converts a declaration specifier object to its string representation.
     *
     * This function inspects the provided declaration specifier and constructs a space-separated string that
     * includes all applicable specifiers (e.g., static, inline, constexpr).
     *
     * @param dS A constant reference to the declaration specifier object.
     * @return A std::string representing the declaration specifiers.
     *
     * @note The returned string is formatted for direct use in a method or function declaration.
     */
    std::string generateDeclarationSpecifier(const PropertiesModels::DeclartionSpecifier &dS);

} // namespace PropertiesGenerator
