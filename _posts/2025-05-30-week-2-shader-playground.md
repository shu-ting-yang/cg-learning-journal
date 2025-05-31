# Week 1: Understanding shader

* date: 2025-05-30
* categories: \[graphics, opengl]
* tags: \[OpenGL, shader]

This week, I started my capstone project for my shader learning. Build a full screen quad renderer for fragment shaders. 

## Goals

* Build a minimal viewer that renders fragment shaders on a fullscreen quad, driven purely by GLSL math (no textures).


## What I Built


## Recording


## Key Learnings

- On GLFW, GLAD
We initialize GLFW, which gives us control over window creation and OpenGL context management.
→ `glfwInit()` sets up the internal state for future calls.
We create a window and make its OpenGL context current, using `glfwCreateWindow` and `glfwMakeContextCurrent`.
This binds the OpenGL context to the current thread — a required step before calling any OpenGL function.
`glfwMakeContextCurrent` What does it do exactly?
https://computergraphics.stackexchange.com/questions/4562/what-does-makecontextcurrent-do-exactly
```
OpenGL is a state machine. An OpenGL context holds that state. The state contains information such as which textures are bound to which texture units, which attachments the current FBO has, and things like that.

When you set the current context, you are switching all the state from the old context to the new context. Here's an example:

glfwMakeContextCurrent(window1);
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texture1);

glfwMakeContextCurrent(window2);
What this does is makes the context of window1 current. It then enables the 2D texture unit and binds texture1 to it.

Next it makes the context of window2 current. If you were to check the enabled state of GL_TEXTURE_2D it would be false (unless you had previously enabled it on the window2 context). Likewise, if you check which texture is bound to the 2D texture unit, you would likely find no texture was bound, or some texture other than texture1 was bound.

Basically all drawing happens in the current context. If you make a different context current, then all drawing will now happen in that context.

It is common to have a single context per thread in a multi-threaded OpenGL application. That way each thread is drawing into its own context and doesn't mess up the state of contexts on other threads.
```

We load OpenGL function pointers using GLAD, via `gladLoadGLLoader`.
GLAD queries the OpenGL driver using the current context, so it must be loaded after `glfwMakeContextCurrent`.

## Next Week

---

Stay tuned as I go deeper into graphics!
