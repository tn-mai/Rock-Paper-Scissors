/**
* @file Application.cpp
*/
#include "Command.h"

/**
* アプリケーションの本体.
*/
void application()
{
  // [文字を表示する]
  // set_text(せっと・てきすと)は、文字を表示する関数(かんすう).
  // 関数は「名前(引数1, 引数2, 引数3, ...)」のように、
  // 丸括弧の内側に引数(ひきすう)というものをカンマで区切って指定できる.
  // 「引数」というのは関数を実行するときのパラメーター.
  // 関数によってパラメーターの内容や数が異なる.
  // set_text関数の場合、最初の2つの数字が「文字を表示する位置」、3つめが「表示する文章」となっている.
  // 「この部分は文章ですよ」ということを示すには、文章を「"(だぶる・くぉーてーしょん)」という記号で囲まなくてはならない.
  set_text(0, 0, "[じゃんけんゲーム]");
  set_text(0, 40, "手を選んでください");

  // [選択肢を表示して選んでもらう]
  // select(せれくと)は、選択肢を表示する関数.
  // 最初の2つの数字が「選択肢を表示する位置」、3つめが「選択肢の数」、4つめ以降が「選択肢の文章」となっている.
  // ここでは、プレイヤーに手を選んでもらうために使っている.
  // どの選択肢が選ばれたのかは、関数の「戻り値(もどりち)」というもので分かるようになっている.
  // 「戻り値」というのは、関数の結果を表す数値や文章など.
  // 関数によって戻り値の内容は異なる.
  // どの関数も戻り値はひとつまで. また、戻り値がない関数もある.
  // select関数の場合、選択肢には左から順番に0,1,2,...という番号が自動で振られ、選ばれた選択肢の番号が戻り値になる.
  // つまり、選ばれたのがグーなら0、チョキなら1、パーなら2が戻り値になる.
  //
  // 戻り値は「変数(へんすう)」というものに格納することができる.
  // 「変数」は数値や文章の入れ物.
  // 変数を作るには、変数の「型」と「名前」が必要.
  // 「型」というのは「変数に入れるものの種類」.
  // 例えば「int(いんと)」は数値を入れる変数を作るための「型」.
  // ここではplayer_handという名前の変数を作り、そこにselect関数の結果を格納している.
  int player_hand = select(40, 80, 3, "グー", "チョキ", "パー");

  // [CPUの手を選ぶ]
  // random(らんだむ)は、整数を無作為に選択する関数.
  // 最初の引数が選択範囲の最小値、2つめの引数が最大値になる.
  // ここではコンピューターの手を決めるために使っている.
  // 最初の引数が0、次が2なので、0〜2の範囲からひとつの整数が無作為に選ばれる.
  // 選ばれた数値が0ならグー、1ならチョキ、2ならパーが選ばれたとみなすことで、プレイヤーの手と対応させる.
  int cpu_hand = random(0, 2);

  set_text(0, 80, "じゃんけんぽん！");

  // [プレイヤーの手を表示]
  // if(いふ)は、変数の内容によって実行するプログラムを切り替える「制御文(せいぎょぶん)」.
  // if文は「if (条件式) { 条件式が真のときだけ実行されるプログラム }」の形を取る.
  // 「条件式」は「真」か「偽」になる数式.
  // 例えば「1 == 1」という条件式は「真」、「1 == 0」は「偽」になる.
  // C++言語では、数値の比較に「==(いこーる・いこーる)」を使う.
  // 「=(いこーる)」は代入に使われているので比較には使えない.
  // 条件式に変数が書かれている場合、変数に入れられた数値を使って比較を行う.
  if (player_hand == 0) {
    set_text(0, 160, "あなた: グー");
  }
  if (player_hand == 1) {
    set_text(0, 160, "あなた: チョキ");
  }
  if (player_hand == 2) {
    set_text(0, 160, "あなた: パー");
  }

  // [CPUの手を表示]
  if (cpu_hand == 0) {
    set_text(0, 200, "あいて: グー");
  }
  if (cpu_hand == 1) {
    set_text(0, 200, "あいて: チョキ");
  }
  if (cpu_hand == 2) {
    set_text(0, 200, "あいて: パー");
  }

  // [一定時間待つ]
  // wait(うぇいと)は、指定された秒数だけプログラムの実行を停止する関数.
  wait(1);

  // [勝敗を判定する]
  // 条件式は組み合わせるこができる.
  // 条件式を組み合わせるには「&&(あんど・あんど)」を使う.
  // 例えば、プレイヤーの手が0(グー)かつCPUの手が1(チョキ)なら、プレイヤーの勝ち、というようにしたい.
  // これは「player_hand == 0 && cpu_hand == 1」という条件式で表すことができる.
  // つまり、&&は「且つ(かつ)」という意味だといえる.
  // 同じ様に、プレイヤーの手が2(パー)でCPUの手が1(チョキ)なら、CPUの勝ちとなる.
  // これは「player_hand == 2 && cpu_hand == 1」という条件式で表すことができる.
  // どちらも同じ手を出した場合は「あいこ」になるが、
  // これは「player_hand == cpu_hand」という条件式で表すことができる.
  if (player_hand == 0 && cpu_hand == 1) {
    set_text(0, 280, "あなたの勝ちです！");
  }
  if (player_hand == 1 && cpu_hand == 2) {
    set_text(0, 280, "あなたの勝ちです！");
  }
  if (player_hand == 2 && cpu_hand == 0) {
    set_text(0, 280, "あなたの勝ちです！");
  }

  if (player_hand == 0 && cpu_hand == 2) {
    set_text(0, 280, "あなたの負けです");
  }
  if (player_hand == 1 && cpu_hand == 0) {
    set_text(0, 280, "あなたの負けです");
  }
  if (player_hand == 2 && cpu_hand == 1) {
    set_text(0, 280, "あなたの負けです");
  }

  if (player_hand == cpu_hand) {
    set_text(0, 280, "あいこです");
  }

  wait(2);

  set_text(0, 320, "何かキーを押すと終了します");

  // [何かキーが押されるまで待つ]
  // wait_any_key(うぇいと・えにー・きー)は、なにかキーが押されるまでプログラムの実行を停止する関数.
  wait_any_key();
}
