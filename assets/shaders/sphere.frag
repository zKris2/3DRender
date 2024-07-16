#version 460 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
struct PointLight  {
    vec3 color;
    vec3 position;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear; 
    float quadratic;
};

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;

uniform sampler2D sphere_sampler;
uniform Material material;
uniform PointLight point_light;
uniform vec3 view_position;


void main()
{
    //FragColor = texture(sphere_sampler,TexCoords);
    vec3 norm = normalize(Normal);//model normal

    //ambient
    vec3 ambient = point_light.color * point_light.ambient * material.ambient;

	//diffuse
    vec3 lightDir = normalize(point_light.position - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = point_light.color * point_light.diffuse * diff * material.diffuse;

    //specular
    vec3 viewDir = normalize(view_position - Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = point_light.color * point_light.specular * spec * material.specular;

    //  attenuation
    float distance = length(point_light.position - Position);
    float attenuation = 1.0 / (point_light.constant + point_light.linear * distance +point_light.quadratic * distance * distance);

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

	vec4 result = vec4(ambient + diffuse + specular ,1.0f);
    FragColor = result;
}