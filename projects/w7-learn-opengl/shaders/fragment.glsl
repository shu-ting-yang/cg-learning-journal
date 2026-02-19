#version 330 core
out vec4 FragColor;
in vec4 vertexColor; // input color from the vertex shader
void main()
{
    // Set the output color to a constant value
    //FragColor = vec4(1.0, 1.0, 0.0, 1.0); // Red color
    FragColor = vertexColor; // Use the color passed from the vertex shadㄜr
