#version 120

uniform sampler2D texure0, texure1;
varying vec4 dc, sc;

void main(void)
{
  gl_FragColor = dc + sc;
}
