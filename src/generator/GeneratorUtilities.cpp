#include "GeneratorUtilities.h"
#include <stdexcept>

namespace GeneratorUtilities
{
    std::string dataTypeToString(const ScaffoldProperties::DataType& dt)
    {
        using Type = ScaffoldProperties::Types;
        
        switch(dt.type)
        {
            case Type::VOID:        return "void";
            case Type::INT:         return "int";
            case Type::UINT:        return "unsigned int";
            case Type::LONG:        return "long";
            case Type::ULONG:       return "unsigned long";
            case Type::LONGLONG:    return "long long";
            case Type::ULONGLONG:   return "unsigned long long";
            case Type::FLOAT:       return "float";
            case Type::DOUBLE:      return "double";
            case Type::BOOL:        return "bool";
            case Type::STRING:      return "string";
            case Type::CHAR:        return "char";
            case Type::CUSTOM:
            {
                if(dt.customType)
                {
                    return *dt.customType;
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