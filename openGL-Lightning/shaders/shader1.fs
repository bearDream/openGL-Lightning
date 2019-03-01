#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lighterColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 FragPos;

void main(){
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lighterColor;
    
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lighterColor;
    
    vec3 res = (ambient + diffuse) * objectColor;
    FragColor = vec4(res, 1.0f);
}
