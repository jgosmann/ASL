/**
 * ShaderName: Laplace Con4
 * ShaderDescription: Uses a Laplace Filter with the given texture.
 * Uses a connectivity of 4.
 * Depends: convolveMat3.asl.frag
 */

mat3 laplaceMat = mat3(0, 1, 0, 1, -4, 1, 0, 1, 0);

void main() {
	gl_FragColor = getConvoluteValue(laplaceMat);
}
