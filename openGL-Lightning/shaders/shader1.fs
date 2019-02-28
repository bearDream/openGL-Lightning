#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lighterColor;

void main(){
    FragColor = vec4(objectColor * lighterColor, 1.0f);
}
