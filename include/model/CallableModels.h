/**
 * @file CallableModels.h
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This file defines the structures that represent various DSL elements such as methods,
 * classes, libraries, etc. These models are used by the generator to create code from
 * the high-level DSL. The models adhere to modern C++23 practices, emphasizing immutability,
 * efficient resource management, and robust type safety.
 */

#pragma once

#include "PropertiesModels.h"

#include <vector>
#include <string>
#include <optional>

/**
 * @namespace CallableModels
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This namespace includes structures that represent various DSL elements used by the code generator.
 * The models are designed to be immutable and to facilitate efficient code generation.
 */
namespace CallableModels
{
    /**
     * @brief Represents a callable entity, such as a function or a method.
     *
     * This base model encapsulates common properties shared by methods and functions,
     * including the return type, name, parameters, declaration specifiers, and a description.
     */
    struct CallableModel
    {
        /// The return type of the callable.
        PropertiesModels::DataType returnType;
        /// The name of the callable.
        std::string name;
        /// A vector of parameters associated with the callable.
        std::vector<PropertiesModels::Parameter> parameters;
        /// Declaration specifiers that modify the callable's behavior (e.g., inline, static).
        PropertiesModels::DeclartionSpecifier declSpec;
        /// A description of the callable.
        std::string description;

        /**
         * @brief Constructor for CallableModel.
         *
         * Initializes a new instance of CallableModel with the specified return type, name, parameters,
         * declaration specifiers, and an optional description.
         *
         * @param retType The return type of the callable.
         * @param n The name of the callable.
         * @param params A vector of parameters associated with the callable.
         * @param dC Declaration specifiers that modify the callable's behavior (e.g., inline, static).
         * @param desc Optional description of the callable. Defaults to a single space.
         */
        CallableModel(const PropertiesModels::DataType retType, std::string n,
                      std::vector<PropertiesModels::Parameter> params,
                      const PropertiesModels::DeclartionSpecifier &dC,
                      std::string desc = "")
            : returnType(retType), name(std::move(n)), parameters(std::move(params)),
              declSpec(std::move(dC)), description(std::move(desc))
        {
        }
    };

    /**
     * @brief Represents a method in a class.
     *
     * Inherits common callable properties from CallableModel.
     */
    struct MethodModel : public CallableModel
    {
        /**
         * @brief Inherits the constructor from CallableModel.
         */
        using CallableModel::CallableModel;
    };

    /**
     * @brief Represents a free function.
     *
     * Inherits common callable properties from CallableModel.
     */
    struct FunctionModel : public CallableModel
    {
        /**
         * @brief Inherits the constructor from CallableModel.
         */
        using CallableModel::CallableModel;
    };

} // namespace CallableModels
