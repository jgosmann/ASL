#version 120

/**
 * ShaderName: Colorizer
 * ShaderDescription: Colors the image with the specified color.
 */

uniform sampler2D tex;
uniform float texWidth;
uniform float texHeight;

/**
 * Name: Color value
 * Default: vec3(1.0, 0.0, 0.0)
 * Control: color, default
 */
uniform vec3 color;

void main() {
  gl_FragColor = mix(texture2D(tex, gl_TexCoord[0].st), vec4(color.r, color.g, color.b, 1.0), 0.5);
}

