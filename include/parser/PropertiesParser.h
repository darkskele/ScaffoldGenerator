/**
 * @file PropertiesParser.h
 * @brief Contains pure functions to parse stated properties in a scaff file.
 *
 * This file defines a set of composable, pure functions that parse properties within a keyword
 * scope from a scaff file. The functions efficiently process string_views to extract type information,
 * qualifiers, declarators, parameters, and declaration specifiers using modern C++20 practices.
 *
 * @note The parser functions are designed to be exception-safe and to handle malformed input robustly.
 */

#pragma once

#include "PropertiesModels.h"

#include <string_view>
#include <vector>

/**
 * @namespace PropertiesParser
 * @brief Encapsulates pure functions that parse keyword properties.
 *
 * This namespace provides functions to convert DSL property segments into structured types that
 * are used by the scaffolder. It leverages std::string_view for zero-copy string manipulation,
 * ensuring both efficiency and clarity.
 */
namespace PropertiesParser
{
    /**
     * @brief Parses a data type from the provided string view.
     *
     * This function extracts type qualifiers (e.g., "const", "volatile") and type declarators
     * (e.g., pointers, references, arrays) from the input string. It then compares the remaining
     * string against known type names. Unrecognized types are treated as custom types.
     *
     * @param typeStr A std::string_view containing the type segment to parse.
     * @return A DataType representing the parsed type.
     */
    PropertiesModels::DataType parseDataType(std::string_view typeStr);

    /**
     * @brief Parses type qualifiers from the provided string view.
     *
     * This function checks if the given string starts with recognized qualifiers (such as "const"
     * or "volatile"). As qualifiers are found, they are accumulated in a bitmask and removed from
     * the string view.
     *
     * @param qualStr A reference to the std::string_view to parse; it is modified to remove parsed qualifiers.
     * @return A TypeQualifier bitmask representing the extracted qualifiers.
     */
    PropertiesModels::TypeQualifier parseTypeQualifier(std::string_view &qualStr);

    /**
     * @brief Parses type declarators from the provided string view.
     *
     * Type declarators (e.g., pointers '*', references '&'/'&&', and arrays '[n]') are expected
     * to appear appended to the base type. This function extracts those declarators from the string view.
     *
     * @param typeStr A reference to the std::string_view containing the type; it is modified to remove declarators.
     * @return A TypeDeclarator representing the parsed declarators.
     *
     * @throws std::runtime_error if the parameter token is malformed.
     */
    PropertiesModels::TypeDeclarator parseTypeDeclarator(std::string_view &typeStr);

    /**
     * @brief Parses a comma-separated list of parameters from the provided string view.
     *
     * The expected format is "param1:int, param2:float". Each parameter is split into a name
     * and a type, with the type being parsed using parseDataType.
     *
     * @param paramStr A std::string_view containing the parameter segment to parse.
     * @return A vector of Parameter objects representing the parsed parameters.
     *
     * @throws std::runtime_error if any parameter token is malformed.
     */
    std::vector<PropertiesModels::Parameter> parseParameters(std::string_view paramStr);

    /**
     * @brief Parses declaration specifiers from the provided string view.
     *
     * Declaration specifiers (e.g., "static", "inline", "constexpr") are extracted from the input
     * and used to set the corresponding flags in a DeclartionSpecifier object.
     *
     * @param declStr A std::string_view containing the declaration specifier segment to parse.
     * @return A DeclartionSpecifier object with the parsed specifiers.
     */
    PropertiesModels::DeclartionSpecifier parseDeclarationSpecifier(std::string_view declStr);

} // namespace PropertiesParser
