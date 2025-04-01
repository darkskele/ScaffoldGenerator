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
        CUSTOM     /**< Custom user-defined type */
    };

    /**
     * @brief Represents a data type, which can be either a built-in type or a custom type.
     */
    struct DataType
    {
        Types type;                            /**< The type enumeration */
        std::optional<std::string> customType; /**< Custom type name when type is CUSTOM */

        /**
         * @brief Constructor for DataType.
         * @param t The built-in type or CUSTOM.
         * @param custom Optional custom type name, used if t == Types::CUSTOM.
         */
        constexpr DataType(const Types t, const std::optional<std::string> &cT = std::nullopt)
            : type(t), customType(cT)
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