# The Lost Vault

A 2D treasure-hunt adventure game built with C++17, OpenGL 3.3 Core Profile, GLFW, GLAD, and GLM.

## Prerequisites

- C++17 compatible compiler (MSVC, GCC, Clang)
- CMake >= 3.16
- Git (for CMake FetchContent)

## Building the Project

```bash
# From repository root
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

## Running the Executable

Run the compiled binary from the repository root or the build directory:

```bash
# Windows (Debug)
.\build\Debug\TheLostVault.exe

# Linux / macOS
./build/TheLostVault
```

## Controls

- **Escape**: Close window / Pause
