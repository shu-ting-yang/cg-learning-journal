## Debugging glTF Viewer on macOS (OpenGL 4.1, Apple Silicon)

**Date:** 2025-06-09

This post documents my experience bringing up the [`gltf-viewer`](https://gitlab.com/gltf-viewer-tutorial/gltf-viewer) tutorial on macOS (OpenGL 4.1, Apple M1). It’s a combination of concise bring-up log and detailed debug steps. This may serve as a reference for future setups or other developers working on macOS + GLFW + OpenGL.

---

## TL;DR: Summary of Key Issues & Fixes

| Step | Issue                             | Fix                                                   | Notes                                                    |
| ---- | --------------------------------- | ----------------------------------------------------- | -------------------------------------------------------- |
| 1    | CMake compatibility error         | Updated `cmake_minimum_required` to >= 3.5            | Modern CMake dropped support for older versions          |
| 2    | Runtime: `Unable to open window`  | Verified and enforced proper OpenGL context flags     | macOS requires forward-compatible, core profile contexts |
| 3    | `glDebugOutputCallback` not found | Manually implemented debug callback                   | Ensured `GL_CONTEXT_FLAG_DEBUG_BIT` is checked           |
| 4    | `install_name_tool` warning       | Ignored / suppressed                                  | Mac-specific and didn’t affect runtime                   |
| 5    | No debug output                   | Enabled `GL_DEBUG_OUTPUT` and validated context flags | Still saw fallback warning if debug context not created  |
| 6    | Segmentation fault                | Used LLDB to trace undefined function pointer         | Callback was declared but never defined                  |

---

## Step-by-Step Bring-Up Log

### 1. Cloning and Building

```bash
git clone https://gitlab.com/gltf-viewer-tutorial/gltf-viewer.git
cd gltf-viewer
source .vscode/bash-init.sh
cmake_prepare
cmake_build
cmake_install
```

Initial CMake error:

```text
Compatibility with CMake < 3.5 has been removed from CMake.
```

**Fix:**
Update `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.22)
```

---

### 2. Runtime Crash: "Unable to Open Window"

```bash
./build/bin/gltf-viewer info
```

Output:

```
libc++abi: terminating due to uncaught exception of type std::runtime_error: Unable to open window.
```

**Root Cause:**
GLFW was unable to create an OpenGL context.

**Fix:**
Set required GLFW hints:

```cpp
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

---

### 3. Missing Symbol: `glDebugOutputCallback`

Source referenced this without defining it:

```cpp
glDebugMessageCallback(glDebugOutputCallback, nullptr);
```

**Fix:**
Implemented callback manually:

```cpp
void APIENTRY glDebugOutputCallback(GLenum source, GLenum type, GLuint id,
                                    GLenum severity, GLsizei length,
                                    const GLchar* message, const void* userParam)
{
    std::cerr << "[OpenGL Debug] " << message << std::endl;
}
```

Also registered the callback and validated context:

```cpp
glEnable(GL_DEBUG_OUTPUT);
glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

GLint flags;
glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glDebugMessageCallback(glDebugOutputCallback, nullptr);
}
```

---

### 4. Install-Time Warning: `install_name_tool`

```
error: install_name_tool: no LC_RPATH load command with path: /opt/homebrew/lib
```

**Fix:**
Benign on Apple Silicon systems. No runtime issues observed. Can suppress or ignore.

---

### 5. Final Verification

```bash
./build/bin/gltf-viewer info
```

Output:

```
width and height are: 600,800
[Debug Output] OpenGL context has no debug flag.
OpenGL Version 4.1
```

---

## Debugging a Segmentation Fault with LLDB: A Walkthrough

While bringing up the glTF Viewer project, I encountered a **segmentation fault** during program launch. It turned out to be a valuable opportunity to practice **LLDB**, Apple’s native debugger.

### Step 1: Identifying the Fault

```bash
./build/bin/gltf-viewer info
```

Output:

```
Segmentation fault: 11
```

This usually means null pointer dereference or an invalid jump.

### Step 2: Launching LLDB

```bash
lldb ./build/bin/gltf-viewer
run info
```

Debugger output:

```
* thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=1, address=0x0)
  * frame #0: 0x0000000000000000
```

### Step 3: Inspecting the Backtrace

```lldb
bt
```

```
frame #0: 0x0000000000000000
frame #1: gltf-viewer`initGLDebugOutput() + 40
frame #2: gltf-viewer`GLFWHandle::GLFWHandle(...) + ...
```

### Step 4: Root Cause — Missing Function Definition

Function call:

```cpp
glDebugMessageCallback(glDebugOutputCallback, nullptr);
```

Crash happened because `glDebugOutputCallback` was declared but not defined.

### Step 5: Fixing It

Implemented the missing callback:

```cpp
void APIENTRY glDebugOutputCallback(GLenum source, GLenum type, GLuint id,
                                    GLenum severity, GLsizei length,
                                    const GLchar* message, const void* userParam)
{
    std::cerr << "[OpenGL Debug] " << message << std::endl;
}
```

Recompiled and confirmed crash resolved.

---

## Lessons Learned

* Always verify OpenGL context flags on macOS.
* Debug output only works if the debug context is explicitly requested.
* `lldb` is invaluable for diagnosing segmentation faults.
* If your backtrace shows `0x0`, you're likely calling a null function pointer.

---

## Notes to Self

* Consider moving shaders to a dedicated `resources/` folder with live reload.
* Add `GLFW_OPENGL_DEBUG_CONTEXT` hint.
* Wrap debug message setup with a runtime flag to toggle verbosity.
* Validate Sponza and Helmet scenes once bring-up stabilizes.
