/**
* @file Application.cpp
*/
#include "Command.h"

/**
* �A�v���P�[�V�����̖{��.
*/
void application()
{
  set_text(0, 0, "[����񂯂�Q�[��]");
  set_text(0, 40, "���I��ł�������");

  // �v���C���[�Ɏ��I��ł��炤.
  // �I�΂ꂽ�̂��O�[�Ȃ�0���A�`���L�Ȃ�1���A�p�[�Ȃ�2��player_hand�Ɋi�[�����.
  int player_hand = select(40, 80, 3, "�O�[", "�`���L", "�p�[");

  // CPU�̎��I��.
  // �I�񂾂̂��O�[�Ȃ�0���A�`���L�Ȃ�1���A�p�[�Ȃ�2��cpu_hand�Ɋi�[�����.
  int cpu_hand = random(0, 2); // 0�`2�̂����ꂩ�̐���������ׂɑI�΂��.

  set_text(0, 80, "����񂯂�ۂ�I");

  // �����Ƀv���C���[�̎��\��
  if (player_hand == 0) {
    set_text(0, 160, "���Ȃ�: �O�[");
  } else if (player_hand == 1) {
    set_text(0, 160, "���Ȃ�: �`���L");
  } else if (player_hand == 2) {
    set_text(0, 160, "���Ȃ�: �p�[");
  }

  // �E����CPU�̎��\��
  if (cpu_hand == 0) {
    set_text(0, 200, "������: �O�[");
  } else if (cpu_hand == 1) {
    set_text(0, 200, "������: �`���L");
  } else if (cpu_hand == 2) {
    set_text(0, 200, "������: �p�[");
  }
  wait(1);

  // ���l�̔�r�͓�d�̃C�R�[���L���u==�v�ōs��.
  // �u&&�v�́u���v�A�u||�v�́u���́v�Ƃ����Ӗ������L��.
  // ���̗D�揇�ʂ𒲐�����ɂ͎Z���Ɠ��l�ɃJ�b�R�u()�v���g��.
  // �O�[��0�A�`���L��1�A�p�[��2�Ȃ̂ŁA�Ⴆ�΃v���C���[�̎肪0(�O�[)��CPU�̎肪1(�`���L)�Ȃ�A�v���C���[�̏����ƂȂ�.
  // �������蕉�����肵���ꍇ�́u�������v����Ȃ��̂�aiko_flag���u�U�v�ɂ��Ă���.
  // �����Ă����Ȃ��������Ă����Ȃ��ꍇ�́u�������v�Ȃ̂�aiko_flag���u�^�v�ɂ��Ă���.
  if (player_hand == 0 && cpu_hand == 1) {
    set_text(0, 280, "���Ȃ��̏����ł��I");
  }
  else if (player_hand == 1 && cpu_hand == 2) {
    set_text(0, 280, "���Ȃ��̏����ł��I");
  }
  else if (player_hand == 2 && cpu_hand == 0) {
    set_text(0, 280, "���Ȃ��̏����ł��I");
  }
  else if (player_hand == 0 && cpu_hand == 2) {
    set_text(0, 280, "���Ȃ��̕����ł�");
  }
  else if (player_hand == 1 && cpu_hand == 0) {
    set_text(0, 280, "���Ȃ��̕����ł�");
  }
  else if (player_hand == 2 && cpu_hand == 1) {
    set_text(0, 280, "���Ȃ��̕����ł�");
  }
  else {
    set_text(0, 280, "�������ł�");
  }

  wait(2);

  set_text(0, 320, "�����L�[�������ƏI�����܂�");
  wait_any_key();
}
