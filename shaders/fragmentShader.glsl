#version 420 core

uniform vec2 resolution;
// Interpolated values from the vertex shaders
in vec4 frag_col;

out vec4 color;

void main(){
	vec2 ndc = vec2 ((gl_FragCoord.x / resolution.x - 0.5) * 2, (gl_FragCoord.y / resolution.y - 0.5) * 2);

	color = frag_col;
}