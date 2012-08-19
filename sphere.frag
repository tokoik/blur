#version 120

varying vec4 iamb;
varying vec4 idiff;
varying vec4 ispec;

void main(void)
{
  gl_FragColor = iamb + idiff + ispec;
}
