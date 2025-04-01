#include "ParserUtilities.h"
#include <cctype> // for std::isspace

namespace ParserUtilities
{
    std::string_view trim(std::string_view sv)
    {
        // Remove leading whitespace by removing the first character until non-space is found.
        while (!sv.empty() && std::isspace(static_cast<char>(sv.front())))
        {
            sv.remove_prefix(1); // Remove one character from the beginning.
        }
        // Remove trailing whitespace by removing the last character until non-space is found.
        while (!sv.empty() && std::isspace(static_cast<char>(sv.back())))
        {
            sv.remove_suffix(1); // Remove one character from the end.
        }
        // Return the trimmed string_view.
        return sv;
    }
} // namespace ParserUtilities
