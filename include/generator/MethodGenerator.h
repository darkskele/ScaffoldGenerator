/**
 * @file MethodGenerator.h
 * @brief Contain pure functions to generate Methods.
 *
 * This file defines a set of composable pure functions to generate Methods
 * within a keyword scope from a scaff file.
 */

#pragma once

#include "ScaffoldModels.h"

/**
 * @namespace MethodGenerator
 * @brief Encapsulates pure functions that generate Methods
 */
namespace MethodGenerator
{
    /**
     * @brief Generates the method declaration string.
     *
     * This function creates the declaration (signature) for a method as it would appear
     * in a header file.
     *
     * Example output:
     *     void doSomething(int param1, float param2);
     *
     * @param method The MethodModel containing the method's properties.
     * @return A string with the method declaration.
     */
    std::string generateMethodDeclaration(const ScaffoldModels::MethodModel &method);

    /**
     * @brief Generates the method definition string.
     *
     * This function creates the definition for a method, including the fully qualified
     * method name (using the class name) and a default method body.
     *
     * Example output:
     *     void MyClass::doSomething(int param1, float param2) {
     *         throw std::runtime_error("Not implemented");
     *     }
     *
     * @param className The name of the class that owns this method.
     * @param method The MethodModel containing the method's properties.
     * @return A string with the fully defined method.
     */
    std::string generateMethodDefinition(const std::string& className, const ScaffoldModels::MethodModel &method);

} // namespace MethodGenerator