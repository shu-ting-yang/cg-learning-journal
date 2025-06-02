# Shader Playground: Procedural Grid with Live Controls

* date: 2025-06-02
* categories: \[graphics, opengl]
* tags: \[graphics, shaders, opengl, glsl, procedural, playground]

## Goal

Build a minimal fragment-shader playground that:
- Renders a full-screen quad
- Loads external GLSL fragment shaders
- Sends real-time uniform inputs (time, resolution, and key-driven parameters)
- Demonstrates a working procedural checker pattern with keyboard control

## What I Built

The core viewer:
- C++ with GLFW + GLAD (OpenGL 4.1 core profile)
- A full-screen triangle strip covering the entire viewport
- A minimal vertex shader that passes normalized UVs to the fragment shader
- Uniforms:
  - `u_time` – real-time float from `glfwGetTime()`
  - `u_resolution` – current framebuffer size
  - `u_grid_size` – controllable via keys `1–9` to change checker size


## Controls

| Key | Function |
|-----|----------|
| `1–9` | Sets grid size for the checker shader |
| `ESC` | Exits the application |

## Shader Logic: Checker Pattern

```glsl
vec2 grid = uv * float(u_grid_size);
float gx = step(1.0, mod(grid.x, 2.0));
float gy = step(1.0, mod(grid.y, 2.0));
float checker = abs(gx - gy);
FragColor = vec4(vec3(checker), 1.0);
```
* uv comes from remapped vertex positions (`[-1, 1]` → `[0, 1]`)
* We tile UV space into a user-defined number of cells
* mod and step help generate sharp borders
* `abs(gx - gy)` flips color every other square


## Screenshots


## Key Learnings

### Understanding GLFW, GLAD

  We initialize GLFW, which gives us control over window creation and OpenGL context management.
  → `glfwInit()` sets up the internal state for future calls.
  We create a window and make its OpenGL context current, using `glfwCreateWindow` and `glfwMakeContextCurrent`.
  This binds the OpenGL context to the current thread — a required step before calling any OpenGL function.
  > `glfwMakeContextCurrent` What does it do exactly?
  > 
  > https://computergraphics.stackexchange.com/questions/4562/what-does-makecontextcurrent-do-exactly
  > 
  > OpenGL is a state machine. An OpenGL context holds that state. The state contains information such as which textures are bound to which texture units, which attachments the current FBO has, and things like that.
  > When you set the current context, you are switching all the state from the old context to the new context. Here's an example:
  > ```
  > glfwMakeContextCurrent(window1);
  > glEnable(GL_TEXTURE_2D);
  > glBindTexture(GL_TEXTURE_2D, texture1);
  > glfwMakeContextCurrent(window2);
  > ```
  > What this does is makes the context of `window1` current. It then enables the 2D texture unit and binds texture1 to it.
  >
  > Next it makes the context of `window2` current. If you were to check the enabled state of `GL_TEXTURE_2D` it would be `false` (unless you had previously enabled it on the `window2` context). Likewise, if you check which texture is bound to the 2D texture unit, you would likely find no texture was bound, or some texture other than texture1 was bound.
  >
  > Basically all drawing happens in the current context. If you make a different context current, then all drawing will now happen in that context.
  >
  > It is common to have a single context per thread in a multi-threaded OpenGL application. That way each thread is drawing into its own context and doesn't mess up the state of contexts on other threads.
  > 

  We load OpenGL function pointers using GLAD, via `gladLoadGLLoader`.
  GLAD queries the OpenGL driver using the current context, so it must be loaded after `glfwMakeContextCurrent`.


### How UV Coordinates Are Generated Manually When There's No Mesh

When building a fullscreen shader playground, we manually define our geometry as a fullscreen quad — usually just two triangles covering the entire screen. Unlike a typical mesh from Blender or glTF, this kind of geometry doesn't come with any UV attributes. That means we need to calculate UVs ourselves.

In a real mesh, UV coordinates are usually stored as part of the vertex data (just like positions and normals) and passed from the vertex buffer into the vertex shader using an input variable like `in vec2 aUV;`. The vertex shader would then pass this value straight to the fragment shader using an `out vec2 uv;`, and the rasterizer would interpolate it across the triangle.

But when we manually define a quad using positions like:

```cpp
-1.0f,  1.0f,
-1.0f, -1.0f,
 1.0f, -1.0f,
...
```

we are not passing UVs. We're directly passing coordinates that are already in the `[-1, 1]` range — which is the **NDC space** (Normalized Device Coordinates). However, in the vertex shader, these values are first treated as **clip space** when assigned to `gl_Position`:

```glsl
gl_Position = vec4(aPos, 0.0, 1.0);
```

This `gl_Position` is in **clip space**. Once this reaches the rasterizer, the GPU performs a perspective divide: it divides the `x`, `y`, and `z` components by the `w` component (which in this case is 1.0). That transforms the clip space position into **NDC space**.

So even though we pass `aPos` into the shader, and assign it directly to `gl_Position`, what the GPU really uses for rasterization, occlusion(depth test), and interpolation for other attributes like `uv` is `gl_Position.xyz / gl_Position.w`, which ends up being the same as `aPos` since `w = 1.0`. 

> What happens if we have a "mesh"? Do we still compute UVs manually?
> 
> When you render a real mesh (e.g., from Blender, glTF, or OBJ):
> The mesh comes with UV coordinates stored as vertex attributes (like normals, tangents, etc.).
> These UVs are supplied in the vertex buffer alongside positions.
> The vertex shader then simply passes them through to the fragment shader, like this:
> ```glsl
> layout(location = 0) in vec3 aPos;
> layout(location = 1) in vec2 aUV;
> 
> out vec2 uv;
> 
> void main() {
>     gl_Position = projection * view * model * vec4(aPos, 1.0);
>     uv = aUV; // pass interpolated UV directly
> }
> ```
> So: you don’t compute UVs manually in this case — they're part of the mesh data. Your job is to transform positions and pass UVs through.


To convert those NDC values (`[-1, 1]`) into a screen-space UV range of `[0, 1]`, we perform a remap inside the vertex shader:

```glsl
uv = (aPos + 1.0) * 0.5;
```

This shifts the range from `[-1, 1]` → `[0, 2]`, then scales it to `[0, 1]`.

The result:

* The bottom-left corner of the screen gets UV (0.0, 0.0)
* The top-right corner of the screen gets UV (1.0, 1.0)

These UV coordinates are then automatically interpolated across the pixels of the triangles and arrive in the fragment shader as a varying input.

To summarize:

1. `aPos` is manually defined in your vertex buffer as values in `[-1, 1]` (screen-space quad)
2. You assign `gl_Position = vec4(aPos, 0.0, 1.0)` → this is in clip space
3. GPU performs `gl_Position.xyz / gl_Position.w` → becomes NDC space (effectively the same as `aPos`)
4. You manually calculate and pass `uv = (aPos + 1.0) * 0.5` → gives screen-space coordinates in `[0, 1]`
5. Fragment shader uses this `uv` for drawing shapes, grids, or patterns

### How Are Values Passed to the Fragment Shader?

When writing GLSL shaders, understanding how **data flows into** the fragment shader is essential. In this shader playground project, we pass variables like time, resolution, and grid size from the CPU program to the GPU. Let’s break down how and why this works.

#### What Is a `uniform`?

A `uniform` is a **read-only global variable** in GLSL that stays **constant during a single draw call**. It is set from the CPU via OpenGL commands like:

```cpp
glUniform1i(glGetUniformLocation(shaderProgram, "u_grid_size"), gridSize);
```

In our example, we let the user press keys `1–9` to change the number of grid tiles. This value (`gridSize`) may change **between** frames, but once we start a draw call, it must **stay the same across all fragments** in that frame. That’s exactly what a `uniform` is designed for.

```glsl
uniform int u_grid_size;
```


#### Why Not Use `in`?

The `in` keyword in the fragment shader means the value is **interpolated** from the **vertex shader output** across the triangle’s surface. It’s how varying data flows between the shader stages.

```glsl
// vertex.glsl
out vec2 uv;

...

// fragment.glsl
in vec2 uv;
```

This is useful for values that **depend on geometry**, such as position or UV per vertex.


## Next Week
I will explore building a basic viewer that loads a sphere model and supports switching between Lambert and Phong shaders, with a GUI to tweak lighting and material parameters.

---

Stay tuned as I go deeper into graphics!
