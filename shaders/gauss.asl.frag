

/**
 * ShaderName: Gaussfilter
 * ShaderDescription: Uses a gauss filter with the given texture.
 * Depends: convolveMat3.asl.frag
 */

mat3 gaussMat = mat3(1, 2, 1, 2,4, 2, 1, 2, 1);


uniform sampler2D tex;
uniform float texWidth;
uniform float texHeight;

void main() {
	convolute(gaussMat);
}
