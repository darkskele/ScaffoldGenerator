/**
 * @file CallableParser.h
 * @brief Contains pure functions to parse the callable block in a scaff file.
 *
 * This file defines functions that process property lines from a DSL callable block—representing either a method or a free function—
 * and build a corresponding CallableModel. The parser leverages modern C++20 features, such as std::string_view, for efficient string handling,
 * and emphasizes robust error handling.
 *
 * @note The parser expects each property line to begin with the '|' character and uses a key-value format separated by '='.
 *       Unrecognized keys will trigger a std::runtime_error.
 */

 #pragma once

 #include "ScaffoldModels.h"
 #include <string>
 #include <string_view>
 #include <deque>
 
 /**
  * @namespace CallableParser
  * @brief Encapsulates pure functions that parse the callable block in a scaff file.
  *
  * This namespace provides functionality to parse DSL-defined properties for a callable (either a method or a free function)
  * and construct a corresponding CallableModel instance. The parser is designed to be efficient, robust, and easy to maintain.
  *
  * In addition, two inline wrapper functions are provided:
  *  - parseMethodProperties: Constructs a MethodModel from the common callable properties.
  *  - parseFunctionProperties: Constructs a FunctionModel from the common callable properties.
  *
  * @sa ScaffoldModels, PropertiesParser, ParserUtilities
  */
 namespace CallableParser
 {
     /**
      * @brief Parses a callable block and constructs a CallableModel.
      *
      * This function consumes property lines (each expected to start with the '|' character) from the provided deque
      * and extracts key-value pairs to build a CallableModel instance. It handles return type conversion,
      * parameter list parsing, description extraction, and declaration specifiers. If any unrecognized
      * property is encountered or the block is malformed, a std::runtime_error is thrown.
      *
      * @param callableName The name of the callable (extracted from the header line).
      * @param propertyLines A deque of string_views representing the property lines from the DSL. Lines are consumed as they are processed.
      * @return A CallableModel containing the parsed properties.
      *
      * @throws std::runtime_error if the callable block is malformed.
      */
     ScaffoldModels::CallableModel parseCallableProperties(const std::string &callableName, std::deque<std::string_view>& propertyLines);
 
     /**
      * @brief Parses a method block and constructs a MethodModel.
      *
      * This inline function wraps parseCallableProperties to build a MethodModel.
      *
      * @param methodName The name of the method.
      * @param propertyLines A deque of string_views representing the property lines from the DSL.
      * @return A MethodModel containing the parsed properties.
      *
      * @throws std::runtime_error if the callable block is malformed.
      */
     inline ScaffoldModels::MethodModel parseMethodProperties(const std::string &methodName, std::deque<std::string_view>& propertyLines)
     {
         auto base = parseCallableProperties(methodName, propertyLines);
         return ScaffoldModels::MethodModel(base.returnType, methodName, base.parameters, base.declSpec, base.description);
     }
 
     /**
      * @brief Parses a function block and constructs a FunctionModel.
      *
      * This inline function wraps parseCallableProperties to build a FunctionModel.
      *
      * @param functionName The name of the function.
      * @param propertyLines A deque of string_views representing the property lines from the DSL.
      * @return A FunctionModel containing the parsed properties.
      *
      * @throws std::runtime_error if the callable block is malformed.
      */
     inline ScaffoldModels::FunctionModel parseFunctionProperties(const std::string &functionName, std::deque<std::string_view>& propertyLines)
     {
         auto base = parseCallableProperties(functionName, propertyLines);
         return ScaffoldModels::FunctionModel(base.returnType, functionName, base.parameters, base.declSpec, base.description);
     }
     
 } // namespace CallableParser
 