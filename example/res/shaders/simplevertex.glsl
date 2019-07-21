#version 450 core
layout (location = 0) in vec3 pos;

layout (location = 0) uniform mat4 model;

void main()
{
    gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);
}