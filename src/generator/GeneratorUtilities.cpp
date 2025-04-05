#include "GeneratorUtilities.h"

#include <stdexcept>
#include <string>
#include <sstream>

/**
 * @namespace
 * @brief Anonymous namespace for internal helper functions used by GeneratorUtilities.
 *
 * The functions within this namespace are implementation details and are not exposed outside
 * of this translation unit. They provide efficient and modern implementations for converting
 * type qualifiers and declarators to string representations.
 */
namespace
{
    /**
     * @brief Converts a TypeQualifier enum to its string representation.
     *
     * This function examines the provided type qualifier flags and returns a string that includes
     * the corresponding C++ qualifiers (e.g., "const", "volatile"). The resulting string is suitable
     * for inclusion in type definitions.
     *
     * @param tQ A constant reference to the TypeQualifier enum value.
     * @return A std::string containing the qualifier(s) followed by a space if applicable.
     *
     * @note Designed for efficiency using simple concatenation and minimal temporary allocations.
     */
    std::string typeQualifierToString(const PropertiesModels::TypeQualifier &tQ)
    {
        using Qualifier = PropertiesModels::TypeQualifier;
        std::string result;

        // Append 'const' qualifier if present.
        if (PropertiesModels::hasQualifier(tQ, Qualifier::CONST))
            result += "const ";

        // Append 'volatile' qualifier if present.
        if (PropertiesModels::hasQualifier(tQ, Qualifier::VOLATILE))
            result += "volatile ";

        return result;
    }

    /**
     * @brief Converts a TypeDeclarator structure to its string representation.
     *
     * This function constructs a string that represents the type declarator, including pointers,
     * references, and array dimensions. It ensures that the declarator conforms to valid C++ syntax.
     *
     * @param tD A constant reference to the TypeDeclarator structure.
     * @return A std::string representing the type declarator (e.g., "*&", "&&", "[10]").
     *
     * @throws std::runtime_error if the declarator is malformed, such as combining references with arrays,
     *         or specifying both lvalue and rvalue references simultaneously.
     *
     * @note The function processes pointers first, followed by reference symbols, and finally appends
     *       any array dimensions, ensuring optimal performance and adherence to C++ syntax rules.
     */
    std::string typeDeclaratorToString(const PropertiesModels::TypeDeclarator &tD)
    {
        std::string result;

        // Check for invalid combinations: references cannot be combined with array dimensions.
        if ((tD.isLValReference || tD.isRValReference) && (!tD.arrayDimensions.empty()))
            throw std::runtime_error("Array of references are not allowed!");

        // Check for conflicting reference types: cannot have both lvalue and rvalue references simultaneously.
        if (tD.isLValReference && tD.isRValReference)
            throw std::runtime_error("Lvalues and Rvalues are not allowed at the same time!");

        // Efficiently append pointer symbols.
        for (int i = 0; i < tD.ptrCount; ++i)
            result += '*';

        // Append reference symbols as needed.
        if (tD.isLValReference)
            result += '&';
        if (tD.isRValReference)
            result += "&&";

        // Append each array dimension using proper C++ array syntax.
        for (const auto &dim : tD.arrayDimensions)
            result += '[' + dim + ']';

        return result;
    }
} // end anonymous namespace

namespace GeneratorUtilities
{
    // This function processes a DataType object defined in the PropertiesModels namespace,
    // combining type qualifiers, the base type, and type declarators into a cohesive string.
    // It supports built-in types, custom types, and compound types with qualifiers and modifiers.
    std::string dataTypeToString(const PropertiesModels::DataType &dt)
    {
        using Type = PropertiesModels::Types;

        // Convert type qualifiers and declarator to their string representations.
        std::string quals = typeQualifierToString(dt.qualifiers);
        std::string decl = typeDeclaratorToString(dt.typeDecl);

        // Determine the base type and combine with qualifiers and declarator.
        switch (dt.type)
        {
        case Type::VOID:
            return quals + "void" + decl;
        case Type::INT:
            return quals + "int" + decl;
        case Type::UINT:
            return quals + "unsigned int" + decl;
        case Type::LONG:
            return quals + "long" + decl;
        case Type::ULONG:
            return quals + "unsigned long" + decl;
        case Type::LONGLONG:
            return quals + "long long" + decl;
        case Type::ULONGLONG:
            return quals + "unsigned long long" + decl;
        case Type::FLOAT:
            return quals + "float" + decl;
        case Type::DOUBLE:
            return quals + "double" + decl;
        case Type::BOOL:
            return quals + "bool" + decl;
        case Type::STRING:
            return quals + "std::string" + decl;
        case Type::CHAR:
            return quals + "char" + decl;
        case Type::AUTO:
            return quals + "auto" + decl;
        case Type::CUSTOM:
            if (dt.customType)
            {
                return quals + *dt.customType + decl;
            }
            else
            {
                throw std::runtime_error("Custom type specified without a name!");
            }
        default:
            throw std::runtime_error("Unknown data type.");
        }
    }

    // Helper function to indent code.
    std::string indentCode(const std::string &code, int indentLevel)
    {
        // Create an input string stream to read the code line by line.
        std::istringstream iss(code);

        // Create an output string stream to accumulate the indented code.
        std::ostringstream oss;

        // Construct a string that contains 'indentLevel' spaces.
        std::string indent(indentLevel, ' ');

        // Temporary variable to store each line read from the input code.
        std::string line;

        // Read the code line by line until the end of the input stream.
        while (std::getline(iss, line))
        {
            // For each line, prepend the indent string and then append a newline.
            oss << indent << line << "\n";
        }

        // Return the complete indented code as a string.
        return oss.str();
    }

    std::string removeRootPrefix(const std::string &path)
    {
        const std::string rootPrefix = "ROOT/";
        if (path.rfind(rootPrefix, 0) == 0)
        { // Check if path starts with "ROOT/"
            return path.substr(rootPrefix.length());
        }
        return path;
    }

} // namespace GeneratorUtilities
