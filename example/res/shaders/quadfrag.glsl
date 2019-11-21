#version 450 core
out vec4 fragColor;

in vec2 texCoords;

layout (binding = 0) uniform sampler2D screenTexture;

void main()
{
    vec3 color = texture(screenTexture, texCoords).rgb;
    fragColor = vec4(color, 1.0);
}