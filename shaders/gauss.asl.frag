/**
 * ShaderName: Gaussfilter
 * ShaderDescription: Uses a gauss filter with the given texture.
 * Depends: convolveMat3.asl.frag
 */

mat3 gaussMat = mat3(1, 2, 1, 2, 4, 2, 1, 2, 1);

void main() {
	gl_FragColor = getConvoluteValue(gaussMat);
}

