#version 330 compatibility

uniform sampler2D texture;

void main(void)
{
  vec4 mix;

  if(mod(gl_PointCoord.x, 2.0) == 0.0) {
    mix = vec4(0.8, 0.8, 0.8, 1.0);
  } else {
    mix = vec4(0.2, 0.2, 0.2, 1.0);
  }
  gl_FragColor = mix * texture2D(texture, gl_TexCoord[0].xy);
}
