#version 330 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

// material
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess; // 反光度
};

// material
struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position; // 光源位置
};

uniform Material material;
uniform Light light;

void main(){
    // 设置环境光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // 计算漫反射向量
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos); // 片段指向光源的向量
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // 设置镜面反射
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    // 放射光
    vec3 emision = texture(material.emission, TexCoords).rgb;
    vec3 res = ambient + diffuse + specular + emision;
    FragColor = vec4(res, 1.0f);
}
