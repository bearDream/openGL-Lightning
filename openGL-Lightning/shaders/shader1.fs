#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float alpha;

void main(){
    //    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    //    FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), 0.2);
    // 将图案的右边对应到纹理的左边(即将纹理向左转)
    FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), 0.2);
}
