---
layout: single
title: "Transforming Normals with Inverse Transpose"
date: 2025-06-18
permalink: /week-3-transform-normals/
math: true
categories:
  - vfx-learning
tags:
  - graphics
  - shaders
  - opengl
  - glsl
  - normal
  - linear-algebra
  - lighting
---
* date: 2025-06-18
* categories: [graphics, opengl]
* tags: [graphics, shaders, opengl, glsl, normal, linear-algebra, lighting]


# Transforming Normals with the Inverse Transpose

When implementing [Lambertian shading]({{ site.baseurl }}/week-3-Lambertian-model/) in vertex shaders, a common but often overlooked detail emerges: **normals are not transformed using the model matrix directly**, but instead through the inverse transpose of that matrix.

This operation is frequently seen in the form:

```glsl
Normal = mat3(transpose(inverse(model))) * aNormal;
```

To justify this transformation, it is necessary to examine the relevant principles of linear algebra, particularly how dot products interact with linear mappings.


## Dot Product and the Role of Matrix Transpose

A key identity from linear algebra underpins the correct handling of normals under affine transformation:

$$
a \cdot (M b) = (M^T a) \cdot b
$$

This identity holds for any vectors $a$, $b$ and any matrix $M$, and follows directly from two well-established rules:

1. The dot product can be represented as a matrix product:

   $$
   a \cdot b = a^T b
   $$

2. The transpose of a matrix product reverses the order:

   $$
   (AB)^T = B^T A^T
   $$

Applying these yields the following derivation:

$$
a \cdot (M b) = a^T (M b) = (a^T M) b = (M^T a)^T b = (M^T a) \cdot b
$$

This equivalence provides the mathematical basis for transferring transformations from one vector to the other within a dot product expression.


## Application to Normal Transformation

Let $v$ denote a surface tangent vector and $n$ a normal vector such that $n \cdot v = 0$, representing perpendicularity in the local coordinate frame. When applying a transformation matrix $M$, the tangent vector is transformed to $M v$. In order to preserve the geometric constraint (i.e., that the transformed normal $n'$ remains perpendicular to $M v$), the following condition must hold:

$$
n' \cdot (M v) = n \cdot v
$$

By the identity established above:

$$
(M^T n') \cdot v = n \cdot v
$$

For this equation to be valid for all $v$, it must follow that:

$$
M^T n' = n \quad \Rightarrow \quad n' = (M^{-1})^T n
$$

This conclusion explains why normals must be transformed using the inverse transpose of the model matrix rather than the model matrix itself.


## Significance of the Inverse Transpose

This transformation becomes essential when the model matrix includes **non-uniform scaling**, **shear**, or other non-orthogonal components. Under such transformations, directly applying the model matrix to normals would distort their orientation and violate their perpendicularity with respect to transformed tangents, leading to incorrect lighting and shading.

In contrast, if the model matrix consists solely of orthonormal transformations (e.g., pure rotation and translation), then $(M^{-1})^T = M$, and the inverse transpose simplifies to the original matrix. However, relying on this simplification is only valid under those specific constraints.
