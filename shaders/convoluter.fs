uniform sampler2D tex;
mat3 gauss = mat3(0, 1, 0, 1, 2, 1, 0, 1, 0);
float texWidth = 512.0;
float texHeight = 512.0;

void main()
{
	vec3 xt = vec3( -1.0 / texWidth, 0, 1.0 / texWidth );
	vec3 yt = vec3( -1.0 / texHeight, 0, 1.0 / texHeight );

	vec3 color = vec3(0, 0, 0);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			color.rgb += gauss[i][j] * texture2D(tex,
				vec2(gl_TexCoord[0].xy + vec2(xt[i], yt[j]))).rgb;
		}
	}

	gl_FragColor = vec4(0, 0, 0, 1);
	gl_FragColor.rgb = color.rgb / 6.0;
}
