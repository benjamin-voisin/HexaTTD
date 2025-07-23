#pragma once

#include "log.hpp"

class GuiElement {
  public:
    GuiElement() {};
    virtual void draw() { Log::Error << "GuiElement parent class called"; };
	virtual ~GuiElement() { Log::Error << "GuiElement parent class called"; };
};
