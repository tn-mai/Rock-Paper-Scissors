/**
* @file Main.cpp
*/
#include <objbase.h>
#include "Audio.h"
#include "GLFWEW.h"
#include <iostream>

extern bool hasQuitRequest;
void initialize();
void application();
void finalize();

/**
* �v���O�����̃G���g���[�|�C���g.
*/
int main()
{
  CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  if (!window.Init(800, 600, "OpenGL 2D")) {
    return 1;
  }

  if (!Audio::Engine::Get().Initialize()) {
    std::cerr << "Audio Engine�̏������Ɏ��s." << std::endl;
  }

  initialize();
  application();
  finalize();

  Audio::Engine::Get().Destroy();

  CoUninitialize();
  std::cout << "Finish." << std::endl;
}