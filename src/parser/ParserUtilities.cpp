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

    std::vector<std::string_view> split(std::string_view input, char delimiter)
    {
        std::vector<std::string_view> tokens;

        // Walk through the string view, splitting at each delimiter
        size_t start = 0;
        while (start < input.size())
        {
            size_t end = input.find(delimiter, start);
            if (end == std::string_view::npos)
            {
                tokens.push_back(input.substr(start));
                break;
            }
            tokens.push_back(input.substr(start, end - start));
            start = end + 1;
        }

        return tokens;
    }

    void flushBlock(std::vector<std::string_view>& buffer, const std::function<void(const std::vector<std::string_view>&)>& handler)
    {
        // If there's anything to flush, call the handler and clear the buffer
        if (!buffer.empty())
        {
            handler(buffer);
            buffer.clear();
        }
    }
    
} // namespace ParserUtilities
