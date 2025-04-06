#include "SpecialMemberGenerator.h"
#include "PropertiesGenerator.h"

#include <stdexcept>
#include <sstream>

/**
 * @brief Internal utility functions for generating Doxygen documentation.
 *
 * This anonymous namespace contains helper functions used to generate minimal
 * Doxygen comment blocks for custom constructors and assignment operators based
 * on a class's DSL model.
 */
namespace
{

    /**
     * @brief Generates Doxygen documentation for a non-default constructor.
     *
     * This function writes a minimal Doxygen comment block for a constructor into
     * the provided output stream. It handles custom, copy, and move constructors.
     * For a custom constructor, parameter documentation is generated for each parameter.
     * For copy and move constructors, a brief description of the 'other' parameter is added.
     * No documentation is generated if the constructor is of type DEFAULT.
     *
     * @param ctor The constructor model containing the constructor type and its parameters.
     * @param className The name of the class for which the constructor is being documented.
     * @param oss The output stream where the generated Doxygen comment will be written.
     */
    static void generateCtorDoxygen(const ClassModels::Constructor &ctor,
                                    const std::string &className,
                                    std::ostringstream &oss)
    {
        // Only write Doxygen for non-default constructors.
        if (ctor.type != ClassModels::ConstructorType::DEFAULT)
        {
            oss << "    /**\n     * @brief Custom ";
            // Write minimal Doxygen based on constructor type.
            if (ctor.type == ClassModels::ConstructorType::CUSTOM)
            {
                oss << "Constructor.\n";
                // Generate parameter Doxygen docstrings.
                for (const auto &param : ctor.parameters)
                {
                    oss << "     * @param " << param.name << " \n";
                }
            }
            else if (ctor.type == ClassModels::ConstructorType::COPY)
            {
                // Generate minimal copy constructor documentation.
                oss << "Copy Constructor.\n";
                oss << "     * @param other The " << className << " object to copy from.\n";
            }
            else if (ctor.type == ClassModels::ConstructorType::MOVE)
            {
                // Generate minimal move constructor documentation.
                oss << "Move Constructor.\n";
                oss << "     * @param other The " << className << " object to move from.\n";
            }
            oss << "     */\n";
        }
    }

    /**
     * @brief Generates Doxygen documentation for copy or move assignment operators.
     *
     * This function writes a minimal Doxygen comment block for an assignment operator
     * into the provided output stream. Depending on the value of the 'copy' parameter,
     * it generates documentation for either a copy assignment or a move assignment operator.
     *
     * @param className The name of the class for which the assignment operator is being documented.
     * @param oss The output stream where the generated Doxygen comment will be written.
     * @param copy If true, generates documentation for a copy assignment operator; otherwise, for a move assignment operator.
     */
    static void generateCopyAndMoveAssingmentDoxygen(const std::string &className,
                                                     std::ostringstream &oss,
                                                     const bool copy)
    {
        // Decide the assignment type in the Doxygen docstring.
        std::string assignmentType = " move ";
        if (copy)
        {
            assignmentType = " copy ";
        }
        // Write minimal Doxygen documentation based on the assignment type.
        oss << "    /**\n     * @brief Custom" << assignmentType << "assignment operator.\n";
        oss << "     * @param other The " << className << " object to" << assignmentType << "from.\n";
        oss << "     * @return Reference to this " << className << ".\n";
        oss << "     */\n";
    }

} // end anonymous namespace

namespace SpecialMemberGenerator
{
    std::string generateConstructorDeclaration(const std::string &className, const ClassModels::Constructor &ctor)
    {
        // Create an output string stream to build the constructor declaration.
        std::ostringstream oss;

        // Generate constructor docstring
        generateCtorDoxygen(ctor, className, oss);

        // Begin the constructor declaration with the class name and an opening parenthesis.
        oss << "    " << className << "(";

        // Check the constructor type to determine the appropriate declaration.
        if (ctor.type == ClassModels::ConstructorType::CUSTOM)
        {
            // For custom constructors, generate the parameter list and close the declaration.
            oss << PropertiesGenerator::generateParameterList(ctor.parameters) << ");\n";
        }
        else if (ctor.type == ClassModels::ConstructorType::COPY)
        {
            // For copy constructors, use a const reference to another instance.
            oss << "const " << className << "& other);\n";
        }
        else if (ctor.type == ClassModels::ConstructorType::MOVE)
        {
            // For move constructors, use an rvalue reference and mark the constructor as noexcept.
            oss << className << "&& other) noexcept;\n";
        }
        else if (ctor.type == ClassModels::ConstructorType::DEFAULT)
        {
            // For default constructors, close the declaration with a default specifier.
            oss << ") = default;\n";
        }
        else
        {
            // If the constructor type is unrecognized, throw a runtime error.
            throw std::runtime_error("Unrecognised constructor type!");
        }

        // Return the complete constructor declaration as a string.
        oss << "\n";
        return oss.str();
    }

    std::string generateConstructorDefinition(const std::string &className, const ClassModels::Constructor &ctor,
                                              const std::vector<PropertiesModels::Parameter> publicMembers,
                                              const std::vector<PropertiesModels::Parameter> privateMembers,
                                              const std::vector<PropertiesModels::Parameter> protectedMembers)
    {
        // For DEFAULT constructor, no out-of-line definition is needed.
        if (ctor.type == ClassModels::ConstructorType::DEFAULT)
        {
            return ""; // or a comment string if you prefer to generate something
        }

        std::ostringstream oss;
        oss << className << "::" << className << "(";

        if (ctor.type == ClassModels::ConstructorType::CUSTOM)
        {
            oss << PropertiesGenerator::generateParameterList(ctor.parameters) << ")";
        }
        else if (ctor.type == ClassModels::ConstructorType::COPY)
        {
            oss << "const " << className << "& other)";
        }
        else if (ctor.type == ClassModels::ConstructorType::MOVE)
        {
            oss << className << "&& other) noexcept";
        }
        else
        {
            throw std::runtime_error("Unrecognised constructor type!");
        }

        // Begin the initializer list.
        std::ostringstream initList;
        bool firstInit = true;
        std::vector<std::vector<PropertiesModels::Parameter>> memberScopes = {publicMembers, privateMembers, protectedMembers};
        for (const auto &scope : memberScopes)
        {
            for (const auto &p : scope)
            {
                if (!firstInit)
                    initList << ", ";
                else
                    firstInit = false;
                initList << p.name << "()";
            }
        }

        if (!firstInit)
            oss << " : " << initList.str();

        // Append an empty body for the constructor definition.
        oss << "\n{\n    // TODO: Implement " + className + " construtor logic.\n";
        oss << "    throw std::runtime_error(\"Not implemented\");\n}\n";

        return oss.str();
    }

    std::string generateDestructorDeclaration(const std::string &className)
    {
        std::ostringstream oss;
        // Build the destructor declaration.
        // This generates a declaration like:
        // "    ~MyClass() = default;"
        oss << "    ~" << className << "() = default;";
        return oss.str();
    }

    std::string generateDestructorDefinition(const std::string &className)
    {
        // For default destructors, no out-of-line definition is needed.
        // Return an empty string as a placeholder.
        return "";
    }

    std::string generateMoveAssignmentDeclaration(const std::string &className)
    {
        std::ostringstream oss;
        // Generate the docstring
        generateCopyAndMoveAssingmentDoxygen(className, oss, false);
        // Build move assignment operator declaration.
        // This creates a declaration of the form:
        // MyClass& operator=(MyClass&& other) noexcept;
        oss << "    " << className << "& operator=(" << className << "&& other) noexcept;\n";
        return oss.str();
    }

    std::string generateMoveAssignmentDefinition(const std::string &className)
    {
        std::ostringstream oss;
        // Start constructing the move assignment operator definition.
        // The generated signature will be:
        // MyClass& MyClass::operator=(MyClass&& other) noexcept {
        oss << className << "& " << className << "::operator=(" << className << "&& other) noexcept {\n";
        // Insert a placeholder body that indicates the method is not yet implemented.
        oss << "    // TODO: Implement " + className + " move assignment logic.\n";
        oss << "    throw std::runtime_error(\"Not implemented\");\n";
        oss << "}\n";
        return oss.str();
    }

    std::string generateCopyAssignmentDeclaration(const std::string &className)
    {
        std::ostringstream oss;
        // Generate the docstring
        generateCopyAndMoveAssingmentDoxygen(className, oss, false);
        // Build copy assignment operator declaration.
        // This creates a declaration of the form:
        // MyClass& operator=(const MyClass& other);
        oss << "    " << className << "& operator=(const " << className << "& other);\n";
        return oss.str();
    }

    std::string generateCopyAssignmentDefinition(const std::string &className)
    {
        std::ostringstream oss;
        // Start constructing the copy assignment operator definition.
        // The generated signature will be:
        // MyClass& MyClass::operator=(const MyClass& other) {
        oss << className << "& " << className << "::operator=(const " << className << "& other) {\n";
        // Insert a placeholder body that indicates the method is not yet implemented.
        oss << "    // TODO: Implement " + className + " copy assignment logic.\n";
        oss << "    throw std::runtime_error(\"Not implemented\");\n";
        oss << "}\n";
        return oss.str();
    }

} // namespace SpecialMemberGenerator
