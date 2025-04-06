/**
 * @file ClassModels.h
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This file defines the structures that represent various DSL elements such as methods,
 * classes, libraries, etc. These models are used by the generator to create code from
 * the high-level DSL. The models adhere to modern C++23 practices, emphasizing immutability,
 * efficient resource management, and robust type safety.
 */

#pragma once

#include "PropertiesModels.h"
#include "CallableModels.h"

#include <vector>
#include <string>
#include <optional>

/**
 * @namespace ClassModels
 * @brief Contains the model definitions for the scaffolder DSL.
 *
 * This namespace includes structures that represent various DSL elements used by the code generator.
 * The models are designed to be immutable and to facilitate efficient code generation.
 */
namespace ClassModels
{
    /**
     * @brief Represents a special member function common to constructors and destructors.
     *
     * This structure encapsulates properties that are shared between constructors and destructors,
     * such as a description. It promotes immutability and consistency across special member functions.
     */
    struct SpecialMemberFunction
    {
        std::string description; /**< Description of the special member function */

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
        ConstructorType type;                                /**< The type of constructor */
        std::vector<PropertiesModels::Parameter> parameters; /**< Constructor parameters (empty for default, copy, or move) */

        /**
         * @brief Constructor for the Constructor model.
         *
         * @param type The type of constructor (default, copy, move, or custom).
         * @param params The parameters for the constructor. For non-custom constructors, this should be empty.
         * @param desc A description of the constructor.
         */
        Constructor(ConstructorType type,
                    const std::vector<PropertiesModels::Parameter> &params,
                    const std::string &desc)
            : SpecialMemberFunction(desc), type(type), parameters(params)
        {
        }
    };

    /**
     * @brief Model representing a destructor.
     *
     * This structure is a specialized version of SpecialMemberFunction that represents
     * a class destructor. It cannot have parameters or overloads.
     */
    struct Destructor : public SpecialMemberFunction
    {
        /**
         * @brief Constructs a Destructor with a description.
         * @param desc The description of the destructor.
         */
        explicit Destructor(const std::string &desc) : SpecialMemberFunction(desc) {}

        // Make move/copy constructors defaulted so it can be used with std::optional
        Destructor(const Destructor &) = default;
        Destructor(Destructor &&) = default;
        Destructor &operator=(const Destructor &) = default;
        Destructor &operator=(Destructor &&) = default;
    };

    /**
     * @brief Represents a C++ class as defined in the scaffolder DSL.
     *
     * This structure models a C++ class declaration and definition, capturing
     * data members, methods, constructors, destructors, and metadata like description
     * and assignment operators. It is the central structure used to translate DSL
     * class definitions into actual header and source files.
     */
    struct ClassModel
    {
        std::string name; /**< Name of the class */

        std::string description; /**< Doxygen-style description of the class */

        std::vector<Constructor> constructors; /**< All defined constructors, including default/copy/move/custom */
        std::optional<Destructor> destructor;  /**< Optional destructor if defined in the DSL */

        std::vector<CallableModels::MethodModel> publicMethods;    /**< All public methods */
        std::vector<CallableModels::MethodModel> privateMethods;   /**< All private methods */
        std::vector<CallableModels::MethodModel> protectedMethods; /**< All protected methods */

        std::vector<PropertiesModels::Parameter> publicMembers;    /**< Public data members (for simplicity, same struct as params) */
        std::vector<PropertiesModels::Parameter> privateMembers;   /**< Private data members */
        std::vector<PropertiesModels::Parameter> protectedMembers; /**< Protected data members */

        bool hasCopyAssignment; /**< True if copy assignment operator should be generated */
        bool hasMoveAssignment; /**< True if move assignment operator should be generated */

        /**
         * @brief Constructor for ClassModel.
         *
         * @param n Name of the class.
         * @param desc Human-readable description for Doxygen.
         * @param ctors A list of constructor definitions.
         * @param dtor Optional destructor definition.
         * @param pubMethods List of public method definitions.
         * @param privMethods List of private method definitions.
         * @param protMethods List of protected method definitions.
         * @param pubMembers List of public data members.
         * @param privMembers List of private data members.
         * @param protMembers List of protected data members.
         * @param copyAssign Whether the copy assignment operator should be generated.
         * @param moveAssign Whether the move assignment operator should be generated.
         */
        ClassModel(
            const std::string &n,
            const std::string &desc,
            const std::vector<Constructor> &ctors,
            const std::optional<Destructor> &dtor,
            const std::vector<CallableModels::MethodModel> &pubMethods,
            const std::vector<CallableModels::MethodModel> &privMethods,
            const std::vector<CallableModels::MethodModel> &protMethods,
            const std::vector<PropertiesModels::Parameter> &pubMembers,
            const std::vector<PropertiesModels::Parameter> &privMembers,
            const std::vector<PropertiesModels::Parameter> &protMembers,
            bool copyAssign,
            bool moveAssign)
            : name(n),
              description(desc),
              constructors(ctors),
              destructor(dtor),
              publicMethods(pubMethods),
              privateMethods(privMethods),
              protectedMethods(protMethods),
              publicMembers(pubMembers),
              privateMembers(privMembers),
              protectedMembers(protMembers),
              hasCopyAssignment(copyAssign),
              hasMoveAssignment(moveAssign)
        {
        }
    };

} // namespace ClassModels
