#version 410 core
uniform float u_time;
uniform vec2 u_resolution;
in vec2 uv;

out vec4 FragColor;

void main() {

    if(uv.x > 0.5){
     FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    } else if (uv.x < cos(u_resolution.y)){
     FragColor = vec4(clamp(sin(u_time),0.3, 0.6), 0.5, 0.2, 1.0);
    } else {
     FragColor = vec4(sin(u_time), 0.5, 0.2, 1.0);
    }
}