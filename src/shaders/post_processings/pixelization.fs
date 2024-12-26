#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float pixelSize;
uniform vec2 screenSize;

void main() {
    vec2 coords = TexCoords * screenSize;
    vec2 pixelatedCoords = floor(coords / pixelSize) * pixelSize;
    vec2 uv = pixelatedCoords / screenSize;
    FragColor = texture(screenTexture, uv);
}