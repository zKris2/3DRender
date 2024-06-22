#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 camera_matrix;
uniform mat4 ortho_metrix;

out vec2 uv;

void main()
{
	vec4 position = vec4(aPos,1.0);
	position =  ortho_metrix * camera_matrix  * position;
	gl_Position = position;
	uv = aUV;
}