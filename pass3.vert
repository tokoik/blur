#version 120

// �N���b�s���O��Ԃ𕢂��|���S���̒��_�ʒu
attribute vec4 pv;

// ���_�̈ʒu���狁�߂�e�N�X�`�����W
varying vec2 t;

void main(void)
{
  // ���_�ʒu�͂��̂܂܃��X�^���C�U�ɑ���
  gl_Position = pv;

  // ���X�^���C�U�Œ��_�ʒu���Ԃ��ăN���b�s���O��Ԓ��̉�f�ʒu�����߂�
  t = pv.xy * 0.5 + 0.5;
}
