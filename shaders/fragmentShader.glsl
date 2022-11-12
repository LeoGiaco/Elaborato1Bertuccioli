#version 420 core

uniform vec2 resolution;

in vec4 frag_col;

out vec4 color;

void main(){
	color = frag_col;
}