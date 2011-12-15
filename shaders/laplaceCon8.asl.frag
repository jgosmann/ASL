/**
 * ShaderName: Laplace Con8
 * ShaderDescription: Uses a Laplace Filter with the given texture.
 * Uses a connectivity of 8.
 * Depends: convolveMat3.asl.frag
 */

const mat3 laplaceMat = mat3(1, 1, 1, 1, -8, 1, 1, 1, 1);

void main() {
	gl_FragColor = getConvoluteValue(laplaceMat);
}
