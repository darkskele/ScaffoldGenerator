/**
 * @file ScaffoldProperties.h
 * @brief Contains definitions for basic properties and types used in the scaffolder DSL.
 *
 * This file defines the fundamental data types, enums, and structures that represent
 * parameters and data types in our DSL. It provides the building blocks for the scaffolder
 * models and generator.
 */

#pragma once

#include <optional>
#include <string>
#include <cstdint>
#include <vector>

/**
 * @namespace ScaffoldProperties
 * @brief Contains definitions for basic properties and types used in the scaffolder DSL.
 */
namespace ScaffoldProperties
{
    /**
     * @brief Enum representing common data types.
     */
    enum class Types
    {
        VOID,      /**< Void type */
        INT,       /**< Signed integer */
        UINT,      /**< Unsigned integer */
        LONG,      /**< Signed long integer */
        ULONG,     /**< Unsigned long integer */
        LONGLONG,  /**< Signed long long integer */
        ULONGLONG, /**< Unsigned long long integer */
        FLOAT,     /**< Floating point */
        DOUBLE,    /**< Double precision floating point */
        BOOL,      /**< Boolean */
        STRING,    /**< String type */
        CHAR,      /**< Character */
        AUTO,      /**< Compiler derived */
        CUSTOM     /**< Custom user-defined type */
    };

    /**
     * @brief Bitmask to represent qualifiers for datatypes
     */
    enum class TypeQualifier : u_int8_t
    {
        NONE = 0,    /**< 0x00 */
        CONST = 1,   /**< 0x01 */
        VOLATILE = 2 /**< 0x02 */
    };

    /**
     * @brief Overloads the bitwise OR operator for TypeQualifier.
     *
     * This operator allows combining multiple TypeQualifier flags into a single bitmask.
     *
     * @param lhs The left-hand side TypeQualifier.
     * @param rhs The right-hand side TypeQualifier.
     * @return A new TypeQualifier representing the combined flags.
     */
    inline TypeQualifier operator|(TypeQualifier lhs, TypeQualifier rhs)
    {
        return static_cast<TypeQualifier>(
            static_cast<u_int8_t>(lhs) | static_cast<u_int8_t>(rhs));
    }

    /**
     * @brief Overloads the bitwise AND operator for TypeQualifier.
     *
     * This operator is used to test if a specific flag is set in a TypeQualifier bitmask.
     *
     * @param lhs The left-hand side TypeQualifier bitmask.
     * @param rhs The TypeQualifier flag to test.
     * @return A TypeQualifier with only the flags that are set in both operands.
     */
    inline TypeQualifier operator&(TypeQualifier lhs, TypeQualifier rhs)
    {
        return static_cast<TypeQualifier>(
            static_cast<u_int8_t>(lhs) & static_cast<u_int8_t>(rhs));
    }

    /**
     * @brief Checks if a given qualifier flag is set in the TypeQualifier bitmask.
     *
     * @param qualifiers The combined TypeQualifier bitmask.
     * @param flag The specific TypeQualifier flag to check.
     * @return true if the flag is set, false otherwise.
     */
    inline bool hasQualifier(TypeQualifier qualifiers, TypeQualifier flag)
    {
        return static_cast<u_int8_t>(qualifiers & flag) != 0;
    }

    /**
     * @brief Represents pointer declartion for data types
     */
    struct TypeDeclarator
    {
    public:
        int ptrCount;                               /**< Pointer direction level */
        bool isLValReference;                       /**< & reference */
        bool isRValReference;                       /**< && reference */
        std::vector<std::string> arrayDimensions;   /**< Empty for [] declarator */

        TypeDeclarator() : ptrCount(0), isLValReference(false), isRValReference(false), 
            arrayDimensions()
        {
        }
    };

    /**
     * @brief Represents a data type, which can be either a built-in type or a custom type.
     */
    struct DataType
    {
        Types type;                            /**< The type enumeration */
        std::optional<std::string> customType; /**< Custom type name when type is CUSTOM */
        TypeQualifier qualifiers;              /**< Type qualifiers */
        TypeDeclarator typeDecl;               /**< Type declarator */

        /**
         * @brief Constructor for DataType with only the type.
         * @param t The built-in type or CUSTOM.
         */
        constexpr DataType(const Types t, const TypeDeclarator& tD = TypeDeclarator())
            : type(t), customType(std::nullopt), qualifiers(TypeQualifier::NONE), 
            typeDecl(std::move(tD))
        {
        }

        /**
         * @brief Constructor for DataType.
         * @param t The built-in type or CUSTOM.
         * @param custom Custom type name, used if t == Types::CUSTOM.
         */
        constexpr DataType(const Types t, const std::optional<std::string> &cT,
                           const TypeQualifier tQ, const TypeDeclarator& tD = TypeDeclarator())
            : type(t), customType(cT), qualifiers(tQ), typeDecl(std::move(tD))
        {
        }

        /**
         * @brief Alternative constructor for DataType, used for non custom datatypes
         * @param t The built-in type or CUSTOM.
         * @param tQ Type qualifiers
         */
        constexpr DataType(const Types t, const TypeQualifier tQ, 
                            const TypeDeclarator& tD = TypeDeclarator())
            : type(t), customType(std::nullopt), qualifiers(tQ), typeDecl(std::move(tD))
        {
        }
    };

    /**
     * @brief Represents a parameter with a type and a name.
     */
    struct Parameter
    {
        const DataType type;    /**< Data type of the parameter */
        const std::string name; /**< Name of parameter */

        /**
         * @brief Constructor for parameter type
         * @param t Data type of the parameter
         * @param n Name of the parameter
         */
        Parameter(const DataType t, const std::string n)
            : type(t), name(std::move(n))
        {
        }
    };

} // namespace ScaffoldProperties