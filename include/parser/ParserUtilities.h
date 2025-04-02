/**
 * @file ParserUtilities.h
 * @brief Contains various parser utility functions.
 *
 * This file provides utility functions for common parsing tasks used throughout the scaffolder DSL.
 * It leverages modern C++20 features, such as std::string_view, for zero-copy string manipulation,
 * ensuring both efficiency and clarity in parsing operations.
 *
 * @note The functions defined herein adhere to robust error handling practices and are designed
 *       to work efficiently in high-performance contexts.
 */

#pragma once

#include <string_view>
#include <functional>
#include <vector>

/**
 * @namespace ParserUtilities
 * @brief Provides utility functions for parsing.
 *
 * This namespace contains functions that assist with common string parsing operations.
 * Currently, it includes a function to trim whitespace from both ends of a std::string_view.
 */
namespace ParserUtilities
{
    /**
     * @brief Trims whitespace from both ends of a std::string_view.
     *
     * This function removes any leading and trailing whitespace characters from the provided
     * string_view and returns a new string_view that references the trimmed portion without
     * performing any dynamic memory allocation.
     *
     * @param sv The std::string_view to trim.
     * @return A std::string_view with leading and trailing whitespace removed.
     *
     * @note Utilizes std::isspace to identify whitespace characters.
     */
    std::string_view trim(std::string_view sv);

    /**
     * @brief Splits a string view by a delimiter.
     *
     * This function returns a vector of substrings by splitting the input
     * string view at each occurrence of the delimiter character.
     * Whitespace is preserved and must be trimmed externally if needed.
     *
     * @param input The string to split.
     * @param delimiter The character delimiter to split on.
     * @return A vector of substrings as std::string_view.
     */
    std::vector<std::string_view> split(std::string_view input, char delimiter);

    /**
     * @brief Flushes a buffered block and dispatches it to a handler.
     *
     * This utility executes a provided callback to process buffered lines,
     * then clears the buffer. It is useful when parsing nested DSL blocks
     * (e.g., methods, constructors, etc.).
     *
     * @param buffer Reference to the buffer of DSL lines.
     * @param handler A function that accepts the buffered block.
     */
    void flushBlock(std::vector<std::string_view> &buffer, const std::function<void(const std::vector<std::string_view> &)> &handler);
} // namespace ParserUtilities
