#version 460 core

in vec2 vUV;
out vec4 fragColor;

uniform sampler2D uTex;

void main(){
	fragColor = texture(uTex, vUV);
}