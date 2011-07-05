uniform sampler2D tex;

void main()
{
         // Convert to grayscale using NTSC conversion weights
    float gray = dot(texture2D(tex,vec2(gl_TexCoord[0].xy)).rgb, vec3(0.299, 0.587, 0.114));
    
    gl_FragColor = vec4(gray, gray, gray, gl_Color.a);
}
