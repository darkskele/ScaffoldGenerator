# Scaffolder DSL Schema

This document defines the initial set of keywords, properties, and explicit scope markers for our custom DSL (with a suggested file extension of `.scaff`). Our goal is to create a human-readable, modular format for specifying project structures, which can then be used to generate C++ projects automatically.

## Overview

The Scaffolder DSL is a human-readable, modular language designed for specifying project structures and automatically generating C++ code. This document outlines the general DSL syntax—including keywords, properties, explicit scope markers, and nested scopes—followed by a dedicated section on method generation. Future revisions will incorporate additional constructs such as class, library, and namespace definitions.

## General Scaffolder Syntax

### Keywords

The DSL supports several top-level keywords that define the various elements of a project. The initial set includes:

- **project**  
  Defines the overall project and its high-level properties.

- **library**  
  Represents a modular library within the project.

- **class**  
  Specifies a C++ class along with its members and methods.

- **method**  
  Describes a member function with its return type, parameters, and additional properties.

- **member**  
  Defines a data member (variable) of a class.

- **function**  
  Represents a standalone function outside a class context.

- **folder**  
  Organizes elements into a file and folder structure for the generated project.

*(Additional keywords such as namespace and concept will be introduced later.)*

### Properties

Each element in the DSL may include one or more of the following properties:

- **name**  
  A unique identifier for the element.

- **version**  
  Specifies the version for a project, library, or any versionable component.

- **dependency**  
  Lists required dependencies (e.g., other libraries or build features like `cpp20` or `openmp`).

- **parameters**  
  For methods or functions, a comma-separated list of parameters defined as `name:type`.

- **data types**  
  Specifies the data types for parameters, members, and return types. Both built-in and custom types are supported.

- **template**  
  Indicates that the element is templated, with optional constraints or concepts.

- **description**  
  A human-readable explanation of the element’s purpose or functionality.

### Explicit Scope Markers

To ensure clear parsing independent of formatting, the DSL uses explicit markers for block boundaries:

- **Start of Scope:**  
  A new block begins with a dash (`-`) at the start of a line, followed by the keyword and identifier.  
  *Example:*  
  `- method doSomething:`

- **Property Lines:**  
  Inside a block, properties are defined on lines beginning with a vertical bar (`|`), using a key-value syntax.  
  *Example:*  
  `| return = void`

- **End of Scope:**  
  A block is explicitly terminated by a line containing only an underscore (`_`).  
  *Example:*  
  `_`

### Nested Scopes

Nested elements are structured using the same explicit markers. For example, a class containing multiple methods can be written as:

```
- class MyClass:
| description = "This is a sample class"
- method doSomething:
| return = void
| parameters = param1:int, param2:float
| description = "Performs a math operation"
_
- method helper:
| return = int
| parameters = x:double
| description = "A helper function"
_
_
```

In this example:
- The class block begins with `- class MyClass:` and closes with the final `_`.
- Each method within the class is a nested block with its own start and end markers.

---

## Method Generation Schema

This section details the schema specific to method generation. It leverages the general DSL syntax while adding properties and rules tailored for defining methods.

### Method Definition

- **method**  
  The keyword to define a member function.  
  *Example:*  
  `- method doSomething:`

#### Core Properties

- **return**  
  Specifies the return type of the method. It supports built-in types (e.g., `void`, `int`, `float`), custom types, and compound types with qualifiers (`const`, `volatile`), pointers (`*`), lvalue references (`&`), rvalue references (`&&`), and arrays (`[]` or `[n]`).

- **parameters**  
  A comma-separated list of parameters where each parameter follows the `name:type` format.  
  *Example:*  
  `| parameters = param1:int, param2:float`

- **description**  
  Provides a human-readable description of the method’s functionality.

- **declaration** (optional)  
  Specifies method modifiers such as `static`, `inline`, or `constexpr` that affect linkage and behavior.  
  *Example:*  
  `| declaration = static inline`

### Type Qualifiers and Modifiers

Methods support enhanced type specifications for both parameters and return values:

- **Qualifiers:**  
  Use qualifiers like `const` or `volatile` before the base type.  
  *Example:*  
  `| return = const int`

- **Pointers:**  
  Indicate pointers by appending an asterisk (`*`).  
  *Example:*  
  `| return = int*`

- **References:**  
  Use `&` for lvalue references and `&&` for rvalue references.  
  *Examples:*  
  `| return = const int&`  
  `| parameters = temp:int&&`

- **Arrays:**  
  Specify arrays with square brackets, either with unspecified size (`[]`) or fixed size (`[n]`).  
  *Example:*  
  `| return = int[10]`

- **Auto:**  
  The keyword `auto` can be used for deduced return types.  
  *Example:*  
  `| return = auto`

### Example Method DSL Snippet

The following snippet illustrates a complete method definition using the DSL:

```
- method doSomething:
| declaration = static inline
| return = void
| parameters = param1:int, param2:float
| description = "Performs a math operation"
_
```

In this example:
- The method block starts with `- method doSomething:`.
- Property lines define modifiers, return type, parameters, and a description.
- The block is explicitly closed with `_`.

---

## Class Generation Schema

This section details the schema specific to class generation in the Scaffolder DSL. It leverages the general DSL syntax while adding properties and rules tailored for defining C++ classes, including member declarations, constructors, destructors, and access specifiers.

### Class Definition

- **class**  
  The keyword used to define a C++ class along with its members, methods, constructors, and destructors.  
  *Example:*  
  `- class MyClass:`

#### Core Properties

- **description**  
  A human-readable explanation of the class’s purpose or functionality.

- **constructors**  
  An inline property that specifies the generation of default, copy, and move constructors. If no nested constructor blocks are provided, the system will generate the listed constructors automatically.  
  *Example:*  
  `| constructors = default, copy, move`

- **assignment**  
  Specifies that the copy and move assignment operators should be generated.  
  *Example:*  
  `| assignment = copy, move`

- **members**  
  A comma-separated list of data members. These are grouped by access specifier and can be defined either inline (with a shorthand property) or within nested access blocks.  
  *Example:*  
  `| members.private = int id, string name`

### Nested Scopes and Access Specifiers

Within the class block, nested scopes can be used to group declarations by access level. By default, any declaration in the class is treated as private unless enclosed in an explicit access specifier block:

- **private:**  
  All declarations within this block are private.
  
- **public:**  
  All declarations within this block are public.
  
- **protected:**  
  All declarations within this block are protected.

Additionally, constructors, destructors, and methods can be nested within these access specifier blocks.

### Constructors and Destructors

Constructors and destructors have two modes of declaration:

- **Inline Property Shorthand:**  
  Use the `constructors` property in the class block to automatically generate default, copy, and move constructors. If a constructor with parameters is needed, it must be defined in a nested block.
  
- **Nested Blocks:**  
  For custom behavior or parameterized constructors, a nested constructor block is used (e.g., `- constructor custom:`). The same applies to destructors (e.g., `- destructor default:`), allowing detailed customization.

### Example Class DSL Snippet

```
- class MyClass:
| description = "This is a sample class."
| constructors = default, copy, move
| assignment = copy, move
- private:
| members = int id, string name
- constructor custom:
| parameters = int param1, string param2
| description = "Custom constructor with parameters"
_
- destructor default:
| description = "Default destructor for MyClass"
_
- method setName:
| return = void
| parameters = name:string
| description = "Sets the name for MyClass"
_
_
- public:
- method getName:
| return = string
| description = "Retrieves the name for MyClass"
_
_
_
```