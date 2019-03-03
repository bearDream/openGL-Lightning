#version 330 core
out vec4 FragColor;

//uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

// material
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess; // 反光度
};

struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction; // 新增光的方向
    vec3 position; // 定向光不需要光源位置
    float cutoff; // 手电筒光照的切向角
    // 计算点光源光线衰减的 三个常数值
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

void main(){
    
    vec3 lightDir = normalize(light.position - FragPos);
    
    float theta = dot(lightDir, normalize(-light.direction));
    if (theta > light.cutoff) {
        // 设置环境光照
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        
        // 计算漫反射向量
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(light.position - FragPos); // 片段指向光源的向量
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
        
        // 设置镜面反射
        vec3 viewDir = normalize(viewPos-FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
        
        // 计算光源衰减
        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        
        vec3 res = ambient + diffuse + specular;
        FragColor = vec4(res, 1.0f);
    }else{
        FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0f);
    }
}
