/**
 * @file MethodParser.h
 * @brief Contains pure functions to parse the Method block in a scaff file.
 *
 * This file defines functions that process property lines from a DSL method block and build
 * a corresponding MethodModel. It leverages modern C++20 features, such as std::string_view,
 * for efficient string handling, and emphasizes robust error handling.
 *
 * @note The parser expects each property line to begin with the '|' character and uses a key-value
 *       format separated by '='. Unrecognized keys will trigger a std::runtime_error.
 */

 #pragma once

 #include "ScaffoldModels.h"
 #include <string>
 #include <string_view>
 #include <vector>
 
 /**
  * @namespace MethodParser
  * @brief Encapsulates pure functions that parse the Method block in a scaff file.
  *
  * This namespace provides functionality to parse the DSL-defined properties for a method and
  * construct a MethodModel instance. The parser is designed to be efficient, robust, and easy
  * to maintain, in accordance with modern C++20 best practices.
  */
 namespace MethodParser
 {
     /**
      * @brief Parses a method block and constructs a MethodModel.
      *
      * This function processes the property lines (each expected to start with the '|' character)
      * and extracts key-value pairs to build a MethodModel instance. It handles return type conversion,
      * parameter list parsing, description extraction, and declaration specifiers. If any unrecognized
      * property is encountered or the block is malformed, a std::runtime_error is thrown.
      *
      * @param methodName The name of the method (extracted from the header line).
      * @param propertyLines A vector of string_views representing the property lines from the DSL.
      * @return A MethodModel containing the parsed properties.
      *
      * @throws std::runtime_error if the method block is malformed.
      */
     ScaffoldModels::MethodModel parseMethodProperties(const std::string &methodName, const std::vector<std::string_view> &propertyLines);
 } // namespace MethodParser
 