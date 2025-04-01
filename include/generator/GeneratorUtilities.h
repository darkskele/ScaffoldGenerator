/**
 * @file GeneratorUtilities.h
 * @brief Contain various generator utilities.
 */

#pragma once

#include "ScaffoldProperties.h"
#include <string>

/**
 * @namespace GeneratorUtilities
 * @brief Encapsulates pure functions that parse keyword properties
 */
namespace GeneratorUtilities
{
    /**
     * @brief Converts datatype object to string
     * @param dt Datatype to convert
     * @return String representation of datatype
     *
     * @throws std::runtime_error data type is not recognised.
     */
    std::string dataTypeToString(const ScaffoldProperties::DataType &dt);

} // namespace GeneratorUtilities