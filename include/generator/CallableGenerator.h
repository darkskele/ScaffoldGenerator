/**
 * @file CallableGenerator.h
 * @brief Contains pure functions to generate callable (free function and method) declarations and definitions.
 *
 * This file defines a set of composable, pure functions that generate callable signatures
 * and bodies for a given DSL keyword scope. The generated code adheres to modern C++20+ best practices,
 * emphasizing efficiency, clarity, and robust error handling.
 *
 * The functions declared in this header produce both free function declarations/definitions
 * and method declarations/definitions. The base generators operate on the common base model (CallableModel)
 * so that they can accept either a free function or a method. The method generators then wrap the base functions
 * to adjust formatting (such as indentation for declarations and class qualification for definitions).
 *
 * @note The implementation leverages the
 * <a href="https://en.cppreference.com/w/cpp/utility/format">std::format</a>
 * library for efficient and type-safe string formatting.
 */

#pragma once

#include "ScaffoldModels.h"
#include <string>

/**
 * @namespace CallableGenerator
 * @brief Provides functions to generate callable (free function and method) declarations and definitions.
 *
 * This namespace offers a set of pure, composable functions for code generation.
 * It includes two groups of functions:
 *  - **Base generators** (generateCallableDeclaration and generateCallableDefinition) which produce
 *    a callable’s declaration and definition from the common base model.
 *  - **Wrapper generators** for free functions and methods. Free function generators are inline
 *    aliases of the base generators, while method generators adjust the base output:
 *     - The method declaration generator indents the output for inclusion within a class.
 *     - The method definition generator qualifies the function name with the owning class name.
 *
 * @sa ScaffoldModels, PropertiesGenerator, GeneratorUtilities
 */
namespace CallableGenerator
{
    //--------------------------------------------------------------------------
    // Base Generators (operating on the common base model)
    //--------------------------------------------------------------------------

    /**
     * @brief Generates a callable declaration string.
     *
     * This function creates a declaration (signature) string for a callable (function or method)
     * as it would appear in a header file. It uses the common properties defined in the base model
     * (CallableModel) to incorporate the return type, parameter list, and declaration specifiers.
     * A Doxygen comment block is prepended to the declaration.
     *
     * @param callable The callable's properties (from the common base model).
     * @return A std::string representing the complete callable declaration.
     *
     * @throws std::runtime_error If any property of the callable is invalid.
     */
    std::string generateCallableDeclaration(const ScaffoldModels::CallableModel &callable);

    /**
     * @brief Generates a callable definition string.
     *
     * This function creates a complete definition string for a callable (function or method)
     * that includes the function signature and a default body that throws a std::runtime_error,
     * indicating that the callable is not yet implemented.
     *
     * @param callable The callable's properties (from the common base model).
     * @return A std::string representing the complete callable definition.
     *
     * @throws std::runtime_error If any property of the callable is invalid.
     */
    std::string generateCallableDefinition(const ScaffoldModels::CallableModel &callable);

    //--------------------------------------------------------------------------
    // Free Function Generators (aliasing the base generators)
    //--------------------------------------------------------------------------

    /**
     * @brief Generates a free function declaration string.
     *
     * This inline function calls generateCallableDeclaration using a FunctionModel.
     *
     * @param func The FunctionModel containing the free function's properties.
     * @return A std::string representing the free function declaration.
     *
     * @throws std::runtime_error If any property of the function is invalid.
     */
    inline std::string generateFunctionDeclaration(const ScaffoldModels::FunctionModel &func)
    {
        return generateCallableDeclaration(func);
    }

    /**
     * @brief Generates a free function definition string.
     *
     * This inline function calls generateCallableDefinition using a FunctionModel.
     *
     * @param func The FunctionModel containing the free function's properties.
     * @return A std::string representing the free function definition.
     *
     * @throws std::runtime_error If any property of the function is invalid.
     */
    inline std::string generateFunctionDefinition(const ScaffoldModels::FunctionModel &func)
    {
        return generateCallableDefinition(func);
    }

    //--------------------------------------------------------------------------
    // Method Generators (wrap the base generators)
    //--------------------------------------------------------------------------

    /**
     * @brief Generates a method declaration string formatted for inclusion in a class.
     *
     * This function wraps generateCallableDeclaration to produce a declaration suitable
     * for a method. It indents the generated declaration (using a 4-space indent) so that it
     * appears correctly when placed inside a class definition.
     *
     * @param method The MethodModel containing the method's properties.
     * @return A std::string representing the indented method declaration.
     *
     * @throws std::runtime_error If any property of the method is invalid.
     */
    std::string generateMethodDeclaration(const ScaffoldModels::MethodModel &method);

    /**
     * @brief Generates a method definition string with class qualification.
     *
     * This function wraps generateCallableDefinition by modifying the free function definition so that
     * the callable name is qualified with the owning class name (e.g., ClassName::MethodName). This produces
     * a method definition suitable for inclusion in a source file.
     *
     * @param className The name of the class that owns this method.
     * @param method The MethodModel containing the method's properties.
     * @return A std::string representing the fully qualified method definition.
     *
     * @throws std::runtime_error If any property of the method is invalid.
     */
    std::string generateMethodDefinition(const std::string &className, const ScaffoldModels::MethodModel &method);
}
