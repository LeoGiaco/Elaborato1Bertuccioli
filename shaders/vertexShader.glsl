#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 color;

uniform mat4 projection;
uniform mat4 model;

out vec4 frag_col;

void main(){	

	gl_Position = projection * model * vec4(aPos.x,aPos.y,aPos.z,1.0);

	frag_col = color;
	
}

