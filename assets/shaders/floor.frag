#version 460 core
out vec4 FragColor;

uniform sampler2D floor_sampler;

in vec2 uv;

void main()
{
	FragColor = texture(floor_sampler, uv);
}