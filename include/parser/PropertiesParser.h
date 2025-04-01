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
     * @param typeStr String view to scaff segment to parse
     * @return The parsed datatype
     */
    ScaffoldProperties::DataType parseDataType(std::string_view typeStr);

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

} // namespace PropertiesParser