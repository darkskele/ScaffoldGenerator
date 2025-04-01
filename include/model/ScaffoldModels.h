/**
 * @file ScaffoldModels.h
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This file defines the structures that represent various DSL elements such as methods,
 * classes, libraries, etc. These models are used by the generator to create code from
 * the high-level DSL.
 */

#pragma once

#include "ScaffoldProperties.h"
#include <vector>

/**
 * @namespace ScaffoldModels
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This namespace includes structures that represent the various DSL elements,
 * such as methods, classes, libraries, etc., which are used by the generator.
 */
namespace ScaffoldModels
{
    /**
     * @brief Model representing a method or function.
     */
    struct MethodModel
    {
        const ScaffoldProperties::DataType returnType;               /**< Return type of method */
        const std::string name;                                      /**< Name of method */
        const std::vector<ScaffoldProperties::Parameter> parameters; /**< Method parameters */
        const std::string description;                               /**< Method description */

        /**
         * @brief Constructor for MethodModel.
         * @param retType The return type.
         * @param n The method name.
         * @param params A vector of parameters.
         * @param desc Optional description of the method.
         */
        MethodModel(const ScaffoldProperties::DataType retType, const std::string n,
                    const std::vector<ScaffoldProperties::Parameter> params,
                    const std::string desc = " ")
                : returnType(retType), name(std::move(n)), parameters(std::move(params)),
                description(std::move(desc))
                {
                }
    };

} // namespace ScaffoldModels