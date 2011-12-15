/**
 * ShaderName: ConvolveMat3
 * ShaderDescription: Convolves a matrix with the texture
 */

uniform sampler2D tex;
uniform int texWidth;
uniform int texHeight;

vec4 getConvoluteValue(mat3 convMat) {

    float sum = 0.0;
    vec2 invTexSize = vec2(1.0 / float(texWidth), 1.0 / float(texHeight));
    vec3 xt = vec3( -invTexSize.x, 0, invTexSize.x );
    vec3 yt = vec3( -invTexSize.y, 0, invTexSize.y );

    vec3 color = vec3(0, 0, 0);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            color.rgb += convMat[i][j] * texture2D(tex,
                vec2(gl_TexCoord[0].xy + vec2(xt[i], yt[j]))).rgb;
            sum += convMat[i][j];
        }
    }

    vec4 retColor = vec4(0, 0, 0, 1);
    if(sum != 0.0){
        retColor.rgb = color.rgb / sum;
    } else {
        retColor.rgb = color.rgb;
    }

    return retColor;
}

#ifdef ASL_MAIN
/**
* Name: Convolution Matrix
* Default: mat3(1, 2, 1, 2, 4, 2, 1, 2, 1)
*/
uniform mat3 convolutionMatrix;

void main() {
    gl_FragColor = getConvoluteValue(convolutionMatrix);
}
#endif

