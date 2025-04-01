#include "ParserUtilities.h"
#include <string>

namespace ParserUtilities
{
    std::string_view trim(std::string_view sv)
    {
        while (!sv.empty() && std::isspace(static_cast<char>(sv.front())))
        {
            sv.remove_prefix(1); // Prune whitespace from the front
        }
        while (!sv.empty() && std::isspace(static_cast<char>(sv.back())))
        {
            sv.remove_suffix(1); // Prune whitespace from the back
        }

        return sv;
    }

} // namespace ParserUtilities