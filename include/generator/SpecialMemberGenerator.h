/**
 * @file SpecialMemberGenerator.h
 * @brief Functions for generating constructor declarations and definitions for scaffolder DSL models.
 */
#pragma once

#include "ScaffoldModels.h"

/**
 * @namespace SpecialMemberGenerator
 * @brief Contains functions for generating code related to special member functions (constructors)
 *        in a C++ class as defined by the scaffolder DSL.
 */
namespace SpecialMemberGenerator
{
    /**
     * @brief Generates the constructor declaration for a class.
     *
     * This function creates the declaration of a constructor for the given class name and constructor model.
     * It supports custom, copy, move, and default constructors. For default constructors, additional
     * declarations for the copy and move constructors are also generated.
     *
     * @param className The name of the class.
     * @param ctor The constructor model containing type, parameters, and description.
     * @return A string containing the generated constructor declaration(s).
     *
     * @exception std::runtime_error if an unrecognised constructor type is provided.
     */
    std::string generateConstructorDeclaration(const std::string& className, const ScaffoldModels::Constructor &ctor);

    /**
     * @brief Generates the constructor definition for a class.
     *
     * This function generates the definition of a constructor for the specified class based on the
     * provided constructor model and the members defined in various access specifier groups.
     *
     * For a custom constructor, the parameters are used along with an initializer list for all members.
     * For copy and move constructors, the definition includes an initializer list based on the members.
     * For a default constructor, no definition is generated since the compiler will generate it automatically.
     *
     * @param className The name of the class.
     * @param ctor The constructor model containing type, parameters, and description.
     * @param publicMembers A vector of public member parameters.
     * @param privateMembers A vector of private member parameters.
     * @param protectedMembers A vector of protected member parameters.
     * @return A string containing the generated constructor definition.
     *
     * @exception std::runtime_error if an unrecognised constructor type is provided.
     */
    std::string generateConstructorDefinition(const std::string& className, const ScaffoldModels::Constructor &ctor,
        const std::vector<ScaffoldProperties::Parameter> publicMembers,
        const std::vector<ScaffoldProperties::Parameter> privateMembers,
        const std::vector<ScaffoldProperties::Parameter> protectedMembers);

} // namespace SpecialMemberGenerator
