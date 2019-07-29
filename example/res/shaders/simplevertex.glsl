#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

layout (location = 0) uniform mat4 model;

out vec4 vColor;

void main()
{
    gl_Position = model * vec4(position.x, position.y, position.z, 1.0);
    vColor = color;
}