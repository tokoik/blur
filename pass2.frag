#version 120

// �w�i�F
const vec4 bg = vec4(0.1, 0.3, 0.5, 1.0);

// ����
uniform vec2 rn[16];

// �F�o�b�t�@
uniform sampler2D texture0;

// ���x�o�b�t�@
uniform sampler2D texture1;

// �X�N���[����̈ʒu
varying vec2 t;

// v ������ n ��f�̐F�̕��ς����߂�
vec4 average(vec2 v, int n)
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
  vec4 c = vec4(0.0);
  vec4 v = texture2D(texture1, t);

  if (v.a != 0.0)
  {
    // �t���O�����g���I�u�W�F�N�g��Ȃ炻�����ڂ���
    gl_FragColor = average(v.xy, 16);
  }
  else
  {
    // �t���O�����g���w�i�Ȃ�
    int count = 0;
    vec4 d = vec4(0.0);

#if 0
    for (int i = 0; i < 16; ++i)
    {
      // ���̃t���O�����g�̎��͂������_���ɃT���v�����O����
      vec4 p = texture2D(texture1, t + rn[i]);

      if (p.a != 0.0)
      {
        // �I�u�W�F�N�g��̃t���O�����g������������
        vec4 q = texture2D(texture1, t - p.xy);
        
        if (q.a != 0.0)
        {
          // �I�u�W�F�N�g��̃t���O�����g���������炻�����ڂ���
          d += average(q.xy, 16);
          ++count;
        }
      }
    }

    if (count == 0)
      gl_FragColor = texture2D(texture1, t);
    else
      gl_FragColor = d / float(count);
#else
    gl_FragColor = texture2D(texture1, t);
#endif
  }
}
