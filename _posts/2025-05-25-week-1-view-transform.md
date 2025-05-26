---
title: "Week 1: Understanding View Transformations"
date: 2025-05-25
categories: [graphics, opengl]
tags: [OpenGL, matrix, camera, glm]
---

This week, I started my first CG engineering project: rendering a triangle using OpenGL, and learning how different transformation matrices affect the final image.

## Goals
- Set up a working OpenGL pipeline using C++/GLFW/GLAD
- Understand object → world → view → projection transformation
- Add keyboard and mouse control to simulate a first-person camera

## What I Built
I created a simple OpenGL app that renders a triangle, and wired up camera controls using GLM to support:

- WASD to move
- Mouse to look around
- Real-time view matrix updates

## Matrix Pipeline

Here’s the transformation applied to each vertex:

gl_Position = Projection * View * Model * vec4(vertex, 1.0);

I'll cover:
- What each matrix does
- What happens when you change the camera position vs object transform

## Screenshots

_coming soon_

## Key Learnings
- OpenGL’s right-handed coordinate system
- How `glm::lookAt()` works
- GLAD vs GLFW responsibilities
- Why matrices must be multiplied in reverse order

## Q&A
- `glm::lookAt()`: Why It's Used
  - this function conjtructs a view matrix that transforms world-space coordinates into view space (a.k.a. camera space), based on
  ```
  glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); 
  ```
  This simulates a camera at position cameraPos, looking in the direction of cameraFront, with cameraUp defining the "tilt" (which way is "up" for the camera).
  ``` 
  We use this because you don't move the camera in OpenGL — you transform the world in the opposite direction.  
  ``` 




## Next Week
I’ll explore shaders more deeply using [The Book of Shaders](https://thebookofshaders.com/), and build an animated GLSL fragment shader viewer.

---

Stay tuned as I go deeper into graphics!
