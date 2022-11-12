#version 420 core

uniform vec2 resolution;
uniform int hit;

in vec4 frag_col;

out vec4 color;

void main(){
	color = frag_col;
    if (hit == 1) {
        color = mix(vec4(1.0, 0.0, 0.0, 1.0), color, 0.6);
    }
}