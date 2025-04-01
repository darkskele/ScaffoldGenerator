/**
 * @file PropertiesGenerator.h
 * @brief Contain pure functions to generate keyword properties.
 *
 * This file defines a set of composable pure functions to generate properties
 * within a keyword scope from a scaff file.
 */

 #pragma once

 #include "ScaffoldProperties.h"
 #include <vector>

 /**
 * @namespace PropertiesGenerator
 * @brief Encapsulates pure functions that generate scope properties
 */
namespace PropertiesGenerator
{
    /**
     * @brief Generates list of parameters from vector
     * @param params List of parameters to generate
     * @return The generated string representation of parameters
     */
    std::string generateParameterList(const std::vector<ScaffoldProperties::Parameter>& params);

    /**
     * @brief Converts decalartion specifier object to string
     * @param dS Declaration Specifier to convert
     * @return String representation of Declaration Specifier
     */
    std::string generateDeclarationSpecifier(const ScaffoldProperties::DeclartionSpecifier &dS);

} // namespace PropertiesGenerator