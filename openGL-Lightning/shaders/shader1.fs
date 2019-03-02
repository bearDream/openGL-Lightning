#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lighterColor;
//uniform vec3 lightPos;
//uniform vec3 viewPos;

in vec3 normal;
in vec3 FragPos;
in vec3 lightPos; //since we need the light position in view space we calculate this in the vertex

void main(){
    // 设置环境光照
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lighterColor;
    
    // 计算漫反射向量
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos); // 片段指向光源的向量
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lighterColor;
    
    // 设置镜面反射
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lighterColor;
    
    vec3 res = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(res, 1.0f);
}
