#version 120

uniform sampler2D texture0;
uniform sampler2D texture1;

varying vec2 t;

void main(void)
{
  gl_FragColor = texture2D(texture0, t);
}
