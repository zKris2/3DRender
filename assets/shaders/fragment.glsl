#version 330 core
out vec4 FragColor;

//uniform sampler2D smapler;
uniform vec3 model_color; 
uniform vec3 light_color;
uniform vec3 light_pos;

//in vec2 uv;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    //FragColor = texture(smapler, uv);
    //FragColor = vec4(0.1f,0.2f,0.0f,1.0f);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_color;
//    vec3 result = ambient * model_color;
//    FragColor = vec4(result, 1.0);

    //计算光源和片段位置之间的方向向量,光的方向向量是光源位置向量与片段位置向量之间的向量差,
    //我们同样希望确保所有相关向量最后都转换为单位向量，所以我们把法线和最终的方向向量都进行标准化
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);
    //我们对norm和lightDir向量进行点乘，计算光源对当前片段实际的漫反射影响。
    //结果值再乘以光的颜色，得到漫反射分量。两个向量之间的角度越大，漫反射分量就会越小
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;
    vec3 result = (ambient + diffuse) * model_color;
    FragColor = vec4(result, 1.0);
}