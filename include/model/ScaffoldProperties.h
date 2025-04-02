/**
 * @file ScaffoldProperties.h
 * @brief Contains definitions for basic properties and types used in the scaffolder DSL.
 *
 * This file defines the fundamental data types, enumerations, and structures that represent
 * parameters and data types in our DSL. These definitions serve as the building blocks for the
 * scaffolder models and code generator, following modern C++20 best practices for type safety
 * and efficiency.
 */

 #pragma once

 #include <optional>
 #include <string>
 #include <cstdint>
 #include <vector>
 
 /**
  * @namespace ScaffoldProperties
  * @brief Contains definitions for basic properties and types used in the scaffolder DSL.
  *
  * This namespace provides essential building blocks for defining DSL elements, including data types,
  * type qualifiers, declaration specifiers, and parameters.
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
         FLOAT,     /**< Floating point number */
         DOUBLE,    /**< Double precision floating point */
         BOOL,      /**< Boolean type */
         STRING,    /**< String type */
         CHAR,      /**< Character type */
         AUTO,      /**< Auto-deduced type */
         CUSTOM     /**< Custom user-defined type */
     };
 
     /**
      * @brief Bitmask to represent qualifiers for data types.
      */
     enum class TypeQualifier : u_int8_t
     {
         NONE = 0,    /**< No qualifier */
         CONST = 1,   /**< const qualifier */
         VOLATILE = 2 /**< volatile qualifier */
     };
 
     /**
      * @brief Overloads the bitwise OR operator for TypeQualifier.
      *
      * Combines multiple TypeQualifier flags into a single bitmask.
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
      * Tests if specific flags are set in a TypeQualifier bitmask.
      *
      * @param lhs The left-hand side TypeQualifier bitmask.
      * @param rhs The TypeQualifier flag to test.
      * @return A TypeQualifier containing only the flags that are set in both operands.
      */
     inline TypeQualifier operator&(TypeQualifier lhs, TypeQualifier rhs)
     {
         return static_cast<TypeQualifier>(
             static_cast<u_int8_t>(lhs) & static_cast<u_int8_t>(rhs));
     }
 
     /**
      * @brief Checks if a specific qualifier flag is set in a TypeQualifier bitmask.
      *
      * @param qualifiers The combined TypeQualifier bitmask.
      * @param flag The specific TypeQualifier flag to check.
      * @return true if the flag is set; false otherwise.
      */
     inline bool hasQualifier(TypeQualifier qualifiers, TypeQualifier flag)
     {
         return static_cast<u_int8_t>(qualifiers & flag) != 0;
     }
 
     /**
      * @brief Represents pointer declarators for data types.
      *
      * This structure captures pointer count, reference type, and array dimensions for a data type.
      */
     struct TypeDeclarator
     {
         int ptrCount;                             /**< Number of pointer indirections */
         bool isLValReference;                     /**< True if it is an lvalue reference (&) */
         bool isRValReference;                     /**< True if it is an rvalue reference (&&) */
         std::vector<std::string> arrayDimensions; /**< Array dimensions (empty for unspecified size) */
 
         /**
          * @brief Default constructor for TypeDeclarator.
          */
         TypeDeclarator() : ptrCount(0), isLValReference(false), isRValReference(false),
                            arrayDimensions()
         {
         }
     };
 
     /**
      * @brief Represents declaration specifiers for methods.
      *
      * This structure defines specifiers that modify method behavior, such as static, inline, or constexpr.
      */
     struct DeclartionSpecifier
     {
         bool isStatic;    /**< Indicates a static method */
         bool isInline;    /**< Indicates an inline method */
         bool isConstexpr; /**< Indicates a constexpr method */
 
         /**
          * @brief Default constructor for DeclartionSpecifier.
          */
         DeclartionSpecifier() : isStatic(false), isInline(false), isConstexpr(false)
         {
         }
     };
 
     /**
      * @brief Represents a data type, either built-in or custom.
      *
      * This structure encapsulates a data type, including its base type, an optional custom name,
      * qualifiers, and declarators.
      */
     struct DataType
     {
         Types type;                            /**< Enumeration representing the base type */
         std::optional<std::string> customType; /**< Custom type name when type is CUSTOM */
         TypeQualifier qualifiers;              /**< Type qualifiers (e.g., const, volatile) */
         TypeDeclarator typeDecl;               /**< Type declarator information (pointers, references, arrays) */
 
         /**
          * @brief Constructor for DataType with only the type.
          *
          * @param t The base type (built-in or CUSTOM).
          * @param tD Optional type declarator structure.
          */
         constexpr DataType(const Types t, const TypeDeclarator &tD = TypeDeclarator())
             : type(t), customType(std::nullopt), qualifiers(TypeQualifier::NONE),
               typeDecl(std::move(tD))
         {
         }
 
         /**
          * @brief Constructor for DataType with a custom type name.
          *
          * @param t The base type (should be CUSTOM for a user-defined type).
          * @param cT The custom type name.
          * @param tQ Type qualifiers.
          * @param tD Optional type declarator structure.
          */
         constexpr DataType(const Types t, const std::optional<std::string> &cT,
                            const TypeQualifier tQ, const TypeDeclarator &tD = TypeDeclarator())
             : type(t), customType(cT), qualifiers(tQ), typeDecl(std::move(tD))
         {
         }
 
         /**
          * @brief Alternative constructor for DataType for non-custom types.
          *
          * @param t The base type.
          * @param tQ Type qualifiers.
          * @param tD Optional type declarator structure.
          */
         constexpr DataType(const Types t, const TypeQualifier tQ,
                            const TypeDeclarator &tD = TypeDeclarator())
             : type(t), customType(std::nullopt), qualifiers(tQ), typeDecl(std::move(tD))
         {
         }
     };
 
     /**
      * @brief Represents a parameter with a type and a name.
      *
      * This structure defines a parameter in the DSL, including its data type and the parameter name.
      */
     struct Parameter
     {
        DataType type;    /**< Data type of the parameter */
        std::string name; /**< Name of the parameter */
 
         /**
          * @brief Constructor for Parameter.
          *
          * @param t The data type of the parameter.
          * @param n The name of the parameter.
          */
         Parameter(const DataType t, const std::string n)
             : type(t), name(std::move(n))
         {
         }
     };
 
 } // namespace ScaffoldProperties
 