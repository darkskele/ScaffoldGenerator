/**
 * @file ClassParser.h
 * @brief Declares the parser for class blocks in the Scaffolder DSL.
 *
 * This file provides the interface for parsing C++ class declarations defined
 * in the custom scaffolder DSL. It extracts metadata, constructors, destructors,
 * members, and methods, and constructs a corresponding ClassModel used in code generation.
 */

#pragma once

#include "ClassModels.h"

#include <string>
#include <string_view>
#include <deque>

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
     * @brief Enumeration of access specifiers used in class parsing.
     *
     * This enum defines the different access levels recognized by the DSL parser.
     * When parsing class blocks, declarations are associated with an access level.
     * If no explicit access specifier is provided, declarations default to Private.
     */
    enum class Access
    {
        None, // No current access specifier; next declarations default to private.
        Private,
        Public,
        Protected
    };

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
     * CallableParser and SpecialMemberFunctionParser.
     *
     * @param className The name of the class as declared in the DSL.
     * @param classBlockLines A deque of DSL lines within the class block (excluding start/end markers).
     *        Lines are consumed as they are processed.
     * @return A fully populated ClassModel representing the parsed class.
     *
     * @throws std::runtime_error on malformed input or unknown keywords.
     *
     * @see ClassModels::ClassModel
     * @see CallableParser
     * @see SpecialMemberFunctionParser
     */
    ClassModels::ClassModel parseClassBlock(
        const std::string &className,
        std::deque<std::string_view> &classBlockLines);

} // namespace ClassParser
