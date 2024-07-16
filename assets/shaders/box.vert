#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aNormal;

out vec2 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    Normal = aNormal;
    gl_Position  = projection * view * model * vec4(aPos, 1.0);

}  