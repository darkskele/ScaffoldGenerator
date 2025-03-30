# Project Scaffold Generator

## Overview

This project aims to create an automated generator for C++ project scaffolds. The generator is split into two major components:

1. **UI Designer (Qt-based):**
   - A graphical interface that allows users to design their projects in a UML-like format.
   - Enables users to define class structures, inheritance hierarchies, and project dependencies.
   - Outputs a custom file format capturing high-level design details, including build, launch, and Docker configuration data.

2. **Project Scaffolder:**
   - A backend tool that reads the custom design file from the UI and generates a complete C++ project.
   - Creates essential configuration files such as:
     - **VS Code:** `launch.json` and `tasks.json`
     - **Dev Container:** `devcontainer.json` and `Dockerfile`
     - **CMake:** `CMakeLists.txt`
   - Generates project directories and code skeletons (headers and source files with placeholder implementations).
   - Emphasizes modern C++ best practices (C++20+), with a focus on templates and robust type management.

## Goals

- **Automate Repetitive Tasks:**  
  Reduce boilerplate by automatically generating configuration and code files.
- **High-Level Project Design:**  
  Allow users to focus on design while the scaffolder handles implementation details.
- **Learning & Best Practices:**  
  Leverage modern C++ standards and patterns to ensure the generated projects are maintainable and scalable.

## Future Considerations

- **Modularity:**  
  The project will be modular to support extensions like alternative build systems or IDE integrations.
- **Enhanced UI Features:**  
  Additional capabilities for error checking, validation, and richer design options.
- **GitHub Integration:**  
  Automate repository initialization and initial push to GitHub.
- **Customizable Generation:**  
  Allow users to define custom generation patterns for different project architectures.


