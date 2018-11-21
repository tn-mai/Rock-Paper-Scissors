/**
* @file Command.cpp
*/
#include <objbase.h>
#include "Command.h"
#include "GLFWEW.h"
#include "Sprite.h"
#include "Font.h"
#include "Audio.h"
#include <glm/gtc/constants.hpp>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <iostream>
#include <stdarg.h>
#include <algorithm>
#include <unordered_map>

bool hasQuitRequest = false;

const ImageNo image_no0(0);
const ImageNo image_no1(1);
const ImageNo image_no2(2);
const ImageNo image_no3(3);
const ImageNo image_no4(4);
const ImageNo image_no5(5);
const ImageNo image_no6(6);
const ImageNo image_no7(7);
const ImageNo image_no8(8);
const ImageNo image_no9(9);
const ImageNo image_no10(10);
const ImageNo image_no11(11);
const ImageNo image_no12(12);
const ImageNo image_no13(13);
const ImageNo image_no14(14);
const ImageNo image_no15(15);
const ImageNo image_no16(16);
const ImageNo image_no17(17);
const ImageNo image_no18(18);
const ImageNo image_no19(19);

namespace /* unnamed */ {

/**
* �C�[�W���O�̎��.
*/
enum easing_type
{
  linear,
  ease_in,
  ease_out,
  ease_in_out,
  ease_out_back,
  ease_out_bounce,
};

/**
* �C�[�W���O����N���X.
*/
template<typename T>
struct action
{
  T start;
  T end;
  easing_type easing;
  float seconds;

  float timer;

  void init(const T& s, const T& e, int ease, double sec) {
    start = s;
    end = e;
    easing = static_cast<easing_type>(glm::clamp(ease, 0, static_cast<int>(ease_out_bounce)));
    seconds = static_cast<float>(sec);
    timer = 0;
  }

  T update(float delta) {
    if (seconds <= 0) {
      return end;
    }
    timer = std::min(timer + delta, seconds);
    float ratio = (timer / seconds);
    switch (easing) {
    default:
    case linear: break;
    case ease_in: ratio *= ratio; break;
    case ease_out: ratio = 2.0f * ratio - ratio * ratio; break;
    case ease_in_out:
      ratio *= 2.0f;
      if (ratio < 1.0f) {
        ratio *= ratio;
      } else {
        ratio -= 1.0f;
        ratio = 2.0f * ratio - ratio * ratio;
        ratio += 1.0f;
      }
      ratio *= 0.5f;
      break;
    case ease_out_back: {
      static const float scale = 1.70158f * 1.525f;
      ratio -= 1;
      ratio = 1 + 2.70158f * ratio * ratio * ratio + 1.70158f * ratio * ratio;
      break;
    }
    case ease_out_bounce: {
      if (ratio < (1 / 2.75f)) {
        ratio = 7.5625f * ratio * ratio;
      } else if (ratio < (2 / 2.75f)) {
        ratio -= 1.5f / 2.75f;
        ratio = 7.5625f * ratio * ratio + 0.75f;
      } else if (ratio < (2.5 / 2.75f)) {
        ratio -= 2.25f / 2.75f;
        ratio = 7.5625f * ratio * ratio + 0.9375f;
      } else {
        ratio -= 2.625f / 2.75f;
        ratio = 7.5625f * ratio * ratio + 0.984375f;
      }
      break;
    }
    }
    return start + (end - start) * ratio;
  }
};

// �ΏۂƂȂ�p�����[�^�[�ʂ̃C�[�W���O����N���X.
using translate_action = action<glm::vec2>;
using scale_action = action<glm::vec2>;
using rotate_action = action<float>;
using shear_action = action<float>;
using color_action = action<glm::vec4>;

/**
* �e��C�[�W���O����@�\�t���X�v���C�g�N���X.
*/
struct actable_sprite : Sprite
{
  translate_action translate;
  scale_action scale;
  rotate_action rotate;
  shear_action shear;
  color_action color;

  void init_action() {
    translate.init(Position(), Position(), linear, 0);
    scale.init(Scale(), Scale(), linear, 0);
    rotate.init(Rotation(), Rotation(), linear, 0);
    shear.init(Shear(), Shear(), linear, 0);
    color.init(Color(), Color(), linear, 0);
  }

  virtual void Update(glm::f32 delta) override {
    Sprite::Update(delta);
    Position(glm::vec3(translate.update(delta), 0));
    Scale(scale.update(delta));
    Rotation(rotate.update(delta));
    Shear(shear.update(delta));
    Color(color.update(delta));
  }
};

/**
* �J���[�t�B���^�[��p�X�v���C�g�N���X.
*/
struct color_filter : Sprite
{
  action<glm::f32> action;

  virtual void Update(glm::f32 delta) override {
    Sprite::Update(delta);
    glm::vec4 c = Color();
    c.w = action.update(delta);
    Color(c);
  }
};

/*
* Command���C�u�����ϐ�.
*/

Sprite rootNode; // �`�擙�̑�{�ɂȂ�X�v���C�g.
std::vector<actable_sprite> spriteBuffer; // ���ۂɕ`�悳���X�v���C�g����.
SpriteRenderer spriteRenderer; // �X�v���C�g�`��p�ϐ�.

std::unordered_map<std::string, TexturePtr> textureCache; // �ǂݍ��񂾃e�N�X�`����ێ�����ϐ�.

// �t�F�[�h�C���E�A�E�g����p�̕ϐ�.
color_filter colorFilter;
SpriteRenderer colorFilterRenderer;

/**
* �e�L�X�g�\�����.
*/
struct text_info
{
  glm::vec2 pos; // �\��������W.
  std::wstring text; // �\������e�L�X�g.
};
std::vector<text_info> textList; // �e�L�X�g���̃��X�g.
Font::Renderer fontRenderer;

std::mt19937 randomEngine; // ����������.

// ��������ϐ�.
std::string bgmFilename;
Audio::SoundPtr bgm;
float seVolume = 0.8f;
float bgmVolume = 0.8f;

} // unnamed namespace

/**
* sjis�����񂩂�utf16������𓾂�.
*/
std::wstring sjis_to_utf16(const char* p)
{
  std::wstring tmp;
  tmp.resize(strlen(p) + 1);
  mbstowcs(&tmp[0], p, tmp.size());
  return tmp;
}

/**
* utf16�����񂩂�sjis������𓾂�.
*/
std::string utf16_to_sjis(const std::wstring& p)
{
  std::string tmp;
  tmp.resize(p.size() * 2);
  wcstombs(&tmp[0], p.c_str(), tmp.size());
  return tmp;
}

/**
* �X�N���[�����W�ɑΉ�����N���b�v���W�𓾂�.
*/
glm::vec2 screen_coord_to_clip_coord(const glm::vec2& pos)
{
  const GLFWEW::Window& window = GLFWEW::Window::Instance();
  const glm::vec2 ss(window.Width() / 2, window.Height() / 2);
  return pos / ss;
}

/**
* �E�B���h�E���W�n(���㌴�_)����OpenGL���W�n(�������_)�ɕϊ�����.
*/
glm::vec2 win_to_ogl_coord(double x, double y)
{
  const GLFWEW::Window& window = GLFWEW::Window::Instance();
  return glm::vec2(x - window.Width() / 2, window.Height() / 2 - y);
}

/**
* ���C�����[�v.
*/
template<typename T>
void main_loop(T func)
{
  const glm::vec2 invColorFilterSize = glm::vec2(1, 1) / glm::vec2(colorFilter.Texture()->Width(), colorFilter.Texture()->Height());

  GLFWEW::Window& window = GLFWEW::Window::Instance();
  for (;;) {
    Audio::Engine::Get().Update();
    window.Update();
    const GamePad gamepad = window.GetGamePad();
    if (window.ShouldClose() || (gamepad.buttonDown & GamePad::ESC)) {
      quit();
      exit(0);
      break;
    }

    if (func()) {
      break;
    }

    const float deltaTime = window.DeltaTime();
    rootNode.UpdateTransform();
    rootNode.UpdateRecursive(deltaTime);
    spriteRenderer.Update(rootNode);

    const glm::vec2 windowSize(window.Width(), window.Height());

    colorFilter.Scale(windowSize * invColorFilterSize);
    colorFilter.UpdateTransform();
    colorFilter.UpdateRecursive(deltaTime);
    colorFilterRenderer.Update(colorFilter);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    spriteRenderer.Draw(windowSize);
    fontRenderer.Draw();
    colorFilterRenderer.Draw(windowSize);

    window.SwapBuffers();
  }
}

/**
* Easy�����Command���C�u�����̏�����.
*/
void initialize()
{
  CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  if (!window.Init(800, 600, "OpenGL 2D")) {
    exit(1);
  }

  if (!Audio::Engine::Get().Initialize()) {
    std::cerr << "Audio Engine�̏������Ɏ��s." << std::endl;
  }

  setlocale(LC_CTYPE, "JPN");
  Texture::Initialize();
  textureCache.reserve(1024);

  static const uint32_t planeTexData[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
  };

  spriteRenderer.Init(1024);
  spriteBuffer.resize(1024);
  for (auto&& e : spriteBuffer) {
    rootNode.AddChild(&e);
  }

  colorFilterRenderer.Init(1);
  colorFilter.Texture(Texture::Create(4, 4, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, planeTexData));
  colorFilter.Color(glm::vec4(0, 0, 0, 0));
  colorFilter.action.init(0, 0, linear, 0);

  fontRenderer.Init(1024, glm::vec2(window.Width(), window.Height()));
  fontRenderer.LoadFromFile("Res/Font/font.fnt");

  randomEngine.seed(std::random_device()());
}

/**
* Easy�����Command���C�u�����̏I��.
*/
void finalize()
{
  bgm.reset();
  rootNode = Sprite();
  spriteBuffer.clear();
  textureCache.clear();
  Texture::Finalize();

  Audio::Engine::Get().Destroy();

  CoUninitialize();
  std::cout << "Finish." << std::endl;
}

void set_text(double x, double y, const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  char tmp[1024];
  vsnprintf(tmp, sizeof(tmp), format, ap);
  va_end(ap);

  const glm::vec2 opengl_pos = win_to_ogl_coord(x, y);
  textList.push_back({ screen_coord_to_clip_coord(opengl_pos), sjis_to_utf16(tmp) });
}

void reset_all_text()
{
  textList.clear();
}

void reset_text_area(double x, double y, double width, double height)
{
  const glm::vec2 min = screen_coord_to_clip_coord(win_to_ogl_coord(x, y));
  const glm::vec2 max = screen_coord_to_clip_coord(win_to_ogl_coord(x + width, y + height));
  const auto itr = std::remove_if(textList.begin(), textList.end(), [min, max](const text_info& e) {
    return (e.pos.x >= min.x) && (e.pos.x < max.x) && (e.pos.y >= min.y) && (e.pos.y < max.y);
  });
  textList.erase(itr, textList.end());
}

void set_image(ImageNo no, double x, double y, const char* filename)
{
  if (no < 0 || no >= static_cast<int>(spriteBuffer.size())) {
    return;
  }

  auto itr = textureCache.find(filename);
  TexturePtr tex;
  if (itr != textureCache.end()) {
    tex = itr->second;
  } else {
    std::string str;
    str.reserve(1024);
    str += "Res/";
    str += filename;
    tex = Texture::LoadFromFile(str.c_str());
    if (tex) {
      textureCache.emplace(filename, tex);
    }
  }

  if (tex) {
    spriteBuffer[no].Texture(tex);
    spriteBuffer[no].Position(glm::vec3(win_to_ogl_coord(x, y), 0));
    spriteBuffer[no].Scale(glm::vec2(1, 1));
    spriteBuffer[no].Rotation(0);
    spriteBuffer[no].Shear(0);
    spriteBuffer[no].Color(glm::vec4(1, 1, 1, 1));
    spriteBuffer[no].ColorMode(BlendMode_Multiply);

    spriteBuffer[no].init_action();
  }
}

void move_image(ImageNo no, double x, double y, int easing, double seconds)
{
  auto& e = spriteBuffer[no];
  e.translate.init(e.Position(), win_to_ogl_coord(x, y), easing, seconds);
  e.Position(glm::vec3(e.translate.update(0), 0));
}

void scale_image(ImageNo no, double x, double y, int easing, double seconds)
{
  auto& e = spriteBuffer[no];
  e.scale.init(e.Scale(), glm::vec2(x, y), easing, seconds);
  e.Scale(e.scale.update(0));
}

void rotate_image(ImageNo no, double degree, int easing, double seconds)
{
  auto& e = spriteBuffer[no];
  e.rotate.init(e.Rotation(), static_cast<float>(degree) * (glm::pi<float>() / 180.0f), easing, seconds);
  e.Rotation(e.rotate.update(0));
}

void shear_image(ImageNo no, double scale, int easing, double seconds)
{
  auto& e = spriteBuffer[no];
  e.shear.init(e.Shear(), static_cast<float>(scale), easing, seconds);
  e.Shear(e.shear.update(0));
}

void color_blend_image(ImageNo no, double red, double green, double blue, double alpha, int mode, int easing, double seconds)
{
  auto& e = spriteBuffer[no];
  e.ColorMode(static_cast<BlendMode>(glm::clamp(mode, 0, 2)));
  e.color.init(e.Color(), glm::vec4(red, green, blue, alpha), easing, seconds);
  e.Color(e.color.update(0));
}

void reset_image(ImageNo no)
{
  spriteBuffer[no].Texture({});
}

void reset_all_image()
{
  for (auto& e : spriteBuffer) {
    e.Texture({});
  }
}

void fade_out(double red, double green, double blue, double seconds)
{
  colorFilter.Color(glm::vec4(red, green, blue, 0));
  colorFilter.action.init(0, 1, linear, seconds);
  wait(seconds);
}

void fade_in(double seconds)
{
  colorFilter.action.init(1, 0, linear, seconds);
  wait(seconds);
}

void wait(double seconds)
{
  fontRenderer.Color(glm::vec4(1));
  fontRenderer.MapBuffer();
  for (const auto& e : textList) {
    fontRenderer.AddString(e.pos, e.text.c_str());
  }
  fontRenderer.UnmapBuffer();

  main_loop([&seconds] {
    GLFWEW::Window& window = GLFWEW::Window::Instance();
    seconds -= window.DeltaTime();
    return seconds <= 0;
  });
}

void wait_any_key()
{
  float timer = 0;
  main_loop([&] {
    GLFWEW::Window& window = GLFWEW::Window::Instance();

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }

    timer += window.DeltaTime();
    if (timer >= 1) {
      timer -= 1;
    }
    fontRenderer.Color(timer < 0.5f ? glm::vec4(1, 1, 1, 1) : glm::vec4(0.5f, 0.5f, 0.5f, 1));
    const glm::vec2 markerPos = screen_coord_to_clip_coord(win_to_ogl_coord(760, 560));
    fontRenderer.AddString(markerPos, L"��");
    fontRenderer.UnmapBuffer();

    const GamePad gamepad = window.GetGamePad();
    return gamepad.buttons || (window.NumOfKeyPressed() > 0);
  });
}

int wait_game_key(bool trigger)
{
  fontRenderer.Color(glm::vec4(1));
  fontRenderer.MapBuffer();
  for (const auto& e : textList) {
    fontRenderer.AddString(e.pos, e.text.c_str());
  }
  fontRenderer.UnmapBuffer();

  int result;
  main_loop([trigger, &result] {
    GLFWEW::Window& window = GLFWEW::Window::Instance();
    const GamePad gamepad = window.GetGamePad();
    static const int keyMap[] = {
      GamePad::DPAD_UP,
      GamePad::DPAD_RIGHT,
      GamePad::DPAD_DOWN,
      GamePad::DPAD_LEFT,
      GamePad::START,
      GamePad::A,
      GamePad::B,
      GamePad::X,
      GamePad::Y,
      GamePad::L,
      GamePad::R,
    };
    const int buttons = trigger ? gamepad.buttonDown : gamepad.buttons;
    for (int i = 0; i < sizeof(keyMap) / sizeof(keyMap[0]); ++i) {
      if (buttons & keyMap[i]) {
        result = i;
        return true;
      }
    }
    return false;
  });
  return result;
}

int select(double x, double y, int count, const char* a, const char* b, ...)
{
  std::vector<std::wstring> selectionList;

  va_list ap;
  va_start(ap, b);
  selectionList.push_back(sjis_to_utf16(a));
  selectionList.push_back(sjis_to_utf16(b));
  for (int i = 2; i < count; ++i) {
    const char* p = va_arg(ap, const char*);
    selectionList.push_back(sjis_to_utf16(p));
  }
  va_end(ap);

  const glm::vec2 textPosOrigin = screen_coord_to_clip_coord(win_to_ogl_coord(x, y));
  int select = 0;
  float timer = 0;
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  main_loop([&] {
    const GamePad gamepad = window.GetGamePad();
    if (gamepad.buttonDown & GamePad::DPAD_UP) {
      if (--select < 0) {
        select = static_cast<int>(selectionList.size() - 1);
      }
      timer = 0;
    } else if (gamepad.buttonDown & GamePad::DPAD_DOWN) {
      if (++select >= static_cast<int>(selectionList.size())) {
        select = 0;
      }
      timer = 0;
    } else if (gamepad.buttonDown & (GamePad::A | GamePad::START)) {
      return true;
    }

    timer += window.DeltaTime();
    if (timer >= 1) {
      timer -= 1;
    }

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }

    glm::vec2 textPos(textPosOrigin);
    const float nextLineOffset = 32.0f / static_cast<float>(window.Height() / 2);
    for (int i = 0; i < static_cast<int>(selectionList.size()); ++i) {
      fontRenderer.Color(i == select && timer < 0.5f ? glm::vec4(1, 1, 1, 1) : glm::vec4(0.5f, 0.5f, 0.5f, 1));
      fontRenderer.AddString(textPos, selectionList[i].c_str());
      textPos.y -= nextLineOffset;
    }
    fontRenderer.UnmapBuffer();
    return false;
  });
  return select;
}

int select_number(double x, double y, int min, int max)
{
  const glm::vec2 textPosOrigin = screen_coord_to_clip_coord(win_to_ogl_coord(x, y));
  int select = 0;
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  float timer = 0;
  main_loop([&] {
    const GamePad gamepad = window.GetGamePad();
    const int unit = (gamepad.buttons & (GamePad::L | GamePad::R)) ? 10 : 1;
    if (gamepad.buttonDown & GamePad::DPAD_UP) {
      select -= unit;
      if (select < min) {
        select = min;
      }
      timer = 0;
    } else if (gamepad.buttonDown & GamePad::DPAD_DOWN) {
      select += unit;
      if (select > max) {
        select = max;
      }
      timer = 0;
    } else if (gamepad.buttonDown & (GamePad::A | GamePad::START)) {
      return true;
    }

    timer += window.DeltaTime();
    if (timer >= 1) {
      timer -= 1;
    }

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }

    const float nextLineOffset = 32.0f / static_cast<float>(window.Height() / 2);
    std::wstring str;
    int i = select;
    for (int j = max; j > 0; j /= 10) {
      str.push_back(L'0' + i % 10);
      i /= 10;
    }
    std::reverse(str.begin(), str.end());

    fontRenderer.Color(timer < 0.5f ? glm::vec4(1, 1, 1, 1) : glm::vec4(0.5f, 0.5f, 0.5f, 1));
    fontRenderer.AddString(textPosOrigin, str.c_str());
    fontRenderer.UnmapBuffer();
    return false;
  });
  return select;
}

void select_string(double x, double y, int max, char* buffer)
{
  enum {
    kana,
    dakuten,
    han_dakuten,
    minus,
    space,
    kana_switch,
    submit,
  };
  struct char_data {
    wchar_t str[4];
    wchar_t daku, handaku;
    int8_t x, y;
    int8_t type;
  };
  static const char_data charTable[] = {
  { L"��", 0, 0, 0, 0, kana },    { L"��", 0, 0, 1, 0, kana },    { L"��", 0, 0, 2, 0, kana },    { L"��", 0, 0, 3, 0, kana },    { L"��", 0, 0, 4, 0, kana },    { L"��", L'��', L'��', 6, 0, kana },{ L"��", L'��', L'��', 7, 0, kana },{ L"��", L'��', L'��', 8, 0, kana },{ L"��", L'��', L'��', 9, 0, kana },{ L"��", L'��', L'��', 10, 0, kana },{ L"�J", 0, 0, 12, 0, dakuten },{ L"�K", 0, 0, 13, 0, han_dakuten },
  { L"��", L'��', 0, 0, 1, kana },{ L"��", L'��', 0, 1, 1, kana },{ L"��", L'��', 0, 2, 1, kana },{ L"��", L'��', 0, 3, 1, kana },{ L"��", L'��', 0, 4, 1, kana },{ L"��", 0, 0, 6, 1, kana },        { L"��", 0, 0, 7, 1, kana },        { L"��", 0, 0, 8, 1, kana },        { L"��", 0, 0, 9, 1, kana },        { L"��", 0, 0, 10, 1, kana },        { L"�|", 0, 0, 12, 1, minus },
  { L"��", L'��', 0, 0, 2, kana },{ L"��", L'��', 0, 1, 2, kana },{ L"��", L'��', 0, 2, 2, kana },{ L"��", L'��', 0, 3, 2, kana },{ L"��", L'��', 0, 4, 2, kana },{ L"��", 0, 0, 6, 2, kana },        { L"��", 0, 0, 7, 2, kana },        { L"��", 0, 0, 8, 2, kana },        { L"��", 0, 0, 9, 2, kana },        { L"��", 0, 0, 10, 2, kana },        { L"��", 0, 0, 12, 2, space },
  { L"��", L'��', 0, 0, 3, kana },{ L"��", L'��', 0, 1, 3, kana },{ L"��", L'��', 0, 2, 3, kana },{ L"��", L'��', 0, 3, 3, kana },{ L"��", L'��', 0, 4, 3, kana },{ L"��", 0, 0, 6, 3, kana },        { L"��", 0, 0, 7, 3, kana },        { L"��", 0, 0, 8, 3, kana },        { L"��", 0, 0, 9, 3, kana },        { L"��", 0, 0, 10, 3, kana },        { L"�J�i", 0, 0, 12, 3, kana_switch },
  { L"��", L'��', 0, 0, 4, kana },{ L"��", 0, 0, 1, 4, kana},     { L"��", 0, 0, 2, 4, kana},     { L"��", 0, 0, 3, 4, kana},     { L"��", 0, 0, 4, 4, kana},     { L"��", 0, 0, 6, 4, kana },        { L"��", 0, 0, 7, 4, kana },        { L"��", 0, 0, 8, 4, kana },        { L"��", 0, 0, 9, 4, kana },        { L"��", 0, 0, 10, 4, kana },        { L"����", 0, 0, 12, 4, submit},
  };
  const glm::vec2 textPosOrigin = screen_coord_to_clip_coord(win_to_ogl_coord(x, y));
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  int select = 0;
  float timer = 0;
  bool isKatakana = false;
  std::wstring tmpBuffer;
  std::vector<char_data> tmpDataBuffer;
  tmpBuffer.reserve(max);
  tmpDataBuffer.reserve(max);
  main_loop([&] {
    static const int kana_offset = L'�A' - L'��';
    const GamePad gamepad = window.GetGamePad();
    if (gamepad.buttonDown & (GamePad::A | GamePad::START)) {
      const int type = charTable[select].type;
      if (type == submit) {
        if (!tmpBuffer.empty()) {
          const std::string tmp = utf16_to_sjis(tmpBuffer);
          std::copy_n(tmp.c_str(), tmp.size() + 1, buffer);
          return true;
        }
      } else if (type == kana_switch) {
        isKatakana = !isKatakana;
      } else if (static_cast<int>(tmpBuffer.size()) < max) {
        if (type == space) {
          tmpBuffer.push_back(L'�@');
          tmpDataBuffer.push_back(charTable[select]);
        } else if (type == minus) {
          tmpBuffer.push_back(L'�|');
          tmpDataBuffer.push_back(charTable[select]);
        } else if (type == dakuten) {
          const char_data& data = tmpDataBuffer.back();
          if (data.daku && !tmpBuffer.empty()) {
            wchar_t c = tmpBuffer.back();
            bool isKana = false;
            if (c == data.str[0] + kana_offset) {
              c -= kana_offset;
              isKana = true;
            }
            if (c == data.daku) {
              c = data.str[0];
            } else {
              c = data.daku;
            }
            if (isKana) {
              c += kana_offset;
            }
            tmpBuffer.back() = c;
          }
        } else if (type == han_dakuten) {
          const char_data& data = tmpDataBuffer.back();
          if (data.handaku && !tmpBuffer.empty()) {
            wchar_t c = tmpBuffer.back();
            bool isKana = false;
            if (c == data.str[0] + kana_offset) {
              c -= kana_offset;
              isKana = true;
            }
            if (c == data.handaku) {
              c = data.str[0];
            } else {
              c = data.handaku;
            }
            if (isKana) {
              c += L'�A' - L'��';
            }
            tmpBuffer.back() = c;
          }
        } else {
          wchar_t c = charTable[select].str[0];
          if (isKatakana) {
            c += L'�A' - L'��';
          }
          tmpBuffer.push_back(c);
          tmpDataBuffer.push_back(charTable[select]);
        }
      }
    } else if (gamepad.buttonDown & GamePad::B) {
      tmpBuffer.pop_back();
      tmpDataBuffer.pop_back();
    }
    if (gamepad.buttonDown & GamePad::DPAD_LEFT) {
      if (select < 12) {
        if (select == 0) {
          select = 11;
        } else {
          --select;
        }
      } else {
        if ((select - 12) % 11 == 0) {
          select += 10;
        } else {
          --select;
        }
      }
      timer = 0;
    } else if (gamepad.buttonDown & GamePad::DPAD_RIGHT) {
      if (select < 12) {
        if (select == 11) {
          select = 0;
        } else {
          ++select;
        }
      } else {
        if ((select - 12) % 11 == 10) {
          select -= 10;
        } else {
          ++select;
        }
      }
      timer = 0;
    }
    if (gamepad.buttonDown & GamePad::DPAD_UP) {
      if (select < 12) {
        if (select == 11) {
          select = 12 + 11 * 4 - 1;
        } else {
          select += 12 + 11 * 3;
        }
      } else if (select < 12 + 11) {
        select -= 12;
      } else {
        select -= 11;
      }
      timer = 0;
    } else if (gamepad.buttonDown & GamePad::DPAD_DOWN) {
      if (select >= 12 + 11 * 3) {
        select -= 12 + 11 * 3;
      } else if (select == 11) {
        select += 11;
      } else if (select < 12) {
        select += 12;
      } else {
        select += 11;
      }
      timer = 0;
    }

    timer += window.DeltaTime();
    if (timer >= 1) {
      timer -= 1;
    }

    fontRenderer.Color(glm::vec4(1));
    fontRenderer.MapBuffer();
    for (const auto& e : textList) {
      fontRenderer.AddString(e.pos, e.text.c_str());
    }

    std::wstring dispBuffer(tmpBuffer);
    dispBuffer.insert(dispBuffer.end(), max - tmpBuffer.size(), L'��');
    fontRenderer.AddString(textPosOrigin, dispBuffer.c_str());
  
    const float nextCharOffset = 32.0f / static_cast<float>(window.Width() / 2);
    const float nextLineOffset = 32.0f / static_cast<float>(window.Height() / 2);
    for (int i = 0; i < sizeof(charTable) / sizeof(charTable[0]); ++i) {
      glm::vec4 color(0.5f, 0.5f, 0.5f, 1);
      if (select == i && timer < 0.5f) {
        color = {1, 1, 1, 1};
      }
      fontRenderer.Color(color);
      const char_data& e = charTable[i];
      glm::vec2 textPos(textPosOrigin);
      textPos.x += e.x * nextCharOffset;
      textPos.y -= (e.y + 1) * nextLineOffset;
      if (isKatakana &&  e.str[1] == L'\0' && e.str[0] != L'�J' && e.str[0] != L'�K' && e.str[0] != L'�|') {
        wchar_t str[2];
        str[0] = e.str[0] + kana_offset;
        str[1] = L'\0';
        fontRenderer.AddString(textPos, str);
      } else {
        fontRenderer.AddString(textPos, e.str);
      }
    }
    fontRenderer.UnmapBuffer();
    return false;
  });
}

int random(int min, int max)
{
  return std::uniform_int_distribution<>(min, max)(randomEngine);
}

void quit()
{
  bgm.reset();
  hasQuitRequest = true;
  Texture::Finalize();
}

void play_sound(const char* filename)
{
  std::string str;
  str.reserve(1024);
  str += "Res/";
  str += filename;
  const std::wstring ws = sjis_to_utf16(str.c_str());
  Audio::SoundPtr p = Audio::Engine::Get().PrepareMFStream(ws.c_str());
  p->Play(Audio::Flag_None);
  p->SetVolume(seVolume);
}

void play_bgm(const char* filename)
{
  if (bgmFilename != filename || !bgm || !(bgm->GetState() & Audio::State_Playing)) {
    if (bgm) {
      bgm->Stop();
    }
    bgmFilename = filename;
    std::string str;
    str.reserve(1024);
    str += "Res/";
    str += filename;
    const std::wstring ws = sjis_to_utf16(str.c_str());
    bgm = Audio::Engine::Get().PrepareMFStream(ws.c_str());
    bgm->Play(Audio::Flag_Loop);
    bgm->SetVolume(bgmVolume);
  }
}

void stop_bgm()
{
  bgmFilename.clear();
  if (bgm) {
    bgm->Stop();
    bgm.reset();
  }
}

void set_sound_volume(double volume)
{
  seVolume = static_cast<float>(volume);
}

void set_bgm_volume(double volume)
{
  bgmVolume = static_cast<float>(volume);
  if (bgm) {
    bgm->SetVolume(bgmVolume);
  }
}