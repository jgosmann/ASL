
/**
 * ShaderName: SobelFilter
 * ShaderDescription: The SobelFilter for edge detection.
 * Depends: convolveMat3.asl.frag
 */

const mat3 sobelx = mat3(1,0,-1,2,0,-2,1,0,-1);
const mat3 sobely = mat3(1,2,1,0,0,0,-1,-2,-1);


vec4 getSobelValue(){
	vec4 sx = getConvoluteValue(sobelx);
	vec4 sy = getConvoluteValue(sobely);

	return sqrt(vec4(sx * sx + sy * sy));

}

#ifdef ASL_MAIN

void main() {
	gl_FragColor = getSobelValue();
	gl_FragColor.a = 1.0;
}

#endif
