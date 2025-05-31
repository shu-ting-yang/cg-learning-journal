#version 410 core
layout(location = 0) in vec3 aPos;
uniform mat4 mvp;

void main(){
  //    upload both maxtrix(view+projection)to vertex shader
    gl_Position = mvp * vec4(aPos, 1.0);
}