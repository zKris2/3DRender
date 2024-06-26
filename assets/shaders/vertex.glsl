#version 460 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aUV;
layout (location = 1) in vec3 aNormal;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 perspective_matrix;

//out vec2 uv;
out vec3 Normal;
out vec3 FragPos;  

void main()
{
	gl_Position = perspective_matrix * camera_matrix * model_matrix  * vec4(aPos,1.0);
	//uv = aUV;
	Normal = aNormal;
	FragPos = vec3(model_matrix * vec4(aPos, 1.0));
}