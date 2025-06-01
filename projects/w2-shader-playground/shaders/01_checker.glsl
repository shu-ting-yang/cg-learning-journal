#version 410 core
uniform float u_time;
uniform vec2 u_resolution;
in vec2 uv;
uniform int u_grid_size;

out vec4 FragColor;

void main() {
    vec2 grid = uv*float(u_grid_size);//0~grid_size
    vec4 cc = vec4(0.0,0.0,0.0,1.0);
    if(step(1.000, mod(grid.x, 2.0))<0.5) { //<1:0, >1:1
        if(step(1.000, mod(grid.y, 2.0))<0.5){
            //flip color
            cc.rgb = vec3(1.0);
        }else{
            cc.rgb = vec3(0.0);
        }        
    } else {
        if(step(1.000, mod(grid.y, 2.0))<0.5){
            //flip color
            cc.rgb = vec3(0.0);
        }else{
            cc.rgb = vec3(1.0);
        }      
    }
    
    //gl_FragColor = cc;
    FragColor = cc;
}