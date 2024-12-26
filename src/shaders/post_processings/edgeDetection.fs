#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
    vec2 tex_offset = 1.0 / textureSize(screenTexture, 0); // gets size of single texel
    vec3 result = vec3(0.0);

    // Edge detection kernel
    float kernel[9] = float[](
        1.0,  1.0,  1.0,
        1.0, -8.0,  1.0,
        1.0,  1.0,  1.0
    );

    vec2 offsets[9] = vec2[](
        vec2(-tex_offset.x,  tex_offset.y), // top-left
        vec2( 0.0,    tex_offset.y), // top-center
        vec2( tex_offset.x,  tex_offset.y), // top-right
        vec2(-tex_offset.x,  0.0),    // center-left
        vec2( 0.0,    0.0),    // center-center
        vec2( tex_offset.x,  0.0),    // center-right
        vec2(-tex_offset.x, -tex_offset.y), // bottom-left
        vec2( 0.0,   -tex_offset.y), // bottom-center
        vec2( tex_offset.x, -tex_offset.y)  // bottom-right
    );

    for(int i = 0; i < 9; i++) {
        result += texture(screenTexture, TexCoords + offsets[i]).rgb * kernel[i];
    }

    FragColor = vec4(result, 1.0);
}