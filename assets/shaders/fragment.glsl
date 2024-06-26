#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

//uniform sampler2D smapler;
uniform vec3 model_color; 
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

//in vec2 uv;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    // 环境光
    //vec3 ambient = light_color * material.ambient;
    vec3 ambient  = light.ambient * material.ambient;

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = light_color * (diff * material.diffuse);
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);

    // 镜面光
    vec3 viewDir = normalize(view_pos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light_color * (spec * material.specular);  
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);


}