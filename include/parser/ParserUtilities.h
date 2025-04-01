/**
 * @file ParserUtilities.h
 * @brief Contain various parser utilities.
 */

#pragma once

#include <string_view>

/**
 * @namespace ParserUtilities
 * @brief Various utility functions for the parser.
 */
namespace ParserUtilities
{
    /**
     * @brief Trims whitespace from both ends of a string_view.
     * @param sv The string_view to trim.
     * @return A trimmed string_view.
     */
    std::string_view trim(std::string_view sv);
    
} // namespace ParserUtilities