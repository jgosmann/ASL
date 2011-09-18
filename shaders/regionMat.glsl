uniform sampler2D tex;
uniform int texWidth;
uniform int texHeight;

vec4[] getRegionMat() {

	vec2 invTexSize = vec2(1.0 / float(texWidth), 1.0 / float(texHeight));
	vec3 xt = vec3( -invTexSize.x, 0, invTexSize.x );
	vec3 yt = vec3( -invTexSize.y, 0, invTexSize.y );

	vec4 regionMat[9];
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			regionMat[i*3+j] = texture2D(tex,vec2(gl_TexCoord[0].xy + vec2(xt[i], yt[j]))).rgb;
		}
	}


	return regionMat;
}
