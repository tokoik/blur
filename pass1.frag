#version 120

// ���ˌ����x
varying vec4 iamb;	// �����̔��ˌ�
varying vec4 idiff;	// �g�U���ˌ�
varying vec4 ispec;	// ���ʔ��ˌ�

void main(void)
{
  // �F�o�b�t�@�ւ̏�������
  gl_FragColor = iamb + idiff + ispec;
}
