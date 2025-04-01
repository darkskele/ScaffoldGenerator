#include "MethodGenerator.h"
#include "PropertiesGenerator.h"
#include "GeneratorUtilities.h"
#include <format>
#include <stdexcept>

namespace MethodGenerator
{
    // This function converts a MethodModel into a declaration string suitable for a header file.
    // It concatenates the declaration specifiers, return type, method name, and parameters into a
    // formatted string. Additionally, a Doxygen comment block is prepended to the declaration to
    // provide inline documentation for the generated method.
    std::string generateMethodDeclaration(const ScaffoldModels::MethodModel &method)
    {
        // Convert the method's return type to its string representation using a utility function.
        std::string returnTypeStr = GeneratorUtilities::dataTypeToString(method.returnType);

        // Generate the parameter list string from the method's parameters.
        std::string paramList = PropertiesGenerator::generateParameterList(method.parameters);

        // Generate any declaration specifiers (e.g., inline, static) required for the method.
        std::string declSpec = PropertiesGenerator::generateDeclarationSpecifier(method.declSpec);

        // Create a Doxygen comment block for the method description.
        // This block will be placed directly above the method declaration.
        std::string result = "    /**\n";
        result += "     * @brief " + method.description + "\n";
        result += "     */\n";

        // Format the final declaration string with proper spacing and syntax.
        result += std::format("    {}{} {}({});\n", declSpec, returnTypeStr, method.name, paramList);

        return result;
    }

    // This function builds the full method definition string for a given class and MethodModel.
    // It combines the declaration specifiers, return type, fully qualified method name, and parameters,
    // along with a default method body that throws an exception to signal unimplemented functionality.
    std::string generateMethodDefinition(const std::string &className, const ScaffoldModels::MethodModel &method)
    {
        // Convert the method's return type and parameters to their string representations.
        std::string returnTypeStr = GeneratorUtilities::dataTypeToString(method.returnType);
        std::string paramList = PropertiesGenerator::generateParameterList(method.parameters);

        // Define a default method body that throws an exception.
        // This is a placeholder indicating that the method is not yet implemented.
        std::string methodBody = "throw std::runtime_error(\"Not implemented\");";

        // Retrieve the declaration specifiers for the method.
        std::string declSpec = PropertiesGenerator::generateDeclarationSpecifier(method.declSpec);

        // Format the complete method definition with class qualification using std::format.
        return std::format("{}{} {}::{}({}) {{\n    {}\n}}\n",
                           declSpec,
                           returnTypeStr,
                           className,
                           method.name,
                           paramList,
                           methodBody);
    }
} // namespace MethodGenerator
