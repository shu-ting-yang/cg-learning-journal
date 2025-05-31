# CMake + Make Build Flow for OpenGL Projects (Triangle Example)

This post summarizes the build system flow using **CMake** and **Make** for a simple OpenGL triangle rendering project, using **GLFW**, **GLAD**, and **C++**.

---

## Folder Structure

Your project layout might look like this:

```text
triangle_glfw/
├── include/               # GLAD headers
│   └── glad/
├── src/                   # C++ source files
│   ├── main.cpp
│   └── glad.c
├── CMakeLists.txt         # Build instructions
├── build/                 # CMake-generated build folder (after configuration)
```

---

## What is CMake?

CMake is a **cross-platform build system generator**. It reads `CMakeLists.txt` to understand how to build your code, and generates a platform-specific build system (e.g., Makefiles on macOS/Linux).

---

## What is Make?

Make is a **build tool** that uses the generated `Makefile` to:

* Compile your `.cpp` files
* Link them into an executable
* Track which files have changed and only rebuild what's needed

---

## Build Workflow

You’ll typically follow this 3-step process:

### 1. Generate the Build System (using CMake)

```bash
mkdir build
cd build
cmake ..
```

This runs CMake and generates the `Makefile` based on your `CMakeLists.txt`.

The `..` means: “Look for `CMakeLists.txt` in the parent folder.”

---

### 2. Build the Project (using Make)

```bash
make
```

This compiles your project using the rules from the generated `Makefile`.

It will produce an executable, typically named based on `add_executable(...)` in your `CMakeLists.txt`.

Example:

```cmake
add_executable(Triangle src/main.cpp src/glad.c)
```

→ Output will be `Triangle`

---

### 3. Run the Program

From inside the `build/` folder:

```bash
./Triangle
```

You should see your first triangle rendered in a GLFW window.

---

## When to Rerun CMake?

| You changed...          | Run `make` again? | Run `cmake ..` again? |
| ----------------------- | ----------------- | --------------------- |
| Edited `.cpp` file      | Yes               | No                    |
| Added new source/header | Yes               | Yes (recommended)     |
| Edited `CMakeLists.txt` | Yes               | Yes                   |
| Changed library setup   | Yes               | Yes                   |

---

## Optional: Clean Rebuild

If things get weird or stale:

```bash
rm -rf build/
mkdir build
cd build
cmake ..
make
```

This resets the entire build state.

---

## Summary

| Tool         | Role                                         |
| ------------ | -------------------------------------------- |
| `CMake`      | Generates build system from `CMakeLists.txt` |
| `Make`       | Compiles source files into an executable     |
| `./Triangle` | Runs the program after successful build      |

This setup works great for small OpenGL projects and scales easily with more complexity later (e.g., shader loading, asset pipelines, camera systems).

---

Happy coding!
