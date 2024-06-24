#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 perspective_matrix;



out vec2 uv;

void main()
{
	gl_Position = perspective_matrix * camera_matrix * model_matrix  * vec4(aPos,1.0);
	//gl_Position =   model_matrix  * vec4(aPos,1.0);
	uv = aUV;
}