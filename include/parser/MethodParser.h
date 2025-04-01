/**
 * @file MethodParser.h
 * @brief Contains pure functions to parse the Method block in a scaff file.
 */

 #pragma once

 #include "ScaffoldModels.h"
 
 /**
  * @namespace MethodParser
  * @brief Encapsulates pure functions that parse the Method block in a scaff file.
  */
 namespace MethodParser
 {
     /**
      * @brief Parses a method block.
      *
      * This function processes the property lines (each expected to start with '|')
      * and builds a MethodModel from them.
      *
      * @param methodName The name of the method (extracted from the header).
      * @param propertyLines A vector of string_views representing the property lines.
      * @return A MethodModel containing the parsed properties.
      * 
      * @throws std::runtime_error if the scaff block is malformed.
      */
     ScaffoldModels::MethodModel parseMethodProperties(const std::string& methodName, const std::vector<std::string_view>& propertyLines);
 } // namespace MethodParser
 