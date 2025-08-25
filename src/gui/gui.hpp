#pragma once

#include "clay.h"
#include "raylib.h"
#include "settings.hpp"
#include "jukebox.hpp"

#include "slider.hpp"

struct button_pressed_data {
	Settings *settings;
	Jukebox *jukebox;
	void *element;
};


namespace CustomElements {
}

class Gui {
  private:
    Font *_font;
    char *_temp_render_buffer = nullptr;
    int _temp_render_buffer_len;

    Settings *_settings;
	Jukebox *_jukebox;

	std::vector<std::unique_ptr<GuiElement>> _custom_elements;
	std::vector<std::unique_ptr<button_pressed_data>> _custom_elements_data;

    void render(Clay_RenderCommandArray);
    static Clay_Dimensions measure_text(Clay_StringSlice text,
                                        Clay_TextElementConfig *config,
                                        void *userData);
    void draw_game();
    void draw_menu();
	void draw_settings();
	button_pressed_data _button_pressed_data;

  public:
    Gui(float width, float heigth, Settings *settings, Jukebox *jukebox);
    ~Gui();
    void draw();
};

