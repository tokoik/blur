#version 120

// ���ˌ����x
varying vec4 iamb;	// �����̔��ˌ�
varying vec4 idiff;	// �g�U���ˌ�
varying vec4 ispec;	// ���ʔ��ˌ�

// ���_�̑��x�x�N�g���̕�Ԓl
varying vec3 vel;

void main(void)
{
  // �F�o�b�t�@�ւ̏�������
  gl_FragData[0] = iamb + idiff + ispec;

  // ���x�o�b�t�@�ւ̏�������
  gl_FragData[1] = vec4(vel, 1.0);
}
