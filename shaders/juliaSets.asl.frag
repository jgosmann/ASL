/**
 * ShaderName: Julia Sets
 * ShaderDescription: Calculates Julia set fractals using the picture loaded as
 *     basis for the color palette.
 * Depends: mapCoords.asl.glsl, cmplxMultiply.glsl
 */

uniform sampler2D tex;
uniform int texWidth;
uniform int texHeight;

/**
 * Name: Iterations
 * Description: Maximum number of iterations to calculate per pixel. High
 *     numbers may reduce the performance.
 * Default: 200
 * Range: 0, max
 */
uniform int iterations;

vec4 calcJuliaSetColor(vec2 z, vec2 c) {
    for (int i = 0; i < iterations; ++i) {
        z = cmplxMultiply(z, z);
        z += c;
        if (dot(z, z) > 4.0) {
            float texCoord =  float(i) / float(iterations);
            return texture2D(tex, vec2(texCoord));
        }
    }
    return texture2D(tex, gl_TexCoord[0].xy);
}

#ifdef ASL_MAIN
/**
 * Name: c
 * Description: Parameter to vary the Julia set.
 */
uniform vec2 c;

void main() {
    gl_FragColor = calcJuliaSetColor(mapCoords(gl_FragCoord.xy), c);
}
#endif

