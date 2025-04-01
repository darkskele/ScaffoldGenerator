#include "MethodGenerator.h"
#include "PropertiesGenerator.h"
#include "GeneratorUtilities.h"
#include <format>

namespace MethodGenerator
{
    std::string generateMethodDeclaration(const ScaffoldModels::MethodModel &method)
    {
        std::string returnTypeStr = GeneratorUtilities::dataTypeToString(method.returnType);
        std::string paramList = PropertiesGenerator::generateParameterList(method.parameters);
        std::string declSpec = PropertiesGenerator::generateDeclarationSpecifier(method.declSpec);

        // Begin Doxygen comment block and include method description.
        // Close the Doxygen block with "*/" before adding the method declaration.
        std::string result = "    /**\n     * @brief " + method.description + "\n     */\n";
        result += std::format("    {}{} {}({});\n", declSpec, returnTypeStr, method.name, paramList);

        return result;
    }

    std::string generateMethodDefinition(const std::string &className, const ScaffoldModels::MethodModel &method)
    {
        std::string returnTypeStr = GeneratorUtilities::dataTypeToString(method.returnType);
        std::string paramList = PropertiesGenerator::generateParameterList(method.parameters);
        // Default method body: placeholder that throws an exception.
        std::string methodBody = "throw std::runtime_error(\"Not implemented\");";
        // Get string representation of declaration specifiers
        std::string declSpec = PropertiesGenerator::generateDeclarationSpecifier(method.declSpec);
        // Build the definition with class qualification.
        return std::format("{}{} {}::{}({}) {{\n    {}\n}}\n",
                           declSpec,
                           returnTypeStr,
                           className,
                           method.name,
                           paramList,
                           methodBody);
    }

} // namespace MethodGenerator
