#version 330 core
out vec4 FragColor;

uniform sampler2D smapler;
uniform vec3 lightColor;

in vec2 uv;

void main()
{
    vec4 texColor = texture(smapler, uv); 
    vec3 resultColor = texColor.rgb * (lightColor * 0.1);
    FragColor = vec4(resultColor, texColor.a);
}