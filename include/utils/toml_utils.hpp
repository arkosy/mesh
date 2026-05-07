/**
 * @file toml_utils.hpp
 * @brief Provides safe and simplified access to TOML configuration data.
 * 
 * This module wraps toml++ parsing and provides helper functions
 * to avoid repetitive checks when reading configuration values.
 */
#pragma once

#include <optional>
#include <string_view>
#include <toml++/toml.hpp>
#include <vector>

/**
 * @namespace toml_utils
 * @brief Provides utility functions for working with TOML configuration data.
 */
namespace toml_utils
{
    /**
     * @brief Retrieves a value of type T from the TOML table using the provided key.
     * 
     * @tparam T The type of the value to retrieve.
     * @param table The TOML table to retrieve the value from.
     * @param key The key of the value to retrieve.
     * @return std::optional<T> The retrieved value, or std::nullopt if the key is not found.
     */
    template<typename T>
    std::optional<T> get(const toml::table& table, std::string_view key)
    {
        // Attempt to retrieve the node using the key
        if (auto node = table.get(key))
        {
            // Attempt to retrieve the value as type T
            if (auto value = node->value<T>())
            {
                return value;
            }
        }
        
        // Return nullopt if the key is not found or the value cannot be converted to type T
        return std::nullopt;
    }

    /**
     * @brief Retrieves an array of values of type T from the TOML table using the provided key.
     * 
     * @tparam T The type of the values to retrieve.
     * @param table The TOML table to retrieve the array from.
     * @param key The key of the array to retrieve.
     * @return std::optional<std::vector<T>> The retrieved array, or std::nullopt if the key is not found.
     */
    template<typename T>
    std::optional<std::vector<T>> get_array(const toml::table& table, std::string_view key)
    {
        // Attempt to retrieve the node using the key
        if (auto node = table.get(key))
        {
            if (auto arr = node->as_array())
            {
                std::vector<T> result;
                result.reserve(arr->size());

                // Iterate over the array elements and convert them to type T
                for (auto&& elem : *arr)
                {
                    
                    if (auto value = elem.value<T>())
                    {
                        result.push_back(*value);
                    }
                    else
                    {
                        // Type mismatch inside array → fail whole thing
                        return std::nullopt;
                    }
                }
    
                return result;
            }
        }

        return std::nullopt;
    }
}