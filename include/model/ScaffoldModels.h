/**
 * @file ScaffoldModels.h
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This file defines the structures that represent various DSL elements such as methods,
 * classes, libraries, etc. These models are used by the generator to create code from
 * the high-level DSL. The models adhere to modern C++20 practices, emphasizing immutability,
 * efficient resource management, and robust type safety.
 */

#pragma once

#include "ScaffoldProperties.h"
#include <vector>
#include <string>

/**
 * @namespace ScaffoldModels
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This namespace includes structures that represent various DSL elements used by the code generator.
 * The models are designed to be immutable and to facilitate efficient code generation.
 */
namespace ScaffoldModels
{
    /**
     * @brief Model representing a method or function.
     *
     * This structure encapsulates all properties required to define a method within the DSL.
     * It includes the return type, method name, parameters, declaration specifiers, and a description.
     *
     * @note All members are declared as const to promote immutability and thread safety.
     */
    struct MethodModel
    {
        const ScaffoldProperties::DataType returnType;               /**< Return type of the method */
        const std::string name;                                      /**< Name of the method */
        const std::vector<ScaffoldProperties::Parameter> parameters; /**< Method parameters */
        const ScaffoldProperties::DeclartionSpecifier declSpec;      /**< Declaration specifiers */
        const std::string description;                               /**< Description of the method */

        /**
         * @brief Constructor for MethodModel.
         *
         * Initializes a new instance of MethodModel with the specified return type, name, parameters,
         * declaration specifiers, and an optional description.
         *
         * @param retType The return type of the method.
         * @param n The name of the method.
         * @param params A vector of parameters associated with the method.
         * @param dC Declaration specifiers that modify the method's behavior (e.g., inline, static).
         * @param desc Optional description of the method. Defaults to a single space.
         */
        MethodModel(const ScaffoldProperties::DataType retType, const std::string n,
                    const std::vector<ScaffoldProperties::Parameter> params,
                    const ScaffoldProperties::DeclartionSpecifier &dC,
                    const std::string desc = " ")
            : returnType(retType), name(std::move(n)), parameters(std::move(params)),
              declSpec(std::move(dC)), description(std::move(desc))
        {
        }
    };

    /**
     * @brief Represents a special member function common to constructors and destructors.
     *
     * This structure encapsulates properties that are shared between constructors and destructors,
     * such as a description. It promotes immutability and consistency across special member functions.
     */
    struct SpecialMemberFunction
    {
        const std::string description; /**< Description of the special member function */

        /**
         * @brief Constructor for SpecialMemberFunction.
         * @param desc A description of the special member function.
         */
        SpecialMemberFunction(const std::string &desc) : description(desc)
        {
        }
    };

    /**
     * @brief Enumerates the types of constructors available.
     */
    enum class ConstructorType
    {
        DEFAULT, /**< Default constructor */
        COPY,    /**< Copy constructor */
        MOVE,    /**< Move constructor */
        CUSTOM   /**< Custom constructor with user-specified parameters */
    };

    /**
     * @brief Model representing a constructor for a class.
     *
     * This structure inherits common properties from SpecialMemberFunction and includes
     * constructor-specific details such as its type and parameters. Constructors defined
     * via the DSL may be auto-generated using an inline property (e.g., "default, copy, move")
     * or explicitly defined in nested blocks for custom behavior.
     */
    struct Constructor : public SpecialMemberFunction
    {
        const ConstructorType type;                                  /**< The type of constructor */
        const std::vector<ScaffoldProperties::Parameter> parameters; /**< Constructor parameters (empty for default, copy, or move) */

        /**
         * @brief Constructor for the Constructor model.
         *
         * @param type The type of constructor (default, copy, move, or custom).
         * @param params The parameters for the constructor. For non-custom constructors, this should be empty.
         * @param desc A description of the constructor.
         */
        Constructor(ConstructorType type,
                    const std::vector<ScaffoldProperties::Parameter> &params,
                    const std::string &desc)
            : SpecialMemberFunction(desc), type(type), parameters(params)
        {
        }
    };

    /**
     * @brief Model representing a destructor for a class.
     *
     * This structure inherits common properties from SpecialMemberFunction.
     * Unlike constructors, destructors do not accept parameters.
     */
    struct Destructor : public SpecialMemberFunction
    {
        /**
         * @brief Constructor for the Destructor model.
         *
         * @param desc A description of the destructor.
         */
        Destructor(const std::string &desc)
            : SpecialMemberFunction(desc)
        {
        }
    };
    

} // namespace ScaffoldModels
