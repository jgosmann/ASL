#version 120

/**
 * ShaderName: Colorizer
 * ShaderDescription: Colors the image with the specified color.
 */

uniform sampler2D tex;
uniform float texWidth;
uniform float texHeight;

/**
 * Name: Red color value
 * Default: 1.0
 */
uniform float red;

/**
 * Name: Green color value
 * Default: 1.0
 */
uniform float green;

/**
 * Name: Blue color value
 * Default: 1.0
 */
uniform float blue;

void main() {
  gl_FragColor = mix(texture2D(tex, gl_TexCoord[0].st), vec4(red, green, blue, 1.0), 0.5);
}

