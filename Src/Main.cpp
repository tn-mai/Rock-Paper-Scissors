#include "Command.h"

// ここからプログラムの実行が開始される
int main()
{
  // プログラムの初期化処理
  initialize("じゃんけんゲーム");

  // 背景を表示
  set_image(No_0, 400, 300, "bg_paper.jpg");

  // 文章を表示
  set_text("じゃんけんぽん！");

  // じゃんけんの手を定義する
  int gu = 0;
  int choki = 1;
  int pa = 2;

  // あなたの手
  int player_hand = gu;

  // コンピューターの手
  int cpu_hand = choki;

  // 1秒待つ
  wait(1);

  // 勝敗を判定する
  if (player_hand == gu && cpu_hand == choki) {
    set_text("あなたの勝ちです！");
  }

  // 何かキーが押されるまで待つ
  set_text("何かキーを押すと終了します");
  wait_any_key();

  // プログラムの終了処理
  finalize();
}