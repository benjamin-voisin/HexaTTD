#pragma once

#include "clay.h"

class GuiElement {
	public:
		Clay_ElementId id;
		GuiElement();
		virtual void draw();
		virtual ~GuiElement();

		enum {
			MASTER_VOLUME_SLIDER,
			NUMBER,
		};
};
