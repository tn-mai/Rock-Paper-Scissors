/**
* @file Application.cpp
*/
#include "Command.h"

/**
* アプリケーションの本体.
*/
void application()
{
  set_text(0, 0, "[じゃんけんゲーム]");
  set_text(0, 40, "手を選んでください");

  // プレイヤーに手を選んでもらう.
  // 選ばれたのがグーなら0が、チョキなら1が、パーなら2がplayer_handに格納される.
  int player_hand = select(40, 80, 3, "グー", "チョキ", "パー");

  // CPUの手を選ぶ.
  // 選んだのがグーなら0が、チョキなら1が、パーなら2がcpu_handに格納される.
  int cpu_hand = random(0, 2); // 0～2のいずれかの整数が無作為に選ばれる.

  set_text(0, 80, "じゃんけんぽん！");

  // 左側にプレイヤーの手を表示
  if (player_hand == 0) {
    set_text(0, 160, "あなた: グー");
  } else if (player_hand == 1) {
    set_text(0, 160, "あなた: チョキ");
  } else if (player_hand == 2) {
    set_text(0, 160, "あなた: パー");
  }

  // 右側にCPUの手を表示
  if (cpu_hand == 0) {
    set_text(0, 200, "あいて: グー");
  } else if (cpu_hand == 1) {
    set_text(0, 200, "あいて: チョキ");
  } else if (cpu_hand == 2) {
    set_text(0, 200, "あいて: パー");
  }
  wait(1);

  // 数値の比較は二重のイコール記号「==」で行う.
  // 「&&」は「且つ」、「||」は「又は」という意味を持つ記号.
  // 式の優先順位を調整するには算数と同様にカッコ「()」を使う.
  // グーは0、チョキは1、パーは2なので、例えばプレイヤーの手が0(グー)でCPUの手が1(チョキ)なら、プレイヤーの勝ちとなる.
  // 勝ったり負けたりした場合は「あいこ」じゃないのでaiko_flagを「偽」にしておく.
  // 勝ってもいないし負けてもいない場合は「あいこ」なのでaiko_flagを「真」にしておく.
  if (player_hand == 0 && cpu_hand == 1) {
    set_text(0, 280, "あなたの勝ちです！");
  }
  else if (player_hand == 1 && cpu_hand == 2) {
    set_text(0, 280, "あなたの勝ちです！");
  }
  else if (player_hand == 2 && cpu_hand == 0) {
    set_text(0, 280, "あなたの勝ちです！");
  }
  else if (player_hand == 0 && cpu_hand == 2) {
    set_text(0, 280, "あなたの負けです");
  }
  else if (player_hand == 1 && cpu_hand == 0) {
    set_text(0, 280, "あなたの負けです");
  }
  else if (player_hand == 2 && cpu_hand == 1) {
    set_text(0, 280, "あなたの負けです");
  }
  else {
    set_text(0, 280, "あいこです");
  }

  wait(2);

  set_text(0, 320, "何かキーを押すと終了します");
  wait_any_key();
}
