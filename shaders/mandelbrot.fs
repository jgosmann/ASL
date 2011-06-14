#version 120
uniform sampler2D tex;
int texWidth = 2048;
int texHeight = 2048;

int iterations = 200;

vec2 multiplyComplex(vec2 a, vec2 b) {
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + b.x * a.y);
}

void main()
{
    vec2 z = vec2(0, 0);
    vec2 c = vec2(gl_FragCoord.x / texWidth * 4 , gl_FragCoord.y / texHeight * 4 );
for (int i = 0; i < iterations; ++i) {
        z = multiplyComplex(z, z);
        z += c;
        if (length(z) > 2) {
            float texCoord =  float(i) / iterations;
            gl_FragColor = texture2D(tex, vec2(texCoord));
            return;
        }
    }
	gl_FragColor = vec4(0, 0, 0, 1);
}
