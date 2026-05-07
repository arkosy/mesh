#include <iostream>
#include <toml++/toml.hpp>

#include "configurations/mesh_config.hpp"
#include "utils/toml_utils.hpp"

/**
 * @brief Loads the configuration from the given TOML file path.
 * 
 * @param path The path to the TOML file.
 * @return The loaded configuration.
 */
MeshConfig load_config(std::string_view path)
{
    toml::table table;

    // Parse the TOML file
    // Probably not the best way to parse the TOML file
    try
    {
        table = toml::parse_file(std::string(path));
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR: failed to parse TOML: " << e.what() << "\n";
        std::exit(1);
    }

    // Define fatal error handler
    auto fatal = [](std::string_view msg)
    {
        std::cerr << "ERROR: " << msg << "\n";
        std::exit(1);
    };

    // Define require helper
    auto require = [&](auto opt, std::string_view name)
    {
        if (!opt || opt->empty())
        {
            std::cerr << "ERROR: " << name << " not set in mesh.toml\n";
            std::exit(1);
        }
        return *opt;
    };

    MeshConfig config;

    // Get the configuration values
    auto projectName = toml_utils::get<std::string>(table, "project_name");
    auto compiler    = toml_utils::get<std::string>(table, "compiler");
    auto outputPath  = toml_utils::get<std::string>(table, "output_path");
    auto sourceFiles = toml_utils::get_array<std::string>(table, "source_files");
    auto flags       = toml_utils::get_array<std::string>(table, "flags");

    config.projectName = projectName.value_or("unnamed");
    config.compiler    = require(compiler, "compiler");
    config.outputPath  = require(outputPath, "output_path");

    if (!sourceFiles || sourceFiles->empty())
        fatal("source_files is empty or missing");

    config.sourceFiles = *sourceFiles;
    config.flags       = flags.value_or(std::vector<std::string>{});

    return config;
}
