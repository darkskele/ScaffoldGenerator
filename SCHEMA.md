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

## Next Steps

For this initial stage, our key focus is on the following keywords and properties:

- **Keywords:** project, library, class, method, member, function, folder  
- **Properties:** name, version, dependency, parameters, data types, template, description

Later, we plan to expand this list with additional keywords (such as `namespace`, `concept`, or build-related settings) as our requirements evolve. We also plan to support splitting the DSL across multiple files, where declarations might reside in one file and more detailed definitions in another—the scaffolder will merge these based on unique identifiers.

As the DSL matures, we will update the parser to fully support these explicit scope markers, ensuring that even if users deviate slightly in indentation or spacing, the core structure remains unambiguous.
