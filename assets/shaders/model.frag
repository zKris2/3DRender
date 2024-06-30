#version 460 core
out vec4 FragColor;

in vec3 model_position;
in vec3 model_normal;
in vec2 model_texcoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 light_color;
uniform vec3 light_position; 
uniform vec3 view_position; 

void main()
{
    vec3 norm = normalize(model_normal);//model normal

    //ambient
    vec3 ambient = light_color * vec3(texture(texture_diffuse1, model_texcoord));

	//diffuse
    vec3 lightDir = normalize(light_position - model_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light_color * diff * vec3(texture(texture_diffuse1, model_texcoord));

    //specular
    vec3 viewDir = normalize(view_position - model_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light_color * spec * vec3(texture(texture_specular1, model_texcoord));

	vec4 result = vec4(ambient + diffuse + specular ,1.0f);
    FragColor = result;
}