/**
 * @file main.cpp
 * @brief The main entry point of the Mesh build system.
 */
#define MESH_VERSION "0.0.1"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "configurations/mesh_config.hpp"

namespace fs = std::filesystem;

/**
 * Matches a simple glob pattern against a filename.
 * Supports only the '*' wildcard character.
 * Huge thanks to ChatGPT for helping in this area!
 * 
 * @param pattern The glob pattern to match against.
 * @param filename The filename to match against the pattern.
 * @return True if the filename matches the pattern, false otherwise.
 */
bool match_simple_glob(const std::string& pattern, const std::string& filename)
{
    // supports only: "*"
    if (pattern == "*")
        return true;

    auto star = pattern.find('*');

    if (star == std::string::npos)
        return pattern == filename;

    std::string prefix = pattern.substr(0, star);
    std::string suffix = pattern.substr(star + 1);

    if (filename.size() < prefix.size() + suffix.size())
        return false;

    return filename.starts_with(prefix) &&
           filename.ends_with(suffix);
}

/**
 * Expands a list of source file patterns into a list of matching file paths.
 * Supports simple glob patterns with the '*' wildcard character.
 * Huge thanks to ChatGPT for helping in this area!
 * 
 * @param patterns The list of source file patterns to expand.
 * @return The list of matching file paths.
 */
std::vector<std::string> expand_sources(const std::vector<std::string>& patterns)
{
    std::vector<std::string> result;

    // Expand each pattern into a list of matching file paths
    for (const auto& pattern : patterns)
    {
        fs::path dir = ".";
        std::string filePattern = pattern;

        auto pos = pattern.find_last_of("/\\");
        if (pos != std::string::npos)
        {
            dir = pattern.substr(0, pos);
            filePattern = pattern.substr(pos + 1);
        }

        if (!fs::exists(dir))
            continue;

        // Match each file against the pattern and add it to the result if it matches
        for (const auto& entry : fs::directory_iterator(dir))
        {
            if (!entry.is_regular_file())
                continue;

            std::string filename = entry.path().filename().string();

            if (match_simple_glob(filePattern, filename))
            {
                result.push_back(entry.path().string());
            }
        }
    }

    return result;
}

/**
 * Builds the command to run the compiler with the given configuration.
 * 
 * @param config The configuration to build the command from.
 * @return The command as a vector of strings.
 */
std::vector<std::string> build_command(const MeshConfig& config)
{
    // This is probably not the most efficient way to build the command, but it works for now.
    std::vector<std::string> cmd;

    cmd.push_back(config.compiler);

    auto expandedSources = expand_sources(config.sourceFiles);
    cmd.insert(cmd.end(), expandedSources.begin(), expandedSources.end());

    cmd.insert(cmd.end(), config.flags.begin(), config.flags.end());

    fs::path output = fs::path(config.outputPath) / config.projectName;

    cmd.push_back("-o");
    cmd.push_back(output.string());

    return cmd;
}

/**
 * Joins a vector of strings into a single command string, with each part separated by a space.
 * 
 * @param cmd The vector of strings to join.
 * @return The joined command string.
 */
std::string join_command(const std::vector<std::string>& cmd)
{
    std::string result;

    // Join the command parts into a single string
    for (const auto& part : cmd)
    {
        result += part + " ";
    }

    if (!result.empty())
        result.pop_back();

    return result;
}

/**
 * Writes the mesh.toml configuration file to the project directory.
 * 
 * @param projectDir The directory of the project.
 * @param projectName The name of the project.
 */
void write_mesh_file(const fs::path& projectDir, const std::string& projectName)
{
    // Generate the mesh.toml file for the project
    std::ofstream file(projectDir / "mesh.toml");

    if (!file)
    {
        std::cerr << "Failed to create mesh.toml\n";
        std::exit(1);
    }

    file << "project_name = \"" << projectName << "\"\n";
    file << "compiler = \"clang++\"\n";
    file << "output_path = \"bin\"\n";

    file << "source_files = [\"src/*.cpp\"]\n";
    file << "flags = [\"-Wall\", \"-Werror\"]\n";
}

/**
 * Writes the main.cpp file to the project directory.
 * 
 * @param projectDir The directory of the project.
 */
void write_main_cpp(const fs::path& projectDir)
{
    // Generate the main.cpp file for the project
    fs::path filePath = projectDir / "src" / "main.cpp";
    std::ofstream file(filePath);

    if (!file)
    {
        std::cerr << "Failed to create main.cpp\n";
        std::exit(1);
    }

    file << R"(#include <iostream>

int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
)";
}

/**
 * Creates a new project with the given name.
 * 
 * @param name The name of the project.
 */
void create_project(const std::string& name)
{
    fs::path projectDir = name;

    fs::create_directories(projectDir / "src");
    fs::create_directories(projectDir / "include");
    fs::create_directories(projectDir / "bin");

    write_mesh_file(projectDir, name);
    write_main_cpp(projectDir);

    std::cout << "Created project: " << name << "\n";
}

/**
 * Runs the project by building and executing the configured command.
 */
void run_project()
{
    if (!fs::exists("mesh.toml"))
    {
        std::cerr << "Error: mesh.toml not found\n";
        std::exit(1);
    }

    MeshConfig config = load_config("mesh.toml");

    auto cmdParts = build_command(config);
    std::string fullCommand = join_command(cmdParts);

    std::cout << "Running: " << fullCommand << "\n";

    // NOTE: std::system is unsafe and I will replace it with a safer alternative in the future
    std::system(fullCommand.c_str());
    std::system((fs::path(config.outputPath) / config.projectName).string().c_str());
}

/**
 * The main entry point of Mesh.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 */
int main(int argc, char* argv[])
{
    // Check if the command is valid
    if (argc < 2)
    {
        std::cerr << "Usage:\n";
        std::cerr << "  mesh run\n";
        std::cerr << "  mesh version\n";
        std::cerr << "  mesh help\n";
        std::cerr << "  mesh create <project_name>\n";
        return 1;
    }
    
    std::string command = argv[1];

    if (command == "run")
    {
        run_project();
    }
    else if (command == "version")
    {
        std::cout << "Current version: " << MESH_VERSION << "\n";
    }
    else if (command == "help")
    {
        std::cout <<
        R"(Mesh - A simple C/C++ build system
        
        Usage:
          mesh run
              Builds and runs the current project
        
          mesh create <project_name>
              Creates a new project scaffold
        
          mesh version
              Prints the current Mesh version
        
          mesh help
              Shows this help message
        )";
    }
    else if (command == "create")
    {
        if (argc != 3)
        {
            std::cerr << "Usage: mesh create <project_name>\n";
            return 1;
        }

        create_project(argv[2]);
    }
    else
    {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }
    
    return 0;
}