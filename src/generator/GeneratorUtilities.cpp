#include "GeneratorUtilities.h"
#include <stdexcept>

/**
 * @namespace Anonymous
 * @brief Anonymous namespace to provide helper functions for Generator utilites
 */
namespace
{
    /**
     * @brief Converts type qualifier enum to string
     * @param tQ Type qualifier to convert
     * @return String representation of type qualifier
     */
    std::string typeQualiferToString(const ScaffoldProperties::TypeQualifier &tQ)
    {
        using Qualifer = ScaffoldProperties::TypeQualifier;

        std::string result = "";

        if (ScaffoldProperties::hasQualifier(tQ, Qualifer::CONST))
            result += "const ";

        if (ScaffoldProperties::hasQualifier(tQ, Qualifer::VOLATILE))
            result += "volatile ";

        return result;
    }

    /**
     * @brief Converts type declarator struct to string
     * @param tD Type declarator to convert
     * @return String representation of type declarator
     * 
     * @throws std::runtime_error if declarator is malformed
     */
    std::string typeDeclaratorToString(const ScaffoldProperties::TypeDeclarator &tD)
    {
        using Declarator = ScaffoldProperties::TypeDeclarator;

        std::string result = "";

        // &[] declarators are not valid in c++
        if((tD.isLValReference || tD.isRValReference) && (tD.arrayDimensions.size() > 0))
            throw std::runtime_error("Array of references are not allowed!");

        // Cannot have a lvalue ref and rvalue ref at the same time
        if(tD.isLValReference && tD.isRValReference)
            throw std::runtime_error("Lvalues and Rvalues are not allowed at the same time!");

        // Pointer declarator is always first
        for(int i = 0; i < tD.ptrCount; ++i)
            result += '*';

        // Print ampersands as neede
        if(tD.isLValReference)
            result += '&';
        if(tD.isRValReference)
            result += "&&";

        for(const auto& dim : tD.arrayDimensions)
            result += '[' + dim + ']';

        return result;
    }

} // namespace

namespace GeneratorUtilities
{
    std::string dataTypeToString(const ScaffoldProperties::DataType &dt)
    {
        using Type = ScaffoldProperties::Types;

        std::string quals = typeQualiferToString(dt.qualifiers);
        std::string decl = typeDeclaratorToString(dt.typeDecl);

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
            return quals + "string" + decl;
        case Type::CHAR:
            return quals + "char" + decl;
        case Type::AUTO:
            return quals + "auto" + decl;
        case Type::CUSTOM:
        {
            if (dt.customType)
            {
                return quals + *dt.customType + decl;
            }
            else
            {
                throw std::runtime_error("Custom type specified without a name!");
            }
        }
        default:
        {
            throw std::runtime_error("Unknown data type.");
        }
        }
    }

} // namespace GeneratorUtilities