#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 model_position;
out vec3 model_normal;
out vec2 model_texcoord;

out vec2 texture_cood;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0);
	model_position = vec3(model * vec4(aPos, 1.0));
	model_normal = aNormal;
	model_texcoord = aTexCoords;
}