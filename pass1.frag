#version 120

uniform sampler2D texure0, texure1;
varying vec4 iamb, idiff, ispec;
varying vec2 vel;

void main(void)
{
  gl_FragData[0] = iamb + idiff + ispec;
  gl_FragData[1] = vec4(vel, 0.0, 1.0);
}
