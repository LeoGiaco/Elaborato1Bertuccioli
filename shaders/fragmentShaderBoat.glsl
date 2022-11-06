
uniform float time;	// remove
uniform vec2 resolution;

int stripes = 20;	// stripes as uniform
float stripe_size = 0.2;
float angle = time / 2.0;	// angle as uniform
vec2 offset = vec2(0.5, 0.5);	// offset as uniform

#define BETWEEN(a, x, b) ((a) < (x) && (x) < (b))

float ratio = resolution.x / resolution.y;

void main( void ) {

	vec2 position = ( gl_FragCoord.xy / resolution.xy - offset );
	vec4 color = vec4(0.35, 0.0, 0.0, 1.0);


	// float modv = mod((position.x + 1.0 / 2.0) * cos(angle) + (position.y + 1.0) * sin(angle), 1.0 / float(stripes));
	float modv = mod(position.x * cos(angle) * ratio + position.y * sin(angle), 1.0 / float(stripes));

	if  ( BETWEEN((stripe_size / 2.0) / float(stripes), modv, (1.0 - stripe_size / 2.0) / float(stripes))) {
		color = vec4(0.4, 0.15, 0.0, 1.0);
	}

	if ( distance(vec2(position.x * ratio, position.y), vec2(0.0, 0.0)) < 0.02 ) {	// shows rotation center
		color = vec4(1.0, 1.0, 1.0, 1.0);
	}

	gl_FragColor = color;
}
