/**
 * @file SpecialMemberFunctionParser.h
 * @brief Declares parsers for constructors and destructors in the Scaffolder DSL.
 *
 * This header defines functions responsible for parsing special member function blocks—
 * specifically constructors and destructors—within the Scaffolder DSL. The parser translates
 * structured DSL input into internal model representations used by the code generator.
 *
 * These functions enforce correctness according to C++ semantics (e.g., destructors cannot have parameters,
 * and copy/move constructors must not define custom parameters). They also reuse existing infrastructure
 * for tokenization, data type parsing, and parameter extraction.
 *
 * @namespace SpecialMemberFunctionParser
 * @see ScaffoldModels::Constructor
 * @see ScaffoldModels::Destructor
 * @see PropertiesParser
 * @see ParserUtilities
 */

 #pragma once

 #include <string>
 #include <string_view>
 #include <deque>
 #include "ScaffoldModels.h"  // For Constructor, Destructor, and ConstructorType
 
 /**
  * @namespace SpecialMemberFunctionParser
  * @brief Contains parsing functions for special member functions in the Scaffolder DSL.
  *
  * This namespace defines functions for parsing constructors and destructors from DSL blocks.
  * It supports both inline and nested declarations, enforcing C++ constraints (e.g., parameter rules),
  * and maps parsed DSL properties to internal models (`Constructor` and `Destructor`) used for code generation.
  *
  * The parsing logic reuses shared utilities for trimming, tokenization, and type resolution to maintain
  * consistency across the scaffolding toolchain.
  *
  * @see ScaffoldModels::Constructor
  * @see ScaffoldModels::Destructor
  * @see PropertiesParser
  * @see ParserUtilities
  */
 namespace SpecialMemberFunctionParser
 {
     /**
      * @brief Parses a constructor block from the DSL.
      *
      * The block identifier (e.g., "default", "copy", "move", "custom") determines
      * the type of constructor, and the property lines define its parameters and description.
      *
      * @param constructorIdentifier The constructor block identifier.
      * @param propertyLines A deque of property lines from the DSL block. Lines are consumed as they are processed.
      * @return A populated Constructor model.
      */
     ScaffoldModels::Constructor parseConstructorProperties(const std::string &constructorIdentifier,
                                                              std::deque<std::string_view>& propertyLines);
 
     /**
      * @brief Parses a destructor block from the DSL.
      *
      * Since destructors cannot have parameters, only a description is parsed.
      *
      * @param propertyLines A deque of property lines from the DSL block. Lines are consumed as they are processed.
      * @return A populated Destructor model.
      */
     ScaffoldModels::Destructor parseDestructorProperties(std::deque<std::string_view>& propertyLines);
 }
 