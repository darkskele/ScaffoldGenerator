/**
 * @file GeneratorUtilities.h
 * @brief Provides utility functions for converting scaffold property data types to their string representations.
 *
 * This header defines functions to convert various data type constructs, including qualifiers,
 * pointers, references, and custom types, into human-readable string formats. The implementation
 * adheres to modern C++20 best practices for efficiency, clarity, and robust error handling.
 *
 * @note The functions declared herein are designed to be pure, exception-safe, and suitable for
 *       use in high-performance contexts.
 */

 #pragma once

 #include "ScaffoldProperties.h"
 #include <string>
 
 /**
 * @namespace GeneratorUtilities
 * @brief Provides utility functions for converting scaffold property data types to their string representations.
 *
 * This namespace encapsulates functions that convert various data type constructs—including qualifiers,
 * pointers, references, and custom types—into human-readable string formats. The implementation adheres
 * to modern C++20 best practices, emphasizing efficiency, clarity, and robust error handling.
 *
 * @note The functions declared within are pure, exception-safe, and optimized for high-performance contexts.
 */

 namespace GeneratorUtilities
 {
     /**
      * @brief Converts a DataType object to its corresponding string representation.
      *
      * This function processes a DataType object defined in the ScaffoldProperties namespace,
      * combining type qualifiers, the base type, and type declarators into a cohesive string.
      * It supports built-in types, custom types, and compound types with qualifiers and modifiers.
      *
      * @param dt A constant reference to the DataType object to convert.
      * @return A std::string representing the full type (including qualifiers and declarators).
      *
      * @throws std::runtime_error If the DataType is unrecognized or if a custom type is specified without a name.
      *
      * @example
      * @code
      * ScaffoldProperties::DataType dt = ...;
      * std::string typeStr = GeneratorUtilities::dataTypeToString(dt);
      * @endcode
      */
     std::string dataTypeToString(const ScaffoldProperties::DataType &dt);

    /**
     * @brief Indents every line in the provided code block.
     *
     * This function reads the input code line by line and prepends each line with a specified
     * number of space characters (indentLevel). The resulting indented code is returned as a new string.
     *
     * @param code The original code block as a string.
     * @param indentLevel The number of spaces to prepend to each line.
     * @return A new string where each line of the input code is indented by the specified number of spaces.
     */
    std::string indentCode(const std::string &code, int indentLevel);

 } // namespace GeneratorUtilities
 