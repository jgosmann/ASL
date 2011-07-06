
/**
 * ShaderName: SobelFilter
 * ShaderDescription: The SobelFilter for edge detection.
 * Depends: convolveMat3.asl.frag
 */

mat3 sobelx = mat3(-1,0,1,-2,0,2,-1,0,1);
mat3 sobely = mat3(-1,-2,-1,0,0,0,1,2,1);

void main() {

	vec4 sx = getConvoluteValue(sobelx) + vec4(0.5,0.5,0.5,0);
	vec4 sy = getConvoluteValue(sobely) + vec4(0.5,0.5,0.5,0);

	gl_FragColor = vec4(sx * sx + sy * sy);

	
	
}
