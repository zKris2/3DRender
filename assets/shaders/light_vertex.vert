#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 perspective_matrix;

void main()
{
	gl_Position = perspective_matrix * camera_matrix * model_matrix  * vec4(aPos,1.0);
}