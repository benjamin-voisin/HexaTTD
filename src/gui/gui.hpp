#pragma once

#include "clay.h"
#include "raylib.h"
#include "settings.hpp"
#include "jukebox.hpp"

struct button_pressed_data {
	Settings *settings;
	Jukebox *jukebox;
};


class Gui {
  private:
    Font *_font;
    char *_temp_render_buffer = nullptr;
    int _temp_render_buffer_len;

    Settings *_settings;
	Jukebox *_jukebox;

    void render(Clay_RenderCommandArray);
    static Clay_Dimensions measure_text(Clay_StringSlice text,
                                        Clay_TextElementConfig *config,
                                        void *userData);
    void draw_game();
    void draw_menu();
	button_pressed_data _button_pressed_data;

  public:
    Gui(float width, float heigth, Settings *settings, Jukebox *jukebox);
    ~Gui();
    void draw();
};

class GuiElement {
  protected:
    float _x;
    float _y;
    float _width;
    float _height;

  public:
    GuiElement(float x, float y, float width, float height);
    virtual void draw();
};
