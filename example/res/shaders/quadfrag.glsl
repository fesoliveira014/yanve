#version 450 core
out vec4 fragColor;

in vec2 texCoords;

layout (binding = 0) uniform sampler2D screenTexture;

void main()
{
    fragColor = vec4(texture(screenTexture, texCoords).xyz, 1.0);
}