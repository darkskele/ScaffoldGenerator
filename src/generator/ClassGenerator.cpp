#include "ClassGenerator.h"
#include "SpecialMemberGenerator.h"
#include "CallableGenerator.h"
#include "GeneratorUtilities.h"

#include <sstream>
#include <format>

/**
 * @brief Anonymous namespace for internal helper functions.
 *
 * This anonymous namespace contains helper functions that are intended for internal use within the translation unit.
 * The functions provided here assist with code generation tasks, such as generating method definitions for classes.
 */
namespace
{
    /**
     * @brief Helper function to generate method definitions.
     *
     * Iterates over the provided methods and appends each method's definition to the output stream.
     *
     * @param methods The vector of MethodModel objects.
     * @param oss The output stream to append the definitions.
     */
    void classMethodDefinitionGenerator(const std::vector<CallableModels::MethodModel> &methods,
                                        const std::string &className, std::ostringstream &oss)
    {
        for (const auto &meth : methods)
        {
            // Generate the definition for each method and append a newline.
            oss << CallableGenerator::generateMethodDefinition(className, meth) << "\n";
        }
    }

    /**
     * @brief Formats and writes class member declarations.
     *
     * This function iterates over a list of member parameters and writes each declaration
     * into the provided output stream in the format:
     * "    <data type> <member name>; ///< " followed by a newline.
     * An extra newline is appended after processing all members.
     *
     * @param members The vector of member parameters to format.
     * @param oss The output string stream where the member declarations are written.
     */
    void classMemberDeclaration(const std::vector<PropertiesModels::Parameter> &members,
                                std::ostringstream &oss)
    {
        // Format list of members
        for (const auto &mem : members)
        {
            oss << std::format("    {} {}; ///< \n", GeneratorUtilities::dataTypeToString(mem.type), mem.name);
        }
        oss << "\n";
    }
}

namespace ClassGenerator
{
    std::string generateClassDeclaration(const ClassModels::ClassModel &cl)
    {
        std::ostringstream oss;
        // Generate Doxygen-style class comment.
        oss << "/**\n * @class " << cl.name << "\n * @brief " << cl.description << "\n */\n";
        // Start class declaration.
        oss << "class " << cl.name << " {\npublic:\n";

        // Generate constructor declarations.
        for (const auto &ctor : cl.constructors)
        {
            oss << SpecialMemberGenerator::generateConstructorDeclaration(cl.name, ctor);
        }

        // Generate destructor declaration if available.
        if (cl.destructor)
        {
            oss << SpecialMemberGenerator::generateDestructorDeclaration(cl.name) << "\n";
        }

        // Generate copy assignment declaration if specified.
        if (cl.hasCopyAssignment)
        {
            oss << SpecialMemberGenerator::generateCopyAssignmentDeclaration(cl.name) << "\n";
        }

        // Generate move assignment declaration if specified.
        if (cl.hasMoveAssignment)
        {
            oss << SpecialMemberGenerator::generateMoveAssignmentDeclaration(cl.name) << "\n";
        }

        // Generate declarations for public methods.
        for (const auto &meth : cl.publicMethods)
        {
            oss << CallableGenerator::generateMethodDeclaration(meth);
        }

        // Generate declarations for public members.
        classMemberDeclaration(cl.publicMembers, oss);

        // Generate private section if necessary.
        if (!cl.privateMembers.empty() || !cl.privateMethods.empty())
        {
            oss << "private:\n";
            for (const auto &meth : cl.privateMethods)
            {
                oss << CallableGenerator::generateMethodDeclaration(meth);
            }

            classMemberDeclaration(cl.privateMembers, oss);
        }

        // Generate protected section if necessary.
        if (!cl.protectedMembers.empty() || !cl.protectedMethods.empty())
        {
            oss << "protected:\n";
            for (const auto &meth : cl.protectedMethods)
            {
                oss << CallableGenerator::generateMethodDeclaration(meth);
            }

            classMemberDeclaration(cl.protectedMembers, oss);
        }

        // End class declaration.
        oss << "};\n";
        return oss.str();
    }

    std::string generateClassDefinition(const ClassModels::ClassModel &cl)
    {
        std::ostringstream oss;

        // Generate definitions for constructors.
        for (const auto &ctor : cl.constructors)
        {
            // Generate out-of-line constructor definition.
            std::string def = SpecialMemberGenerator::generateConstructorDefinition(cl.name, ctor,
                                                                                    cl.publicMembers, cl.privateMembers, cl.protectedMembers);
            if (!def.empty())
            {
                oss << def << "\n";
            }
        }

        // Generate definition for copy assignment operator if specified.
        if (cl.hasCopyAssignment)
        {
            std::string def = SpecialMemberGenerator::generateCopyAssignmentDefinition(cl.name);
            if (!def.empty())
            {
                oss << def << "\n";
            }
        }

        // Generate definition for move assignment operator if specified.
        if (cl.hasMoveAssignment)
        {
            std::string def = SpecialMemberGenerator::generateMoveAssignmentDefinition(cl.name);
            if (!def.empty())
            {
                oss << def << "\n";
            }
        }

        // Generate destructor definition if available.
        if (cl.destructor)
        {
            std::string def = SpecialMemberGenerator::generateDestructorDefinition(cl.name);
            if (!def.empty())
            {
                oss << def << "\n";
            }
        }

        // Generate definitions for public methods.
        classMethodDefinitionGenerator(cl.publicMethods, cl.name, oss);

        // Generate definitions for private methods.
        classMethodDefinitionGenerator(cl.privateMethods, cl.name, oss);

        // Generate definitions for protected methods.
        classMethodDefinitionGenerator(cl.protectedMethods, cl.name, oss);

        return oss.str();
    }

} // namespace ClassGenerator
