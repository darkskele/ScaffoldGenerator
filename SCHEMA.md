# Scaffolder DSL Schema - Expanded Draft

This document defines the initial set of keywords, properties, and explicit scope markers for our custom DSL (with a suggested file extension of `.scaff`). Our goal is to create a human-readable, modular format for specifying project structures, which can then be used to generate C++ projects automatically.

## Overview

Our DSL allows you to define a project structure using a simple syntax that leverages both indentation for readability and explicit scope markers for unambiguous parsing. The high-level design (such as project dependencies, libraries, classes, methods, and more) is expressed in one or more files that the scaffolder tool will parse and then use to generate all necessary project files (CMake, VS Code settings, etc.).

While whitespace and indentation are encouraged for human readability, the actual scope boundaries are defined by explicit markers:

- **Start of Scope:** A new block begins with a dash (`-`) at the start of a line.
- **End of Scope:** A block is explicitly closed by a line containing a single underscore (`_`).

This approach ensures that the structure of the DSL is robust, even if formatting is inconsistent.

## Keywords

The following top-level keywords define the various elements in our project:

- **project**  
  Defines the overall project and its high-level properties.

- **library**  
  Represents a modular library within the project. Libraries can be marked as static or dynamic and encapsulate related functionalities.

- **class**  
  Specifies a C++ class and its related details such as inheritance, methods, and members.

- **method**  
  Describes a member function within a class. It includes the return type, parameters, and an optional description or placeholder implementation.

- **member**  
  Defines a data member (variable) of a class.

- **function**  
  Represents a standalone function that exists outside of a class context.

- **folder**  
  Used to organize elements into a file/folder structure, guiding how files are laid out in the generated project.

## Properties

Each element defined by the keywords can have associated properties. These include:

- **name**  
  A unique identifier for the element.

- **version**  
  Specifies the version for a project, library, or any versionable component.

- **dependency**  
  Lists dependencies required by the element (e.g., other libraries or modules, or even specific build features like `cpp20` or `openmp`).

- **parameters**  
  For methods or functions, this property defines the list of parameters, including their names and data types.

- **data types**  
  Specifies the data types for parameters, members, and return types. This will support advanced features such as templates and concepts (to be expanded later).

- **template**  
  (Optional) Indicates that the element (class or method) is templated, possibly with specific constraints or concepts.

- **description**  
  A human-readable text describing the element's purpose or functionality, useful for documentation and clarity.

## Explicit Scope Markers

To clearly define the beginning and end of each DSL block, we introduce explicit scope markers:

- **Start of Scope:**  
  Each new block begins with a dash (`-`) at the start of a line.  
  Example:  
  `- method doSomething:`

- **Property Lines:**  
  Within a scope, properties are defined on lines that start with a vertical bar (`|`), using a key-value syntax.  
  Example:  
  `| return = void`

- **End of Scope:**  
  A scope is explicitly closed by a line that contains only an underscore (`_`).  
  Example:  
  `_`

This mechanism allows the parser to detect block boundaries independently of whitespace or indentation.

## Example DSL Snippet

Below is an example that illustrates the intended format for a method, with explicit scope markers:

```
- method doSomething:
| return = void
| parameters = param1:int, param2:float
| description = "Performs a math operation"
_
```

### Explanation

1. **Header Line:**  
   `- method doSomething:`  
   - The dash (`-`) indicates the beginning of a new block.
   - The keyword `method` specifies that this block defines a method.
   - `doSomething:` is the method name, with the trailing colon signaling that details follow.

2. **Property Lines:**  
   Each property line starts with `|`:
   - `| return = void` specifies that the method returns `void`.
   - `| parameters = param1:int, param2:float` defines a comma-separated list of parameters, with each parameter expressed in the format `name:type`.
   - `| description = "Performs a math operation"` provides a human-readable description of the method.

3. **Scope Closure:**  
   The line `_` indicates the end of the method block. This explicit marker tells the parser that all properties for this method have been defined.

## Nested Scopes

This explicit scope mechanism can also be applied to nested elements. For example, a class containing multiple methods might be structured as follows:

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
- The class block starts with `- class MyClass:` and ends with the final `_`.
- Inside the class block, each method is a separate nested scope that begins with `- method ...:` and ends with its own `_`.

Below are two new sections for our DSL schema that define how users can specify parameter and return type qualifiers as well as the overall data types for our DSL. These sections follow our goal of keeping the DSL simple, intuitive, and C++-inspired.

---

### Parameter/Return Type Qualifiers

In our DSL, both parameter and return types can be enhanced with qualifiers and modifiers to express additional semantics. The allowed qualifiers and modifiers include:

- **Qualifiers:**  
  Users can specify qualifiers such as `const` or `volatile` before the base type. For example:  
  ```
  | return = const int
  | parameters = param1:const int, param2:volatile float
  ```

- **Pointers:**  
  A pointer is indicated by appending an asterisk (`*`) directly to the type. This means the function or parameter returns a pointer rather than a value. Examples include:  
  ```
  | return = int*
  | parameters = ptr:float*
  ```
  When combined with qualifiers:  
  ```
  | return = const int*
  ```

- **Lvalue References:**  
  Lvalue references are expressed using the ampersand (`&`). This signals that the function returns or accepts a reference to an object. For instance:  
  ```
  | return = int&
  | parameters = ref:double&
  ```
  This can also be combined with qualifiers:  
  ```
  | return = const int&
  ```

- **Rvalue References:**  
  Rvalue references are denoted using a double ampersand (`&&`). These allow functions to return or accept objects that can be moved from. Examples include:  
  ```
  | return = int&&
  | parameters = temp:int&&
  ```
  Rvalue references are especially useful for move semantics, such as in constructors or factory methods, where resources are transferred rather than copied.

- **Arrays:**  
  Arrays are specified by appending square brackets after the type. They can be either of unspecified size (`[]`) or fixed size (e.g., `[10]`). Examples include:  
  ```
  | return = int[]
  | parameters = arr:float[5]
  ```

- **Auto:**  
  For scenarios where the return type should be deduced by the compiler, the DSL supports the use of the keyword `auto`:  
  ```
  | return = auto
  ```

These rules allow users to compose types in a manner that is both familiar and expressive, while remaining simple and readable.

---

### Data Types

Our DSL supports a range of built-in data types, as well as user-defined types. In our current implementation, the following data types are supported:

- **Built-In Types:**  
  The DSL provides an enumeration of common data types for quick specification. These include:  
  - `void`
  - `int`
  - `uint`
  - `long`
  - `ulong`
  - `longlong` (or `LONGLONG`)
  - `ulonglong` (or `ULONGLONG`)
  - `float`
  - `double`
  - `bool`
  - `string`
  - `char`
  - `auto`

- **Custom Types:**  
  If a built-in type does not suffice, users can define a custom type. This is indicated by the keyword `CUSTOM` along with the name of the user-defined type. For example, if you have a user-defined type `MyType`, you might specify:  
  ```
  | return = MyType
  ```
  Behind the scenes, this will be captured as a custom type in our model.

- **Combining with Qualifiers and Modifiers:**  
  The final data type for a parameter or return value can include any of the above qualifiers and modifiers. For example:  
  ```
  | return = const int*       // A constant pointer to an int
  | return = int[10]          // An array of 10 ints
  | parameters = data:const MyType&  // A reference to a constant user-defined type
  | return = auto             // Let the compiler deduce the return type
  | return = int&&            // An rvalue reference to an int
  | parameters = temp:int&&   // An rvalue reference parameter
  ```

## Next Steps

For this initial stage, our key focus is on the following keywords and properties:

- **Keywords:** project, library, class, method, member, function, folder  
- **Properties:** name, version, dependency, parameters, data types, template, description

Later, we plan to expand this list with additional keywords (such as `namespace`, `concept`, or build-related settings) as our requirements evolve. We also plan to support splitting the DSL across multiple files, where declarations might reside in one file and more detailed definitions in another—the scaffolder will merge these based on unique identifiers.

As the DSL matures, we will update the parser to fully support these explicit scope markers, ensuring that even if users deviate slightly in indentation or spacing, the core structure remains unambiguous.
