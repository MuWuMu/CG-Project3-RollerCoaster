#version 460 core
out vec4 FragColor;

in vec3 WorldPos;

uniform samplerCube skybox;
uniform vec3 viewPos;

void main() {
    vec3 I = normalize(WorldPos - viewPos);
    vec3 N = normalize(WorldPos);
    N.y = -N.y; // reverse normal y
    vec3 R = reflect(I, N);
    vec3 reflectColor = texture(skybox, R).rgb;
    FragColor = vec4(reflectColor, 1.0);
}