#version 460 core
out vec4 FragColor;

in vec3 model_position;
in vec3 model_normal;
in vec2 model_texcoord;

uniform sampler2D texture_sampler;
uniform vec3 light_color;
uniform vec3 light_position; 
uniform vec3 view_position; 

void main()
{
    
	vec4 model_color = texture (texture_sampler,model_texcoord); //model color
    vec3 norm = normalize(model_normal);//model normal

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * light_color;

	//diffuse
    vec3 lightDir = normalize(light_position - model_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(view_position - model_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_color;

	vec4 result = vec4(ambient + diffuse + specular ,1.0f) * model_color;
    FragColor = result;
}
