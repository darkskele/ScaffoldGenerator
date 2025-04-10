#include "CallableGenerator.h"
#include "PropertiesGenerator.h"
#include "GeneratorUtilities.h"
#include "CallableModels.h"

#include <sstream>
#include <format>
#include <stdexcept>

namespace CallableGenerator
{
    //--------------------------------------------------------------------------
    // Base Generators (operate on CallableModel)
    //--------------------------------------------------------------------------

    std::string generateCallableDeclaration(const CallableModels::CallableModel &callable)
    {
        // Convert the callable's return type to its string representation.
        std::string returnTypeStr = GeneratorUtilities::dataTypeToString(callable.returnType);

        // Generate the parameter list string.
        std::string paramList = PropertiesGenerator::generateParameterList(callable.parameters);

        // Retrieve the declaration specifiers (e.g., inline, static).
        std::string declSpec = PropertiesGenerator::generateDeclarationSpecifier(callable.declSpec);

        // Build a Doxygen comment block.
        std::string result = "/**\n";
        result += " * @brief " + callable.description + "\n";
        result += " */\n";

        // Construct the free callable declaration.
        if (callable.declSpec.isInline)
        {
            // Define a default body that signals unimplemented functionality.
            std::string body = "// TODO: Implement " + callable.name + " logic.\n";
            body += "    throw std::runtime_error(\"Not implemented\");";
            result += std::format("{}{} {}({}) {{\n    {}\n}}\n",
                                  declSpec,
                                  returnTypeStr,
                                  callable.name,
                                  paramList,
                                  body);
        }
        else
        {
            result += std::format("{}{} {}({});\n",
                                  declSpec,
                                  returnTypeStr,
                                  callable.name,
                                  paramList);
        }

        return result;
    }

    std::string generateCallableDefinition(const CallableModels::CallableModel &callable)
    {
        // Convert the callable's return type to its string representation.
        std::string returnTypeStr = GeneratorUtilities::dataTypeToString(callable.returnType);

        // Generate the parameter list string.
        std::string paramList = PropertiesGenerator::generateParameterList(callable.parameters);

        // Retrieve declaration specifiers.
        std::string declSpec = PropertiesGenerator::generateDeclarationSpecifier(callable.declSpec, true);

        // Define a default body that signals unimplemented functionality.
        std::string body = "// TODO: Implement " + callable.name + " logic.";
        // constexpr methods/functions cannot throw errors
        if (!declSpec.contains("constexpr"))
        {
            body += "\n    throw std::runtime_error(\"Not implemented\");";
        }
        else
        {
            // constexpr methods/functions must return something
            body += "\n    return" + (!returnTypeStr.contains("void") ? " " + returnTypeStr + "()" : "") + ";";
        }

        // Construct the free callable definition.
        // Inline methods do not get defined in cpp file
        std::string definition = "";
        if (!callable.declSpec.isInline)
        {
            definition = std::format("{}{} {}({}) {{\n    {}\n}}\n",
                                     declSpec,
                                     returnTypeStr,
                                     callable.name,
                                     paramList,
                                     body);
        }

        return definition;
    }

    //--------------------------------------------------------------------------
    // Method Generators (wrap the base generators)
    //--------------------------------------------------------------------------

    std::string generateMethodDeclaration(const CallableModels::MethodModel &method)
    {
        // Generate the free callable declaration using the base generator.
        std::string decl = generateCallableDeclaration(method);

        // Indent the declaration so it fits inside a class definition.
        return GeneratorUtilities::indentCode(decl);
    }

    std::string generateMethodDefinition(const std::string &className, const CallableModels::MethodModel &method)
    {
        // Instead of directly using the base definition, rebuild the definition so that the function
        // name is qualified with the owning class name.
        std::string returnTypeStr = GeneratorUtilities::dataTypeToString(method.returnType);
        std::string paramList = PropertiesGenerator::generateParameterList(method.parameters);
        std::string declSpec = PropertiesGenerator::generateDeclarationSpecifier(method.declSpec, true);
        // Construct body of method with TODO
        std::string body = "// TODO: Implement " + method.name + " logic.";
        // constexpr methods/functions cannot throw errors
        if (!declSpec.contains("constexpr"))
        {
            body += "\n    throw std::runtime_error(\"Not implemented\");";
        }
        else
        {
            // constexpr methods/functions must return something
            body += "\n    return" + (!returnTypeStr.contains("void") ? " " + returnTypeStr + "()" : "") + ";";
        }

        // Construct the fully qualified method name.
        std::string qualifiedName = className + "::" + method.name;

        // Construct the method definition.
        std::string definition = "";
        // Inline methods do not get defined in cpp file
        if (!method.declSpec.isInline)
        {
            definition = std::format("{}{} {}({}) {{\n    {}\n}}\n",
                                     declSpec,
                                     returnTypeStr,
                                     qualifiedName,
                                     paramList,
                                     body);
        }

        return definition;
    }

} // namespace CallableGenerator
