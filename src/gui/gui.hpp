#pragma once

#include "clay.h"
#include "raylib.h"

class Gui {
  private:
    Font *_font;
    char *_temp_render_buffer = nullptr;
    int _temp_render_buffer_len;
    bool _is_debug;

    void render(Clay_RenderCommandArray);
    static Clay_Dimensions measure_text(Clay_StringSlice text,
                                        Clay_TextElementConfig *config,
                                        void *userData);

  public:
    Gui(float width, float heigth);
    ~Gui();
    void draw();
    void togle_debug();
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
