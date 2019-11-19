#version 450 core
layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 model = mat4(1.0);

out vec4 vpos;

void main()
{
    vpos = view * model * vec4(position, 1.0f);
    gl_Position = vpos;
}