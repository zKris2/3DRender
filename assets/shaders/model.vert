#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in ivec4 aBoneIds; 
layout(location = 6) in vec4 aWeights;

out vec3 model_position;
out vec3 model_normal;
out vec2 model_texcoord;

out vec2 texture_cood;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{

    gl_Position =  projection * view * model * vec4(aPos,1.0f);
	texture_cood = aTexCoords;

    //For Light
	model_position = vec3(model * vec4(aPos, 1.0));
	model_normal =mat3(transpose(inverse(model))) * aNormal;
	model_texcoord = aTexCoords;
}