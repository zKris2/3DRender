#version 330 core
out vec4 FragColor;

uniform sampler2D smapler;

in vec2 uv;

void main()
{
	FragColor = texture(smapler, uv);
}