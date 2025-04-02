/**
 * @file ClassParser.h
 * @brief Declares the parser for class blocks in the Scaffolder DSL.
 *
 * This file provides the interface for parsing C++ class declarations defined
 * in the custom scaffolder DSL. It extracts metadata, constructors, destructors,
 * members, and methods, and constructs a corresponding ClassModel used in code generation.
 */

 #pragma once

 #include "ScaffoldModels.h"
 #include <string>
 #include <string_view>
 #include <vector>
 
 /**
 * @namespace ClassParser
 * @brief Contains parsing logic for class blocks in the Scaffolder DSL.
 *
 * The ClassParser namespace provides the high-level parser for translating DSL-defined
 * class blocks into structured ClassModel representations. It supports parsing nested
 * access sections, constructors, destructors, and methods by coordinating with other
 * specialized parsers.
 *
 * This parser is responsible for enforcing DSL grammar rules, managing scope transitions,
 * and aggregating class-level metadata for later code generation.
 */
 namespace ClassParser
 {
     /**
      * @brief Parses a class block from the DSL into a ClassModel.
      *
      * This function processes all top-level and nested properties within a `class` block,
      * including:
      * - Class description
      * - Inline constructor/assignment flags
      * - Nested method, constructor, and destructor blocks
      * - Access-level scopes (public, private, protected)
      *
      * It delegates parsing of individual constructs to specialized sub-parsers such as
      * MethodParser and SpecialMemberFunctionParser.
      *
      * @param className The name of the class as declared in the DSL.
      * @param classBlockLines The lines of DSL content within the class block (excluding start/end markers).
      * @return A fully populated ClassModel representing the parsed class.
      *
      * @throws std::runtime_error on malformed input or unknown keywords.
      *
      * @see ScaffoldModels::ClassModel
      * @see MethodParser
      * @see SpecialMemberFunctionParser
      */
     ScaffoldModels::ClassModel parseClassBlock(
         const std::string& className,
         const std::vector<std::string_view>& classBlockLines);
 }
 