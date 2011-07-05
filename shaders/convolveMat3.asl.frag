#version 120

/∗∗
 ∗ ShaderName: ConvolveMat3
 ∗ ShaderDescription: Convolves a 3x3 matrix with the 2Dtexture.
 ∗/


/∗
 ∗ Name: Convolution Matrix
 * Default: mat3(1, 2, 1, 2, 4, 2, 1, 2, 1)
 ∗/
/* uniform */ mat3 convolutionMatrix = mat3(1, 2, 1, 2, 4, 2, 1, 2, 1);


uniform sampler2D tex;
uniform float texWidth;
uniform float texHeight;


void convolute(mat3 convMat) {

	int sum = 0;
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
	gl_FragColor.rgb = color.rgb / float(sum);

}



void main() {

convolute(convolutionMatrix);

}

