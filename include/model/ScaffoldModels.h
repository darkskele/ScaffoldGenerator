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
#include <optional>

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
     * @brief Represents a callable entity, such as a function or a method.
     *
     * This base model encapsulates common properties shared by methods and functions,
     * including the return type, name, parameters, declaration specifiers, and a description.
     */
    struct CallableModel
    {
        /// The return type of the callable.
        ScaffoldProperties::DataType returnType;
        /// The name of the callable.
        std::string name;
        /// A vector of parameters associated with the callable.
        std::vector<ScaffoldProperties::Parameter> parameters;
        /// Declaration specifiers that modify the callable's behavior (e.g., inline, static).
        ScaffoldProperties::DeclartionSpecifier declSpec;
        /// A description of the callable.
        std::string description;

        /**
         * @brief Constructor for CallableModel.
         *
         * Initializes a new instance of CallableModel with the specified return type, name, parameters,
         * declaration specifiers, and an optional description.
         *
         * @param retType The return type of the callable.
         * @param n The name of the callable.
         * @param params A vector of parameters associated with the callable.
         * @param dC Declaration specifiers that modify the callable's behavior (e.g., inline, static).
         * @param desc Optional description of the callable. Defaults to a single space.
         */
        CallableModel(const ScaffoldProperties::DataType retType, std::string n,
                      std::vector<ScaffoldProperties::Parameter> params,
                      const ScaffoldProperties::DeclartionSpecifier &dC,
                      std::string desc = " ")
            : returnType(retType), name(std::move(n)), parameters(std::move(params)),
              declSpec(std::move(dC)), description(std::move(desc))
        {
        }
    };

    /**
     * @brief Represents a method in a class.
     *
     * Inherits common callable properties from CallableModel.
     */
    struct MethodModel : public CallableModel
    {
        /**
         * @brief Inherits the constructor from CallableModel.
         */
        using CallableModel::CallableModel;
    };

    /**
     * @brief Represents a free function.
     *
     * Inherits common callable properties from CallableModel.
     */
    struct FunctionModel : public CallableModel
    {
        /**
         * @brief Inherits the constructor from CallableModel.
         */
        using CallableModel::CallableModel;
    };

    /**
     * @brief Represents a special member function common to constructors and destructors.
     *
     * This structure encapsulates properties that are shared between constructors and destructors,
     * such as a description. It promotes immutability and consistency across special member functions.
     */
    struct SpecialMemberFunction
    {
        std::string description; /**< Description of the special member function */

        /**
         * @brief Constructor for SpecialMemberFunction.
         * @param desc A description of the special member function.
         */
        SpecialMemberFunction(const std::string &desc) : description(desc)
        {
        }
    };

    /**
     * @brief Enumerates the types of constructors available.
     */
    enum class ConstructorType
    {
        DEFAULT, /**< Default constructor */
        COPY,    /**< Copy constructor */
        MOVE,    /**< Move constructor */
        CUSTOM   /**< Custom constructor with user-specified parameters */
    };

    /**
     * @brief Model representing a constructor for a class.
     *
     * This structure inherits common properties from SpecialMemberFunction and includes
     * constructor-specific details such as its type and parameters. Constructors defined
     * via the DSL may be auto-generated using an inline property (e.g., "default, copy, move")
     * or explicitly defined in nested blocks for custom behavior.
     */
    struct Constructor : public SpecialMemberFunction
    {
        const ConstructorType type;                                  /**< The type of constructor */
        const std::vector<ScaffoldProperties::Parameter> parameters; /**< Constructor parameters (empty for default, copy, or move) */

        /**
         * @brief Constructor for the Constructor model.
         *
         * @param type The type of constructor (default, copy, move, or custom).
         * @param params The parameters for the constructor. For non-custom constructors, this should be empty.
         * @param desc A description of the constructor.
         */
        Constructor(ConstructorType type,
                    const std::vector<ScaffoldProperties::Parameter> &params,
                    const std::string &desc)
            : SpecialMemberFunction(desc), type(type), parameters(params)
        {
        }
    };

    /**
     * @brief Model representing a destructor.
     *
     * This structure is a specialized version of SpecialMemberFunction that represents
     * a class destructor. It cannot have parameters or overloads.
     */
    struct Destructor : public SpecialMemberFunction
    {
        /**
         * @brief Constructs a Destructor with a description.
         * @param desc The description of the destructor.
         */
        explicit Destructor(const std::string &desc) : SpecialMemberFunction(desc) {}

        // Make move/copy constructors defaulted so it can be used with std::optional
        Destructor(const Destructor &) = default;
        Destructor(Destructor &&) = default;
        Destructor &operator=(const Destructor &) = default;
        Destructor &operator=(Destructor &&) = default;
    };

    /**
     * @brief Represents a C++ class as defined in the scaffolder DSL.
     *
     * This structure models a C++ class declaration and definition, capturing
     * data members, methods, constructors, destructors, and metadata like description
     * and assignment operators. It is the central structure used to translate DSL
     * class definitions into actual header and source files.
     */
    struct ClassModel
    {
        const std::string name; /**< Name of the class */

        const std::string description; /**< Doxygen-style description of the class */

        const std::vector<ScaffoldModels::Constructor> constructors; /**< All defined constructors, including default/copy/move/custom */
        const std::optional<ScaffoldModels::Destructor> destructor;  /**< Optional destructor if defined in the DSL */

        const std::vector<ScaffoldModels::MethodModel> publicMethods;    /**< All public methods */
        const std::vector<ScaffoldModels::MethodModel> privateMethods;   /**< All private methods */
        const std::vector<ScaffoldModels::MethodModel> protectedMethods; /**< All protected methods */

        const std::vector<ScaffoldProperties::Parameter> publicMembers;    /**< Public data members (for simplicity, same struct as params) */
        const std::vector<ScaffoldProperties::Parameter> privateMembers;   /**< Private data members */
        const std::vector<ScaffoldProperties::Parameter> protectedMembers; /**< Protected data members */

        const bool hasCopyAssignment; /**< True if copy assignment operator should be generated */
        const bool hasMoveAssignment; /**< True if move assignment operator should be generated */

        /**
         * @brief Constructor for ClassModel.
         *
         * @param n Name of the class.
         * @param desc Human-readable description for Doxygen.
         * @param ctors A list of constructor definitions.
         * @param dtor Optional destructor definition.
         * @param pubMethods List of public method definitions.
         * @param privMethods List of private method definitions.
         * @param protMethods List of protected method definitions.
         * @param pubMembers List of public data members.
         * @param privMembers List of private data members.
         * @param protMembers List of protected data members.
         * @param copyAssign Whether the copy assignment operator should be generated.
         * @param moveAssign Whether the move assignment operator should be generated.
         */
        ClassModel(
            const std::string &n,
            const std::string &desc,
            const std::vector<ScaffoldModels::Constructor> &ctors,
            const std::optional<ScaffoldModels::Destructor> &dtor,
            const std::vector<ScaffoldModels::MethodModel> &pubMethods,
            const std::vector<ScaffoldModels::MethodModel> &privMethods,
            const std::vector<ScaffoldModels::MethodModel> &protMethods,
            const std::vector<ScaffoldProperties::Parameter> &pubMembers,
            const std::vector<ScaffoldProperties::Parameter> &privMembers,
            const std::vector<ScaffoldProperties::Parameter> &protMembers,
            bool copyAssign,
            bool moveAssign)
            : name(n),
              description(desc),
              constructors(ctors),
              destructor(dtor),
              publicMethods(pubMethods),
              privateMethods(privMethods),
              protectedMethods(protMethods),
              publicMembers(pubMembers),
              privateMembers(privMembers),
              protectedMembers(protMembers),
              hasCopyAssignment(copyAssign),
              hasMoveAssignment(moveAssign)
        {
        }
    };

    /**
     * @brief Represents a namespace in the code generation system.
     *
     * This struct models a namespace that can contain nested classes and functions.
     * Additional nested namespaces can be added if necessary.
     */
    struct NamespaceModel
    {
        std::string name;                       ///< The name of the namespace.
        std::string description;                ///< A description of what the namespace represents.
        std::vector<ClassModel> classes;        ///< A list of classes defined within this namespace.
        std::vector<FunctionModel> functions;   ///< A list of functions defined within this namespace.
        std::vector<NamespaceModel> namespaces; ///< A list of nested namespaces.
    };

    /**
     * @brief Represents a folder in the project structure.
     *
     * A folder serves as a container for files and is created in both the include and src directories.
     * The folder's nesting within the DSL determines its relative placement in the generated file system.
     * Each DSL element declared within the folder is assumed to generate its own file.
     */
    struct FolderModel
    {
        /// The name of the folder.
        std::string name;

        /// Subfolders contained within this folder.
        std::vector<FolderModel> subFolders;

        /// Classes defined in this folder; each will generate its own file.
        std::vector<ClassModel> classFiles;

        /// Namespaces defined in this folder; each will generate its own file.
        std::vector<NamespaceModel> namespaceFiles;

        // Each inner vector represents a file that may contain multiple free functions.
        std::vector<FunctionModel> functionFiles;

        /**
         * @brief Constructs a new FolderModel.
         *
         * @param name The name of the folder.
         * @param subFolders A vector of subfolders contained within this folder.
         * @param classFiles A vector of class models that will each generate a file.
         * @param namespaceFiles A vector of namespace models that will each generate a file.
         * @param functionFiles A vector of functions; the vector itself represents a file containing free functions.
         */
        FolderModel(std::string name,
                    const std::vector<FolderModel> &subFolders = {},
                    const std::vector<ClassModel> &classFiles = {},
                    const std::vector<NamespaceModel> &namespaceFiles = {},
                    const std::vector<FunctionModel> &functionFiles = {})
            : name(std::move(name)),
              subFolders(subFolders),
              classFiles(classFiles),
              namespaceFiles(namespaceFiles),
              functionFiles(functionFiles)
        {
        }
    };

    /**
     * @brief Represents a library in the project scaffolding DSL.
     *
     * Inherits from FolderModel, so a LibraryModel creates a folder in both the include and src directories.
     * Additional properties include versioning and dependency management for CMake configuration.
     *
     * Note: Libraries cannot be nested within other libraries. Attempting to do so will result in a DSL error.
     */
    struct LibraryModel : public FolderModel
    {
        /// The version of the library.
        std::string version;
        /// A vector of dependencies (e.g., other libraries or build features such as Boost, cpp20).
        std::vector<std::string> dependencies;

        /**
         * @brief Constructs a new LibraryModel.
         *
         * @param name The name of the library. This is also used as the folder name.
         * @param version The version of the library.
         * @param dependencies A vector of dependencies required by the library.
         * @param subFolders Optional subfolder models nested inside the library.
         * @param classFiles Optional class models that generate individual files.
         * @param namespaceFiles Optional namespace models that generate individual files.
         * @param functionFiles Optional free function models; the vector represents a file containing functions.
         */
        LibraryModel(std::string name,
                     std::string version,
                     std::vector<std::string> dependencies,
                     const std::vector<FolderModel> &subFolders = {},
                     const std::vector<ClassModel> &classFiles = {},
                     const std::vector<NamespaceModel> &namespaceFiles = {},
                     const std::vector<FunctionModel> &functionFiles = {})
            : FolderModel(std::move(name), subFolders, classFiles, namespaceFiles, functionFiles),
              version(std::move(version)),
              dependencies(std::move(dependencies))
        {
        }
    };

    /**
     * @brief Represents a project in the scaffolding DSL.
     *
     * Inherits from FolderModel, so a ProjectModel creates a folder in the project root.
     * It includes versioning and dependency management for CMake configuration,
     * and additionally holds a collection of libraries.
     */
    struct ProjectModel : public FolderModel
    {
        /// The version of the project.
        std::string version;
        /// A vector of dependencies required by the project.
        std::vector<std::string> dependencies;
        /// A vector of libraries that are part of the project.
        std::vector<LibraryModel> libraries;

        /**
         * @brief Constructs a new ProjectModel.
         *
         * @param name The name of the project. This is also used as the folder name.
         * @param version The version of the project.
         * @param dependencies A vector of dependencies required by the project.
         * @param libraries A vector of LibraryModel instances that are part of the project.
         * @param subFolders Optional subfolder models nested inside the project.
         * @param classFiles Optional class models that generate individual files.
         * @param namespaceFiles Optional namespace models that generate individual files.
         * @param functionFiles Optional free function models; the vector represents a file containing functions.
         */
        ProjectModel(std::string name,
                     std::string version,
                     std::vector<std::string> dependencies,
                     std::vector<LibraryModel> libraries,
                     const std::vector<FolderModel> &subFolders = {},
                     const std::vector<ClassModel> &classFiles = {},
                     const std::vector<NamespaceModel> &namespaceFiles = {},
                     const std::vector<FunctionModel> &functionFiles = {})
            : FolderModel(std::move(name), subFolders, classFiles, namespaceFiles, functionFiles),
              version(std::move(version)),
              dependencies(std::move(dependencies)),
              libraries(std::move(libraries))
        {
        }
    };

} // namespace ScaffoldModels
