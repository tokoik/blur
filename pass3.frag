#version 120

// ����
uniform vec2 rn[16];

// �F�o�b�t�@
uniform sampler2D texture0;

// ���x�o�b�t�@
uniform sampler2D texture1;

// �X�N���[����̈ʒu
varying vec2 t;

// v ������ n ��f�̐F�̕��ς����߂�
vec4 average(in vec2 v, in int n)
{
  vec4 c = vec4(0.0);

  for (int i = 0; i < n; ++i)
  {
    c += texture2D(texture0, t - v * float(i) / float(n));
  }

  return c / float(n);
}

void main(void)
{
  vec4 v = texture2D(texture1, t);

  gl_FragColor = texture2D(texture1, t);
  return;

  if (v.a != 0.0)
  {
    // �t���O�����g���I�u�W�F�N�g��Ȃ炻�����ڂ���
    gl_FragColor = average(v.xy, 16);
  }
  else
  {
    // �t���O�����g���I�u�W�F�N�g��łȂ���Δw�i�F�Ƃ���
    gl_FragColor = texture2D(texture0, t);
  }
}
