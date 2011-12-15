/**
 * ShaderName: Sharpen
 * ShaderDescription: Schärft das Bild.
 * Depends: convolveMat3.asl.frag
 */

const mat3 sharpenMat = mat3(-1, -1, -1, -1, 9, -1, -1, -1, -1);

void main() {
	gl_FragColor = getConvoluteValue(sharpenMat);
}
