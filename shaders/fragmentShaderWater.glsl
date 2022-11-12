#version 420 core

uniform vec2 resolution;

in vec4 frag_col;

out vec4 color;

void main(){
	vec2 norm = vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y);

	color = frag_col;
	color.a = clamp(1.0 - norm.y, 0.2, 0.6);
}