// === shader/lambert1.frag ===
#version 410 core
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightDir;

out vec4 FragColor;

void main() {
    vec3 n = normalize(Normal);
    vec3 l = normalize(-lightDir);
    float diff = max(dot(n, l), 0.0);
    vec3 Kd = vec3(1.0, 0.6, 0.3);
    FragColor = vec4(diff * Kd, 1.0);
}
