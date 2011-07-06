

/**
 * ShaderName: ConvolveMat3
 * ShaderDescription: Convolves a matrix with the texture
 */

/*
* Name: Convolution Matrix
* Default: mat3(1, 2, 1, 2, 4, 2, 1, 2, 1)
*
/* uniform */ mat3 convolutionMatrix = mat3(1, 2, 1, 2, 4, 2, 1, 2, 1);// mat3(-1,0,1,-2,0,2,-1,0,1);

uniform sampler2D tex;

/*uniform*/ float texWidth = 2400.0;
/*uniform*/ float texHeight = 1800.0;


void convolute(mat3 convMat) {

	float sum = 0.0;
	vec3 xt = vec3( -1.0 / texWidth, 0, 1.0 / texWidth );
	vec3 yt = vec3( -1.0 / texHeight, 0, 1.0 / texHeight );

	vec3 color = vec3(0, 0, 0);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			color.rgb += convMat[i][j] * texture2D(tex,
				vec2(gl_TexCoord[0].xy + vec2(xt[i], yt[j]))).rgb;
			sum += convMat[i][j];
		}
	}

	gl_FragColor = vec4(0, 0, 0, 1);
	gl_FragColor.rgb = color.rgb / sum;

}

vec4 getConvoluteValue(mat3 convMat) {

	float sum = 0.0;
	vec3 xt = vec3( -1.0 / texWidth, 0, 1.0 / texWidth );
	vec3 yt = vec3( -1.0 / texHeight, 0, 1.0 / texHeight );

	vec3 color = vec3(0, 0, 0);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			color.rgb += convMat[i][j] * texture2D(tex,
				vec2(gl_TexCoord[0].xy + vec2(xt[i], yt[j]))).rgb;
			sum += convMat[i][j];
		}
	}

	vec4 retColor = vec4(0, 0, 0, 1);
	retColor.rgb = color.rgb / sum;

	return retColor;

}

#ifdef ASL_MAIN

void main() {

convolute(convolutionMatrix);

}

#endif

