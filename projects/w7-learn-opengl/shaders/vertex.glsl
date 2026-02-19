#version 330 core
layout (location = 0) in vec3 aPos; // position attribute
layout (location = 1) in vec3 aColor; // color attribute
out vec4 vertexColor; // output color to the fragment shader
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.x, 1.0f); // set the position of the vertex
    // gl_Position is a built-in variable that determines the position of the vertex in clip space
    // vec4(aPos, 1.0f) converts the vec3 position to a vec4 by adding a w component of 1.0

    vertexColor = vec4(0.5f, 0.5f, 0.5f, 1.0f); // set the color of the vertex
}