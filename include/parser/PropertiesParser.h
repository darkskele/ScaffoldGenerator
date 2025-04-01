/**
 * @file PropertiesParser.h
 * @brief Contain pure functions to parse stated properties in a scaff file.
 *
 * This file defines a set of composable pure functions to parse properties
 * within a keyword scope from a scaff file.
 */

#pragma once

#include "ScaffoldProperties.h"
#include "ScaffoldModels.h"
#include <string_view>
#include <vector>

/**
 * @namespace PropertiesParser
 * @brief Encapsulates pure functions that parse keyword properties
 */
namespace PropertiesParser
{
    /**
     * @brief Parses datatype from the provided string view
     * 
     * This function first extracts any type qualifiers (e.g., "const", "volatile")
     * and then compares the remaining string against known type strings. If the
     * type isn't recognized, it is treated as a custom type.
     * 
     * @param typeStr String view to scaff segment to parse
     * @return The parsed datatype
     */
    ScaffoldProperties::DataType parseDataType(std::string_view typeStr);

    /**
     * @brief Parses type qualifier from provided string view
     *
     * This function checks if the given string starts with recognized qualifiers
     * (e.g., "const" or "volatile"). It updates the string view to remove those
     * qualifiers as they are found.
     *
     * @param qualStr String view to scaff segment to parse
     * @return The parsed type qualifier
     */
    ScaffoldProperties::TypeQualifier parseTypeQualifier(std::string_view& qualStr);

    /**
     * @brief Parses type declartors for data type.
     * 
     * Type declarators appear appened to data type, where as qualifiers are appeneded.parseDataType
     * 
     * @param typeStr String view to scaff segment to parse
     * @return Parsed declarator
     * 
     * @throws std::runtime_error if a parameter token is malformed.
     */
    ScaffoldProperties::TypeDeclarator parseTypeDeclarator(std::string_view& typeStr);

    /**
     * @brief Parses parameters from the provided string view
     *
     * Expected format: "param1:int, param2:float"
     *
     * @param paramStr String view to scaff segment to parse
     * @return The parsed parameter list
     *
     * @throws std::runtime_error if a parameter token is malformed.
     */
    std::vector<ScaffoldProperties::Parameter> parseParameters(std::string_view paramStr);

    /**
     * @brief Parses declaration specifiers for methods.
     * @param declStr String view to scaff segment to parse
     * @return Parsed declartion specifier
     */
    ScaffoldProperties::DeclartionSpecifier parseDeclarationSpecifier(std::string_view declStr);

} // namespace PropertiesParser