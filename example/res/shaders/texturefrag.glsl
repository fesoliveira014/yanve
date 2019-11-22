#version 450 core

out vec4 fragColor;

in vec4 vColor;
in vec2 texCoords;

uniform sampler2D texture1;

void main()
{
    fragColor = vec4(texture2D(texture1, texCoords).rgb, 1.0);
}
