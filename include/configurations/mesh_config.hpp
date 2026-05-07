/**
 * @file mesh_config.hpp
 * @brief Configuration struct and functions for the Mesh build system.
 */
#pragma once

#include <string>
#include <string_view>
#include <vector>

/**
 * @struct MeshConfig
 * @brief Configuration struct for the Mesh build system.
 */
struct MeshConfig
{
    std::string projectName;
    std::string compiler;
    std::string outputPath;
    
    std::vector<std::string> sourceFiles;
    std::vector<std::string> flags;
};

/**
 * @brief Loads the Mesh configuration from a file.
 * 
 * @param path The path to the configuration file.
 * @return The loaded configuration.
 */
MeshConfig load_config(std::string_view path);