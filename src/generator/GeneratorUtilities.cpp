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

        if(ScaffoldProperties::hasQualifier(tQ, Qualifer::CONST))
            result += "const ";
        
        if(ScaffoldProperties::hasQualifier(tQ, Qualifer::VOLATILE))
            result += "volatile ";

        return result;
    }
} // namespace

namespace GeneratorUtilities
{
    std::string dataTypeToString(const ScaffoldProperties::DataType& dt)
    {
        using Type = ScaffoldProperties::Types;

        std::string result = typeQualiferToString(dt.qualifiers);
        
        switch(dt.type)
        {
            case Type::VOID:        return result + "void";
            case Type::INT:         return result + "int";
            case Type::UINT:        return result + "unsigned int";
            case Type::LONG:        return result + "long";
            case Type::ULONG:       return result + "unsigned long";
            case Type::LONGLONG:    return result + "long long";
            case Type::ULONGLONG:   return result + "unsigned long long";
            case Type::FLOAT:       return result + "float";
            case Type::DOUBLE:      return result + "double";
            case Type::BOOL:        return result + "bool";
            case Type::STRING:      return result + "string";
            case Type::CHAR:        return result + "char";
            case Type::AUTO:        return result + "auto";
            case Type::CUSTOM:
            {
                if(dt.customType)
                {
                    return result + *dt.customType;
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