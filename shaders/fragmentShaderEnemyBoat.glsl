#version 420 core

uniform vec2 resolution;
uniform int stripes;// 20
uniform float stripe_size;// 0.2
uniform float angle;
uniform vec2 off;

uniform vec4 base_color;
uniform vec4 stripe_color;

uniform int hit;

out vec4 col;

#define BETWEEN(a,x,b)((a)<(x)&&(x)<(b))

float ratio = resolution.x / resolution.y;

void main(void){
	vec2 position = (gl_FragCoord.xy / resolution.xy - off);
	vec4 color = stripe_color;
	
	float modv = mod(position.x * cos(angle) * ratio + position.y * sin(angle), 1.0 / float(stripes));
	
	if (BETWEEN((stripe_size / 2.0) / float(stripes), modv, (1.0 - stripe_size / 2.0) / float(stripes))){
		color = base_color;
	}
	
	col = color;

    if (hit == 1) {
        col = mix(vec4(1.0, 0.0, 0.0, 1.0), col, 0.6);
    }
}
