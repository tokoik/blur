#version 120

uniform sampler2D texure0, texure1;
varying vec4 iamb, idiff, ispec;

void main(void)
{
  gl_FragColor = iamb + idiff + ispec;
}
