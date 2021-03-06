
/**
 * ShaderName: GrayScale
 * ShaderDescription: Transforms a picture into a grayscale picture
 */

uniform sampler2D tex;

float getGray(vec4 t ) {
    // Convert to grayscale using NTSC conversion weights
    return dot(t.rgb, vec3(0.299, 0.587, 0.114));
}

#ifdef ASL_MAIN

void main()
{
    float gray = getGray(texture2D(tex,gl_TexCoord[0].st));
    gl_FragColor = vec4(gray, gray, gray, gl_Color.a);
}

#endif

