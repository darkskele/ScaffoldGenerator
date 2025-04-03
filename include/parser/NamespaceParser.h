/**
 * @file NamespaceParser.h
 * @brief Provides functions to parse namespace blocks from the scaffolder DSL.
 *
 * This file declares functions for processing DSL input that defines C++ namespaces.
 * A namespace block may contain properties (such as a description) and nested blocks for
 * classes, functions, or even other namespaces. Methods are not allowed at the namespace level.
 * The parser uses a consume-as-you-go approach with a std::deque of std::string_view to ensure each line
 * is processed exactly once.
 */

#pragma once

#include "CodeGroupModels.h"

#include <string>
#include <string_view>
#include <deque>
#include <vector>
#include <optional>

/**
 * @namespace NamespaceParser
 * @brief Provides functions to parse namespace blocks from the scaffolder DSL.
 *
 * This namespace defines functions that process DSL lines to construct
 * a NamespaceModel. A namespace block may contain a description property
 * and nested blocks for classes, functions, and nested namespaces.
 * Methods cannot be declared directly in a namespace.
 */
namespace NamespaceParser
{
    /**
     * @brief Parses a namespace block from the DSL using a consume-as-you-go approach.
     *
     * This function processes DSL lines from the provided deque, consuming each line as it is handled.
     * It supports a namespace-level description (via a property line starting with '|') and nested blocks:
     * - Nested namespaces (using the "namespace" keyword)
     * - Classes (using the "class" keyword)
     * - Free functions (using the "function" keyword)
     *
     * If a "method" block is encountered, a std::runtime_error is thrown.
     *
     * @param nsName Optional name for the namespace; if not provided, an anonymous namespace is assumed.
     * @param lines A deque of DSL lines representing the namespace block. Lines are consumed as they are processed.
     * @return CodeGroupModels::NamespaceModel The parsed namespace model.
     *
     * @throws std::runtime_error on malformed input or unknown keywords.
     */
    CodeGroupModels::NamespaceModel parseNamespaceBlock(const std::optional<std::string> &nsName,
                                                        std::deque<std::string_view> &lines);

} // namespace NamespaceParser
