#include "Command.h"

// ��������v���O�����̎��s���J�n�����
int main()
{
    // �v���O�����̏���������
    initialize("����񂯂�Q�[��");

    // �w�i��\��
    image background;
    background.set(400, 300, "bg_paper.jpg");

    // ���͂�\��
    printf("����񂯂�ۂ�I");

    // ����񂯂�̎���`����
    int gu = 0;
    int choki = 1;
    int pa = 2;

    // �v���C���[�̎�����߂�
    int player_hand = gu;

    // �R���s���[�^�[�̎�����߂�
    int cpu_hand = choki;

    // �v���C���[�̎��\������
    if (player_hand == gu) {
        printf("�v���C���[�̎�F�O�[");
    }

    // �R���s���[�^�[�̎��\������
    if (cpu_hand == choki) {
        printf("CPU�̎�F�`���L");
    }

    // 1�b�҂�
    usleep(1'000'000);
  
    // ���s��\������
    printf("���Ȃ��̏����ł��I");

    // �Ȃɂ��L�[���������܂ő҂�
    printf("�Ȃɂ��L�[�������ƏI�����܂�");
    wait_any_key();

    // �v���O�����̏I������
    finalize();
}