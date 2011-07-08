uniform sampler2D tex;

/*
* Name: Convolution Matrix
* Description: Converts an image to a Binary Picture. The threshold is
* adjustable.
* Default: 1.0
*/
/*uniform*/ float threshold = 1.0;

void main() {

	vec3 color = texture2D(tex,gl_TexCoord[0].xy).rgb;
	if( threshold < sqrt(dot(color,color))){
		gl_FragColor = vec4(1.0,1.0,1.0,1.0);
	} else {
		gl_FragColor = vec4(0.0,0.0,0.0,1.0);
	}
}
