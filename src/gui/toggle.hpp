#pragma once
#include "gui.hpp"

#include <string>

class GuiToggleElement : GuiElement {
  private:
    bool _pressed;
    std::string _text;

  public:
    void draw() override;
    bool is_pressed();
    GuiToggleElement(float x, float y, float width, float height,
                     std::string text, bool init_value);
};
