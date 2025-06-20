# Week 1: Understanding View Transformations


* date: 2025-05-25
* categories: \[graphics, opengl]
* tags: \[OpenGL, matrix, camera, glm]

This week, I started my first CG engineering project: rendering a triangle using OpenGL, and learning how different transformation matrices affect the final image.

## Goals

* Set up a working OpenGL pipeline using C++/GLFW/GLAD
* Understand object → world → view → projection transformation
* Add keyboard and mouse control to simulate a first-person camera

## What I Built

I created a simple OpenGL app that renders a triangle, and wired up camera controls using GLM to support:

* WASD to move
* Mouse to look around
* Real-time view matrix updates

[Code](https://github.com/shu-ting-yang/cg-learning-journal/tree/master/projects/w1-view-transform)


## Recording

<figure class="video-container">
  <video controls autoplay muted loop style="width: 100%;">
    <source src="{{ '/assets/videos/w1-demo.mp4' | relative_url }}" type="video/mp4">
    Your browser does not support the video tag.
  </video>
</figure>


## Key Learnings

* GLAD vs GLFW responsibilities

  * *GLFW* creates a window, handles keyboard/mouse input, and sets up the OpenGL context.
  * *GLAD* loads the OpenGL function pointers—without it, OpenGL calls like `glDrawArrays()` won't work.

* Vertex Shader vs Fragment Shader

  * The vertex shader transforms each vertex's position—e.g., using the MVP matrix.
  * The fragment shader colors the pixels that fill the shapes made by those vertices.

* Attribute and location in shader code

  * An attribute is vertex data that we send from the CPU (C++ program) to the GPU (vertex shader). Each vertex can have multiple attributes like:

    * Position (x, y, z)
    * Color (r, g, b)
    * Texture coordinates (u, v)
    * Normal vectors (x, y, z)

  * The location number is like an "address" or "slot number" where we put each type of data.

  * `layout(location = 0)` in the shaders matches `glVertexAttribPointer(0, ...)` in C++ (CPU side); they must align.

  * Example: multiple attributes

    ```glsl
    layout(location = 0) in vec3 aPos;     // Position
    layout(location = 1) in vec3 aColor;   // Color
    layout(location = 2) in vec2 aTexCoord; // Texture coordinates
    ```

    ```cpp
    float vertices[] = {
        // positions      // colors        // texture coords
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f
    };

    // Set up location 0 (position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up location 1 (color)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set up location 2 (texture coordinates)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    ```

* MVP Matrix Pipeline

  ```glsl
  gl_Position = Projection * View * Model * vec4(vertex, 1.0);
  ```

  * Model matrix: local → world transform (we used identity matrix this week)
  * View matrix: world → camera transform; we used `glm::lookAt`
  * Projection matrix: maps 3D to 2D space, with perspective effects

* Homogeneous Coordinates

  * A `vec4` with `.w = 1.0` is a point—it will be affected by translation.
  * A `vec4` with `.w = 0.0` is a vector—it will not be affected by translation.

* Perspective Projection

  At first, I thought `w` in homogeneous coordinates was only used to distinguish between points (`w = 1`) and vectors (`w = 0`). But in 3D graphics and perspective projection, it serves a deeper purpose.

  The projection matrix is designed so that after transformation, `gl_Position.w` encodes the depth relative to the camera. The `z` component is also transformed in a compatible way, so `z / w` becomes meaningful—it represents how deep a point is from the camera’s perspective.

  After the vertex shader, the GPU performs **perspective division**: `gl_Position.xyz /= gl_Position.w`. This gives a normalized depth value, written into the **z-buffer**, used for depth testing and occlusion.

  The matrix is crafted (based on FOV, aspect ratio, near/far planes) so that `w` carries useful depth information, and `z` is scaled accordingly. Post-division, the projected position has depth correctly encoded.

  ```glsl
  gl_Position = vec4(x, y, z, w) → becomes vec3(x/w, y/w, z/w)
  ```

  In the projection matrix:

  * It’s designed so that **z ≈ w** on the far plane → `z/w ≈ 1`
  * `z ≪ w` on the near plane → `z/w ≈ 0`

  So:

  * Near objects → larger `w` → smaller `z/w` → appear closer
  * Far objects → smaller `w` → larger `z/w` → appear farther

* Coordinate System & View Direction

  * OpenGL uses a right-handed system; +Z points out of the screen (toward the viewer)
  * Forward is −Z
  * That’s why `cameraFront = vec3(0, 0, -1)` and `glm::lookAt(cameraPos, cameraPos + cameraFront, ...)` works

* How `glm::lookAt()` Works

  * First parameter: camera location (world space)
  * Second parameter: target location (world space)
  * Third parameter: camera's up direction; usually +Y
  * It computes the inverse transform of the camera’s pose. We don’t move the camera—we move the world in the opposite direction.

## Next Week

I’ll explore shaders more deeply using [The Book of Shaders](https://thebookofshaders.com/), and build an animated GLSL fragment shader viewer.

---

Stay tuned as I go deeper into graphics!
