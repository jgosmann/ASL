/**
 * ShaderName: Mandelbrot Set
 * ShaderDescription: Calculates the Mandelbrot set using the current image as
 *     basis for a color palette.
 * Depends: juliaSets.asl.frag, mapCoords.asl.glsl
 */

void main() {
    gl_FragColor = calcJuliaSetColor(vec2(0, 0), mapCoords(gl_FragCoord.xy));
}

