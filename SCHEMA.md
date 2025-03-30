# Scaffolder DSL Schema - Initial Draft

This document defines the initial set of keywords and properties for our custom DSL (with a suggested file extension of `.scaff`). Our goal is to create a human-readable, modular format for specifying project structures, which can then be used to generate C++ projects automatically.

## Overview

Our DSL will allow you to define a project structure using a simple, indented syntax. The idea is to express the high-level design (such as project dependencies, libraries, classes, and methods) in one or more files that the scaffolder tool can parse and use to generate all necessary project files (CMake, VS Code settings, etc.).

## Keywords

The following top-level keywords will define the various elements in our project:

- **project**  
  Defines the overall project and its high-level properties.

- **library**  
  Represents a modular library within the project. Libraries can be marked as static or dynamic, and they encapsulate related functionalities.

- **class**  
  Specifies a C++ class and its related details such as inheritance, methods, and members.

- **method**  
  Describes a member function within a class. It includes return type, parameters, and a placeholder implementation.

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
  Specifies the data types for parameters, members, and return types. This will support advanced features such as templates and concepts.

- **template**  
  (Optional) Indicates that the element (class or method) is templated, possibly with specific constraints or concepts.

- **description**  
  A human-readable text describing the element's purpose or functionality, useful for documentation and clarity.

## Example DSL Snippet

Here’s an example to illustrate the intended format:

```
proj:
| name = MyProject
| version = 1.0
| dependency = cpp20, openmp
- library utilLibrary:
  | type = dynamic
  | dependency = mathLib
  - class mathClass:
    | template = arithmetic, floating point
    | description = "Provides math operations with template support"
    - method doSomething:
      | return = void
      | parameters = param1:int, param2:float
      | description = "Performs a math operation"
    - member result:
      | data type = float
      | description = "Stores the computation result"
```

## Next Steps

For this initial stage, our key focus is on the following keywords and properties:

- **Keywords:** project, library, class, method, member, function, folder  
- **Properties:** name, version, dependency, parameters, data types, template, description

Later, we can expand this list with additional keywords (such as `namespace`, `concept`, or build-related settings) as our requirements evolve. We also plan to support splitting the DSL across multiple files, where declarations might reside in one file and more detailed definitions in another—the scaffolder will merge these based on unique identifiers.