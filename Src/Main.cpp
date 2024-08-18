#include "Command.h"

// ここからプログラムの実行が開始される
int main()
{
    // プログラムの初期化処理
    initialize("じゃんけんゲーム");

    // 背景を表示
    image background;
    background.set(400, 300, "bg_paper.jpg");

    // 文章を表示
    printf("じゃんけんぽん！");

    // じゃんけんの手を定義する
    int gu = 0;
    int choki = 1;
    int pa = 2;

    // プレイヤーの手を決める
    int player_hand = gu;

    // コンピューターの手を決める
    int cpu_hand = choki;

    // プレイヤーの手を表示する
    if (player_hand == gu) {
        printf("プレイヤーの手：グー");
    }

    // コンピューターの手を表示する
    if (cpu_hand == choki) {
        printf("CPUの手：チョキ");
    }

    // 1秒待つ
    usleep(1'000'000);
  
    // 勝敗を表示する
    printf("あなたの勝ちです！");

    // なにかキーが押されるまで待つ
    printf("なにかキーを押すと終了します");
    wait_any_key();

    // プログラムの終了処理
    finalize();
}