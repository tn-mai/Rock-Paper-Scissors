#include "Command.h"

// ��������v���O�����̎��s���J�n�����
int main()
{
  // �v���O�����̏���������
  initialize("����񂯂�Q�[��");

  // �w�i��\��
  set_image(No_0, 400, 300, "bg_paper.jpg");

  // ���͂�\��
  set_text("����񂯂�ۂ�I");

  // ����񂯂�̎���`����
  int gu = 0;
  int choki = 1;
  int pa = 2;

  // ���Ȃ��̎�
  int player_hand = gu;

  // �R���s���[�^�[�̎�
  int cpu_hand = choki;

  // 1�b�҂�
  wait(1);

  // ���s�𔻒肷��
  if (player_hand == gu && cpu_hand == choki) {
    set_text("���Ȃ��̏����ł��I");
  }

  // �����L�[���������܂ő҂�
  set_text("�����L�[�������ƏI�����܂�");
  wait_any_key();

  // �v���O�����̏I������
  finalize();
}