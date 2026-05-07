# Mesh Build System (C/C++)

Mesh is a lightweight build system for C and C++ projects. It’s currently under active development and aims to make building small to medium-sized projects simpler using a clean TOML-based configuration.

> ⚠️ **Note:** Mesh is still in development, so expect bugs and breaking changes. It’s not recommended yet for large or production-critical projects.

---

## Features

* Simple CLI tool
* TOML-based configuration (`mesh.toml`)
* Supports C and C++ projects
* C++20 standard support
* Works with any compiler that supports C++20
* Lightweight and fast
* Basic glob support for source files (e.g. `src/*.cpp`)

---

## Commands

* `help` → Show available commands
* `version` → Display Mesh version
* `create [project_name]` → Create a new project structure
* `run` → Build and run the project (must be executed inside a directory containing a `mesh.toml` file)

---

## Creating a Project

You can create a project using:

```bash
mesh create [project_name]
```

This automatically generates a ready-to-use project structure, including:

* `mesh.toml` (preconfigured)
* `src/` folder with a `main.cpp` "Hello World" program
* `include/` folder for headers
* `bin/` folder where the compiled executable will be placed

This is meant to give you an instant starting point so you can focus on coding instead of setup.

---

## Running a Project

To build and run a project, use:

```bash
mesh run
```

This command works as long as there is a `mesh.toml` file in the current directory. Mesh will automatically read the configuration, compile the source files, and run the resulting executable.

---

## Project Configuration (mesh.toml)

Mesh uses a `mesh.toml` file to define how a project is built.

Example:

```toml
project_name = "my_project"
compiler = "clang++"
output_path = "bin"
source_files = ["src/*.cpp"]
flags = ["-Wall", "-Werror"]
```

This keeps things simple while still giving enough flexibility for most small projects.

---

## Building the Project

Mesh does not provide a prebuilt executable. You must compile it yourself using a C++20-compatible compiler.

Example build command:

```bash
clang++ src/main.cpp src/mesh_config.cpp -std=c++20 -Wall -Wextra -Wpedantic -Werror -O3 -Iinclude -o mesh
```

You can adjust the compiler and flags depending on your system.

---

## Adding Mesh to PATH

To use `mesh` from anywhere in your terminal, you need to add it to your system PATH.

### Windows

1. Rename the output to `mesh.exe`
2. Move it to a folder like `C:\mesh\`
3. Add that folder to PATH via Environment Variables
4. Restart your terminal

Then you can run:

```bash
mesh help
```

---

### Linux / macOS

```bash
mv mesh /usr/local/bin/mesh
chmod +x /usr/local/bin/mesh
```

Now you can use:

```bash
mesh help
```

---

## Contribution

Contributions are welcome!

If you want to help:

* Fork the repo
* Make changes
* Submit a pull request

Even small fixes or improvements are appreciated.

---

## Future Plans

* Rewrite core system for stability and to support other languages
* Better dependency handling
* Improved error messages
* Faster parsing and build speed
* Custom scripting language (maybe)

---

## Closing Note

Mesh is a personal project built to make small C/C++ (and possibly other langaues) projects easier to manage without dealing with heavy build systems. It’s meant to stay simple, readable, and easy to experiment with.

Thanks for checking it out 🙌
