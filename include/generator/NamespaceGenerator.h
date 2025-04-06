/**
 * @file NamespaceGenerator.h
 * @brief Functions to generate C++ namespace declarations and definitions from DSL models.
 *
 * This file declares functions that convert a DSL-defined namespace model (NamespaceModel)
 * into C++ source code. The generated code includes the namespace declaration, an optional
 * description comment, and nested declarations for classes, functions, and nested namespaces.
 */

#pragma once

#include "CodeGroupModels.h"

#include <string>

/**
 * @namespace NamespaceGenerator
 * @brief Provides functions to generate C++ code for namespaces based on DSL models.
 *
 * The NamespaceGenerator namespace contains functions that transform a DSL NamespaceModel into
 * the corresponding C++ code. It supports:
 *  - Generating the namespace declaration (header) which includes nested declarations for classes,
 *    free functions, and nested namespaces.
 *  - Generating the namespace definition (implementation), including out-of-line definitions
 *    for nested classes and functions by delegating to the appropriate generators.
 *
 * @see CodeGroupModels::NamespaceModel
 * @see ClassGenerator
 * @see CallableGenerator
 */
namespace NamespaceGenerator
{
    /**
     * @brief Generates the C++ namespace declaration from a NamespaceModel.
     *
     * This function produces the header portion for a C++ namespace. It generates:
     *  - A namespace block with the appropriate namespace keyword and name.
     *  - A Doxygen-style comment if a description is provided.
     *  - Nested declarations for classes, functions, and nested namespaces.
     *
     * @param ns The NamespaceModel containing the DSL namespace data.
     * @return A string containing the complete C++ namespace declaration.
     */
    std::string generateNamespaceDeclaration(const CodeGroupModels::NamespaceModel &ns);

    /**
     * @brief Generates the C++ namespace definition from a NamespaceModel.
     *
     * This function produces the out-of-line definitions for a namespace. It handles:
     *  - Generating definitions for nested classes and functions.
     *  - Recursively generating code for nested namespaces.
     *
     * @param ns The NamespaceModel containing the DSL namespace data.
     * @return A string containing the complete C++ namespace definition.
     */
    std::string generateNamespaceDefinition(const CodeGroupModels::NamespaceModel &ns);

} // namespace NamespaceGenerator
