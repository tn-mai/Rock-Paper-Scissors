/**
* @file Main.cpp
*/
#include "../Command.h"

void initialize();
void application();
void finalize();

/**
* プログラムのエントリーポイント.
*/
int main()
{
  initialize();
  application();
  finalize();
}