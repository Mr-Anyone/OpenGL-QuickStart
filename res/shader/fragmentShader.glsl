#version 330 core 
out vec4 fragColor;

in vec2 textCor;
uniform sampler2D texture_diffuse0;

void main(){
    fragColor = texture(texture_diffuse0, textCor);
}