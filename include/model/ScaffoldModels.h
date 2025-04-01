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
         const std::string name;                                        /**< Name of the method */
         const std::vector<ScaffoldProperties::Parameter> parameters;   /**< Method parameters */
         const ScaffoldProperties::DeclartionSpecifier declSpec;        /**< Declaration specifiers */
         const std::string description;                                 /**< Description of the method */
 
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
 
 } // namespace ScaffoldModels
 