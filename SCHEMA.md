# Scaffolder DSL Schema

This document describes the **.scaff** file format that was created for specifying C++ project structures. The Scaffolder DSL is a human‐readable, modular language that automates the generation of C++ code and CMake configurations by describing projects, libraries, folders, namespaces, classes, free functions, and member methods.

---

## Overview

The Scaffolder DSL lets you describe an entire C++ project using a series of explicit blocks. A DSL file starts with a **project** block that sets up the global configuration (version, dependencies) and creates a root folder. The DSL then allows you to declare additional elements—such as libraries, folders, namespaces, classes, functions, and methods—in a nested hierarchy.

Files generated from a **.scaff** file are automatically split into two default directories:
- **include:** Contains all declaration files.
- **src:** Contains all implementation files.

Every folder declared in the DSL (at any level) is created in both the `include` and `src` directories. Their nested structure determines the relative file paths in each directory.

---

## Table of Contents

1. [Overview](#overview)
2. [Contents and Purpose](#contents-and-purpose)
3. [Keywords and Properties Overview](#keywords-and-properties-overview)
4. [Scoping and Formatting Rules](#scoping-and-formatting-rules)
5. [Data Types, Parameters, and Declaration Specifiers](#data-types-parameters-and-declaration-specifiers)
6. [Error Conditions](#error-conditions)
7. [Detailed Keywords](#detailed-keywords)  
   1. [Project](#project)  
   2. [Library](#library)  
   3. [Folder](#folder)  
   4. [Namespace](#namespace)  
   5. [Class](#class)  
   6. [Function](#function)  
   7. [Method](#method)
8. [File Generation](#file-generation-and-structure)
9. [Conclusion](#conclusion)

---

## Contents and Purpose

The DSL captures every aspect of a C++ project’s structure in a concise, declarative format. Every **.scaff** file must begin with a **project** block, which defines global settings (such as version and dependencies) and the root folder for the project. Nested within this block, you can declare libraries (each generating its own folder with metadata), organizational folders, namespaces, classes, and free functions. This structure not only organizes your code but also drives the generation of declarations (in `include`) and implementations (in `src`).

---

## Keywords and Properties Overview

Each DSL element is declared by a keyword and may have associated properties, defined as key–value pairs on lines that start with a vertical bar (`|`). Common properties include:

- **name:** The unique identifier for the element (usually implicit in the block header).
- **version:** Version information for projects, libraries, or versionable components.
- **dependency:** A comma-separated list of required libraries or build features (e.g., `Boost`, `cpp20`).
- **parameters:** A comma-separated list (format `name:type`) for functions and methods.
- **description:** A brief explanation of the element’s purpose.
- **declaration:** Modifiers for functions or methods (e.g., `static`, `inline`, `constexpr`).

---

## Scoping and Formatting Rules

The DSL uses explicit markers for defining blocks, regardless of how you indent the file. Indentation is optional and for readability only.

- **Start of Scope:**  
  A block begins with a dash (`-`), followed by the keyword and an optional identifier, ending with a colon.  
  *Example:*  
  `- project MyProject:`

- **Property Lines:**  
  Each property line begins with a vertical bar (`|`) followed by the key–value pair.  
  *Example:*  
  `| version = 1.0.0`

- **End of Scope:**  
  A block ends with a line containing only an underscore (`_`).  
  *Example:*  
  `_`

---

## Data Types, Parameters, and Declaration Specifiers

### Data Types

Data types are used to specify the return types for functions and methods, as well as the types for parameters and class members. The DSL recognizes several built-in types:

- **void, int, uint, long, ulong, longlong, ulonglong, float, double, bool, string, char, auto**

Any type string that does not match these is treated as a **custom** type.

### Type Qualifiers and Declarators

When specifying a data type, you can use:
- **Qualifiers:**  
  Place qualifiers like `const` and `volatile` at the beginning. For example:  
  `const int`
  
- **Declarators:**  
  Modify the type with pointers (`*`), references (`&` for lvalue or `&&` for rvalue), and arrays (using `[]` or `[n]` for fixed size). These tokens appear at the end of the type declaration. For example:  
  `int*`  
  `int&`  
  `int&&`  
  `int[10]`

The DSL’s parser extracts qualifiers and declarators to build a structured representation of the type.

### Parameter Parsing

Parameters for functions and methods are declared as a comma-separated list in the format `name:type`.  
*Example:*  
```
| parameters = id:int, name:string, values: const float*[5]
```
The parser splits the list on commas and uses the data type parser to interpret the type (including qualifiers and declarators). If a parameter is malformed (e.g., missing the colon), a parse error is thrown.

### Declaration Specifiers

The `declaration` property allows you to specify function and method modifiers:
- **static**
- **inline**
- **constexpr**

These specifiers are parsed sequentially from the provided string, and each is applied to the declaration to adjust linkage or optimization.

---

## Error Conditions

The DSL enforces strict rules to ensure consistency:

- **Multiple Project Blocks:**  
  Only one project block is allowed per file. Encountering a second or nested `- project` triggers an error.

- **Nested Libraries:**  
  Library blocks must be declared at the top level of a project; nested library blocks are forbidden.

- **Invalid Contexts:**  
  - Methods are only allowed inside classes.
  - Functions cannot be declared inside classes.
  
- **Malformed Parameters:**  
  Missing colons or improperly formatted parameters result in a parse error.

- **Misplaced Properties:**  
  Properties must appear in the initial property section of a block. Additional property lines later in the block cause errors.

- **Syntax Errors:**  
  Missing colons, leading dashes, or end-of-scope markers (`_`) produce errors.

---

## Detailed Keywords

The following sections provide details for each DSL keyword, ordered by broadness of scope.

### Project

- **Scope:** Must be the first and only project block in the DSL file.
- **Purpose:**  
  Defines the overall project, sets global properties (version, dependency), and creates the root folder (mirrored in both `include` and `src`).
- **Properties:**  
  - **name** (implicit in the header)  
  - **version**  
  - **dependency**
- **Allowed Nested Elements:**  
  Libraries, folders, namespaces, classes, and free functions.
- **Syntax Example:**
  ```
  - project MyProject:
  | version = 1.0.0
  | dependency = Boost, cpp20
  ... (nested content)
  _
  ```
- **Error Conditions:**  
  - A second or nested `- project` block is disallowed.
  - Methods or invalid keywords in this block trigger errors.

### Library

- **Scope:** Must be declared directly within a project; cannot be nested inside other elements.
- **Purpose:**  
  Represents a modular library that creates its own folder (in both `include` and `src`) and carries metadata (version, dependency).
- **Properties:**  
  - **name** (implicit)  
  - **version**  
  - **dependency**
- **Allowed Nested Elements:**  
  Folders, classes, namespaces, and free functions.
- **Syntax Example:**
  ```
  - library MyLibrary:
  | version = 1.2.3
  | dependency = cpp20
  - folder core:
    - class Logger:
    | description = "Provides logging functionality"
    - method log:
    | return = void
    | parameters = message:string
    | description = "Logs a message"
    _
    _
  _
  ```
- **Error Conditions:**  
  - Nested library blocks trigger an error.
  - Methods declared directly in a library block are invalid.

### Folder

- **Scope:** Can appear at any level (project, library, namespace) for organizational purposes.
- **Purpose:**  
  Organizes code into folders. Each folder is generated in both `include` and `src` directories, with nested elements split accordingly into header (declaration) and source (implementation) files.
- **Properties:**  
  - **name** (implicit)
- **Allowed Nested Elements:**  
  Classes, functions, namespaces, and even other folders.
- **Syntax Example:**
  ```
  - folder core:
  ... (nested content)
  _
  ```
- **Error Conditions:**  
  - A folder must have a valid identifier.
  - Misplaced property lines produce errors.

### Namespace

- **Scope:** Declared at the project level or nested within folders or libraries.
- **Purpose:**  
  Defines a C++ namespace to group related classes and free functions.
- **Properties:**  
  - **name** (optional for anonymous namespaces)  
  - **description**
- **Allowed Nested Elements:**  
  Classes, free functions, and nested namespaces.
- **Syntax Example:**
  ```
  - namespace MyNamespace:
  | description = "Groups related classes and functions."
  ... (nested classes and functions)
  _
  ```
- **Error Conditions:**  
  - Methods cannot be declared directly within a namespace.
  - Invalid nesting or missing markers trigger errors.

### Class

- **Scope:** Declared within folders, libraries, or namespaces.
- **Purpose:**  
  Defines a C++ class with its members, methods, constructors, and destructors.
- **Properties:**  
  - **name** (implicit)  
  - **description**  
  - **constructors** (for auto-generation of default, copy, and move constructors)  
  - **assignment** (for copy/move assignment operators)  
  - **members** (grouped by access specifier)
- **Allowed Nested Elements:**  
  Methods, constructors, destructors, and member variables.
- **Syntax Example:**
  ```
  - class MyClass:
  | description = "This is a sample class."
  | constructors = default, copy, move
  | assignment = copy, move
  - private:
  | members = id:int, name:string
  - constructor custom:
  | parameters = param1:int, param2:string 
  | description = "Custom constructor"
  _
  - destructor default:
  | description = "Default destructor"
  _
  - method setName:
  | return = void
  | parameters = name:string
  | description = "Sets the class name"
  _
  _
  - public:
  - method getName:
  | return = string
  | description = "Retrieves the class name"
  _
  _
  _
  ```
- **Error Conditions:**  
  - Free functions or methods declared outside allowed contexts cause errors.
  - Malformed parameters trigger parse errors.

### Function

- **Scope:** Must be declared at the global or namespace level (not within a class).
- **Purpose:**  
  Describes a free-standing function with its return type, parameters, and optional declaration modifiers.
- **Properties:**  
  - **name** (implicit)  
  - **return**  
  - **parameters**  
  - **description**  
  - **declaration** (optional)
- **Syntax Example:**
  ```
  - function doSomething:
  | declaration = inline constexpr
  | return = int
  | parameters = param1:int, param2:float
  | description = "Performs a calculation"
  _
  ```
- **Error Conditions:**  
  - Declaring a function inside a class is invalid.
  - Missing or misformatted properties produce errors.

### Method

- **Scope:** Must be declared within a class.
- **Purpose:**  
  Specifies a member function with its return type, parameters, and optional declaration modifiers.
- **Properties:**  
  - **name** (implicit)  
  - **return**  
  - **parameters**  
  - **description**  
  - **declaration** (optional)
- **Syntax Example:**
  ```
  - method doSomething:
  | declaration = static inline
  | return = void
  | parameters = param1:int, param2:float
  | description = "Executes a class operation"
  _
  ```
- **Error Conditions:**  
  - Methods declared outside a class trigger an error.
  - Invalid parameter formats or missing end-of-scope markers produce errors.

## File Generation and Structure

In the Scaffolder DSL, the concept of a “file” is defined by the scope in which elements appear. The broadest scopes—those that appear directly under a project, folder, or library—are considered top-level and will generate individual files. Each file is actually split into two parts:
- **Header File (in the `include` folder):** Contains declarations.
- **Implementation File (in the `src` folder):** Contains definitions/implementations.

The rules for file generation are as follows:

- **Namespaces:**  
  A namespace declared at the project, folder, or library level generates its own file. This file contains all the contents of the namespace (such as nested classes and free functions) and is split into header and source parts.

- **Classes:**  
  A class that appears directly at the project, folder, or library level also generates its own set of files. This ensures that the class declaration is placed in the header file, with its corresponding implementations (methods, constructors, etc.) placed in the source file.

- **Free Functions:**  
  Collections of free functions declared at one of these broader scopes (project, folder, or library) are grouped together to form a file. The functions are split so that declarations appear in the header file and definitions in the source file.

In essence, any DSL element that is not nested within another file-level element (i.e., beyond the folder level) is treated as a candidate for file generation. This design keeps the generated project structure organized, with a clear separation between interface (declarations in `include`) and implementation (definitions in `src`).

---

## Conclusion

The Scaffolder DSL provides a comprehensive, modular way to define C++ project structures. This schema document outlines every aspect of the DSL—from global settings in the project block to the detailed handling of types, parameters, and declaration specifiers, as well as error conditions and file generation rules. By following this schema, you can create a **.scaff** file that drives automatic, consistent code generation and build configuration for your C++ projects.