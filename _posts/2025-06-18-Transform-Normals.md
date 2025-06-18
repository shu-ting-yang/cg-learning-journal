---
layout: single
title: "Transforming Normals with Inverse Transpose"
permalink: /week-3-transform-normals/
math: true
---
* date: 2025-06-18
* categories: [graphics, opengl]
* tags: [graphics, shaders, opengl, glsl, normal, linear-algebra, lighting]

## Goal

To understand why normals in vertex shaders are transformed using the inverse transpose of the model matrix when applying lighting models like Lambertian reflectance.

## What Was Implemented

```glsl
Normal = mat3(transpose(inverse(model))) * aNormal;
````

This common line in GLSL shaders transforms the vertex normal using the inverse transpose of the model matrix. While initially counterintuitive, the reasoning lies in preserving surface orientation under general linear transformations.

---

## Shader Logic

### Why Not Just Use the Model Matrix?

Transforming vertex positions with a model matrix $M$ is straightforward:

```glsl
vec4 worldPos = model * vec4(aPos, 1.0);
```

However, transforming normals is not as simple. If the model matrix contains **non-uniform scaling or shearing**, applying `model * aNormal` may distort the normal direction, breaking the expected **perpendicular relationship** between the normal and the surface.

### Linear Algebra Identity

A key identity from linear algebra explains the fix:

$$
a \cdot (M b) = (M^T a) \cdot b
$$

This states that if a vector **b** is transformed by a matrix **M**, then to preserve the dot product, the other vector **a** must be transformed by the **transpose of M**.

Applying this to normals:

To preserve the dot product between the normal and the surface tangent after transformation:

$$
n' \cdot (Mv) = n \cdot v
$$

Using the identity above:

$$
(M^T n') \cdot v = n \cdot v
\quad \Rightarrow \quad M^T n' = n
\quad \Rightarrow \quad n' = (M^{-1})^T n
$$

Thus, the correct transformation for normals under general affine transformations is:

```glsl
Normal = mat3(transpose(inverse(model))) * aNormal;
```

---

## Key Learnings

### When Is the Inverse Transpose Necessary?

The inverse transpose is required when the model matrix contains **non-uniform scale**, **skew**, or **any non-orthogonal transformation**. These break the assumption that normals can simply follow vertex positions.

If the matrix is purely orthonormal (e.g., only rotation and translation), then:

$$
(M^{-1})^T = M
$$

In such cases, normals can be transformed directly using the model matrix without loss of correctness.

### Visual Consequences

Incorrect normal transformation results in lighting artifacts: surfaces may appear too dark, too bright, or inconsistent depending on view angle and light position. These effects are especially visible when using directional lights or dynamic shading models.
