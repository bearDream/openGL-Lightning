#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 LightPosition; // LightPosition current is world space.

out vec3 normal;
out vec3 FragPos;
out vec3 lightPos; // turn lightPos in World Space into view Space

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(view * model * vec4(aPos, 1.0f));
    // 将model变为法线矩阵。防止当进行不等比缩放时对光照进行破坏
    // Tips. 为了效率着想，最好用CPU计算法线矩阵，避免使用着色器计算
    normal = mat3(transpose(inverse(view * model))) * aNormal;
    lightPos = vec3(view * vec4(LightPosition, 1.0f));
}
