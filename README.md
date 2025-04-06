# Project Scaffold Generator

The **Project Scaffold Generator** is a tool that automates the creation of C++ project structures, configuration files, and skeleton code. It reads from a custom DSL file (`.scaff`) that describes high-level design elements (folders, classes, namespaces, etc.) and generates:

- **CMakeLists.txt** (build system configuration)
- **VS Code** configuration files (`launch.json` and `tasks.json`)
- **main.cpp** (entry point)
- **Headers and source files** for classes, namespaces, and functions
- A structured directory tree, following modern C++23 best practices

## Table of Contents

1. [Features in This Release](#features-in-this-release)  
2. [Future Roadmap](#future-roadmap)  
3. [Overview of the DSL Schema](#overview-of-the-dsl-schema)  
4. [Building the Scaffolder](#building-the-scaffolder)  
5. [Using the Scaffolder via CLI](#using-the-scaffolder-via-cli)  
6. [Error Handling and Logs](#error-handling-and-logs)  
7. [Contributing](#contributing)  
8. [License](#license)

---

## Features in This Release

1. **DSL Parsing and Code Generation**  
   - Reads a `.scaff` file (or directory containing one) and converts it into a structured C++ project.
   - Supports classes, namespaces, free functions, and folder hierarchies.
   - Automatically splits declarations (`.h`) and definitions (`.cpp`) into organized directories (`include/` and `src/`).

2. **CMakeLists Generation**  
   - Generates a top-level `CMakeLists.txt` that defines your project, libraries, and dependencies.
   - Supports project-level and library-level metadata, including version and dependency information.

3. **VS Code Integration**  
   - Creates `launch.json` and `tasks.json` under a `.vscode` folder for debugging and build tasks.
   - Provides a default debug configuration and a build task using CMake.

4. **main.cpp Generation**  
   - Automatically generates a minimal `main.cpp` with a “Hello, world!” message.

5. **Directory Tree Construction**  
   - Recursively builds a directory tree for the project, mapping DSL elements (folders, classes, namespaces) to their respective file paths.

---

## Future Roadmap

Several enhancements are planned for upcoming releases. Below are some of the future features you can expect (based on the current GitHub issues and roadmap):

- **Modules Support (#25)**  
  Integrate C++20/23 Modules into the code generation for faster compilation and cleaner interface boundaries.
- **Docker File Generation (#24)**  
  Automatically generate Dockerfiles and `devcontainer.json` for container-based development environments.
- **Test Scaffold Generation (#23)**  
  Provide a testing structure (e.g., Google Test or Catch2) for each generated class and function.
- **VSCode Extension (#19)**  
  A dedicated VSCode extension that manages scaffolding tasks and updates the DSL file in real time.
- **Inheritance & Polymorphism (#2)**  
  Expand the DSL to handle inheritance hierarchies and polymorphic behavior, generating relevant C++ boilerplate.
- **Teamplating (#5)**
  Expand DSL and scaffolder to handle templated classes and functions as well as complex templated types.
---

## Overview of the DSL Schema

The `.scaff` file format is defined by a custom **Scaffolder DSL** schema. It allows you to declare:

- **Project**: The root element containing version, dependencies, and top-level folders or libraries.
- **Library**: Represents a modular library with its own version, dependencies, and folder hierarchy.
- **Folder**: Organizational unit for classes, namespaces, and free functions.
- **Namespace**: A C++ namespace that can contain classes and free functions.
- **Class**: A C++ class with constructors, destructors, methods, and member variables.
- **Function**: A free function at the project, library, or folder level.
- **Method**: A member function within a class.

For a more detailed description of each DSL element, please see the [Scaffolder DSL Schema](./docs/ScaffolderDSL.md).  
*(Adjust the link above based on your repo structure.)*

---

## Building the Scaffolder

1. **Clone the Repository**  
   ```bash
   git clone https://github.com/YourUser/ProjectScaffolder.git
   cd ProjectScaffolder
   ```

2. **Configure and Build**  
   The scaffolder uses CMake for its build process:
   ```bash
   mkdir build && cd build
   cmake ..
   make -j4
   ```
   *(Adjust the number `-j4` to match the number of cores on your machine.)*

3. **Run Tests (Optional)**  
   If you have unit tests set up, you can run them:
   ```bash
   ctest
   ```

4. **Install (Optional)**  
   You can install the scaffolder to a system directory if desired:
   ```bash
   sudo make install
   ```
   *(Ensure you have permission to install to the target directory.)*

---

## Using the Scaffolder via CLI

Once built, the **scaffolder** executable is placed in the `build/` directory (unless installed). You can run it directly:

```bash
./scaffolder <input_path> [--output-folder <output_path>]
```

- **`<input_path>`**  
  - Can be a `.scaff` file or a directory containing one.  
  - If a directory is provided, the scaffolder searches for the first `.scaff` file in alphabetical order.

- **`--output-folder <output_path>`** (optional)  
  - Specifies the directory where the generated project files will be placed.  
  - Defaults to `generatedOutputs/` if not specified.

### Example

```bash
./scaffolder MyProject.scaff --output-folder MyGeneratedProject
```

This command parses `MyProject.scaff` and outputs all generated files (headers, sources, CMake, VS Code configs) into the `MyGeneratedProject` directory.

---

## Error Handling and Logs

The scaffolder will terminate with an error message if:

- The input path does not exist or is neither a file nor a directory.
- No `.scaff` file is found in the specified directory.
- A file I/O operation fails (e.g., unable to read or write a file).
- The DSL file is malformed (e.g., missing a `- project <name>:` block).

In such cases, the scaffolder prints an error message to `stderr` and returns a non-zero exit code.

---

## Contributing

We welcome contributions! If you have suggestions or bug reports, please open an issue or submit a pull request:

1. **Fork** the repository.
2. **Create** a new branch for your feature or bug fix.
3. **Commit** and **push** your changes.
4. **Open** a pull request describing the changes.

For major feature requests, please check our [open issues](https://github.com/YourUser/ProjectScaffolder/issues) and create a new issue if one does not already exist.

---

## License

```
Hobbyist Use License (Version 1.0)

Copyright (c) 2025 darkskele

Permission for Non-Commercial Use
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software") for personal, educational, and hobbyist use only. This includes, but is not limited to, use for personal projects, academic experiments, or other non-commercial activities.

Restrictions on Commercial Use
Any use of the Software for commercial purposes, including but not limited to integration into products or services that are sold, distributed for profit, or used as part of a for-profit operation, is not permitted under this license unless a separate commercial license is obtained from the copyright holder.
To request a commercial license, please contact [your email address or preferred contact method].

Other Rights
The Software is provided "as is", without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose, and noninfringement. In no event shall the authors or copyright holders be liable for any claim, damages, or other liability, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the Software or the use or other dealings in the Software.

Redistribution
Redistributions of the Software must include this license and retain the above copyright notice and this permission notice in all copies or substantial portions of the Software.

```

---

### Thank You!

We appreciate your interest in the **Project Scaffold Generator**. By automating repetitive tasks and leveraging modern C++23 features, our goal is to help developers focus on writing meaningful code rather than boilerplate. If you have questions or feedback, please open an issue or start a discussion in the repository. Happy scaffolding!