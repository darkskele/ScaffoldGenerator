/**
 * @file MethodGenerator.h
 * @brief Contains pure functions to generate method declarations and definitions.
 *
 * This file defines a set of composable, pure functions that generate method signatures
 * and bodies for a given DSL keyword scope. The generated code adheres to modern C++20+ best practices,
 * emphasizing efficiency, clarity, and robust error handling.
 *
 * The functions declared in this header are designed to produce both the method declaration
 * (as it would appear in a header file) and the method definition (with a default body) suitable
 * for a C++ project scaffold.
 *
 * @note The implementation leverages the <a href="https://en.cppreference.com/w/cpp/utility/format">std::format</a>
 * library for efficient and type-safe string formatting.
 */

 #pragma once

 #include "ScaffoldModels.h"
 #include <string>
 
 /**
 * @namespace MethodGenerator
 * @brief Encapsulates pure functions for generating method declarations and definitions.
 *
 * This namespace offers composable, pure functions that convert a MethodModel into its corresponding
 * C++ declaration and definition strings. The implementations leverage modern C++20 features—such as
 * <a href="https://en.cppreference.com/w/cpp/utility/format">std::format</a> for type-safe string formatting—
 * ensuring efficient and robust code generation.
 *
 * @details The provided functions include:
 *  - **generateMethodDeclaration:** Produces a method declaration string (suitable for header files)
 *    with an accompanying Doxygen comment block detailing the method's purpose.
 *  - **generateMethodDefinition:** Produces a fully qualified method definition string that includes
 *    a default body (throwing a std::runtime_error) to indicate unimplemented functionality.
 *
 * @note All functions within this namespace are designed to be pure and exception-safe, supporting
 *       unit testing and promoting side-effect free operations. They adhere to high standards of
 *       clarity, efficiency, and maintainability in line with modern C++20 best practices.
 *
 * @sa ScaffoldModels, PropertiesGenerator, GeneratorUtilities
 */
 namespace MethodGenerator
 {
     /**
      * @brief Generates the method declaration string.
      *
      * This function creates the method's declaration (signature) string as it would appear in a header file.
      * It incorporates the return type, parameter list, and any declaration specifiers, along with a brief
      * Doxygen comment block containing the method description.
      *
      * Example output:
      * @code
      *     /**
      *      * @brief Performs a calculation.
      *      *\/
      *     inline int doSomething(int param1, float param2);
      * @endcode
      *
      * @param method The MethodModel containing the method's properties.
      * @return A std::string representing the complete method declaration.
      *
      * @throws std::runtime_error If any property of the method is invalid.
      */
     std::string generateMethodDeclaration(const ScaffoldModels::MethodModel &method);
 
     /**
      * @brief Generates the method definition string.
      *
      * This function creates the full method definition string, including the fully qualified
      * method name (qualified with the class name) and a default method body that throws an exception.
      * The generated method body serves as a placeholder, indicating that the method is not yet implemented.
      *
      * Example output:
      * @code
      *     inline int MyClass::doSomething(int param1, float param2) {
      *         throw std::runtime_error("Not implemented");
      *     }
      * @endcode
      *
      * @param className The name of the class that owns this method.
      * @param method The MethodModel containing the method's properties.
      * @return A std::string representing the fully defined method.
      *
      * @throws std::runtime_error If any property of the method is invalid.
      */
     std::string generateMethodDefinition(const std::string& className, const ScaffoldModels::MethodModel &method);
 } // namespace MethodGenerator
 