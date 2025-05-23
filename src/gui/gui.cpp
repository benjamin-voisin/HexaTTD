// This file will contain the implementation of Raygui, and only this file
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#define CLAY_IMPLEMENTATION
#include "clay.h"
#undef CLAY_IMPLEMENTATION
#pragma GCC diagnostic pop

#include <cstring>

#include "gui.hpp"
#include "log.hpp"
#include "raylib.h"

const Clay_Color COLOR_LIGHT = {224, 215, 210, 255};
const Clay_Color COLOR_RED = {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = {225, 138, 50, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    Log::Error << errorData.errorText.chars;
}
Gui::Gui(float width, float height) : _is_debug{false} {
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(
        clayRequiredMemory, malloc(clayRequiredMemory));
    Clay_Initialize(
        clayMemory, {.width = width, .height = height},
        {HandleClayErrors, nullptr}); // This final argument is new since the
                                      // video was published
    /* Font fonts[1]; */
    /* fonts[FONT_ID_BODY_16] = LoadFontEx("resources/Roboto-Regular.ttf", 48,
     * 0, 400); */
    /* SetTextureFilter(fonts[FONT_ID_BODY_16].texture,
     * TEXTURE_FILTER_BILINEAR); */
    Clay_SetMeasureTextFunction(measure_text, _font);
}

void Gui::togle_debug() {
    _is_debug = !_is_debug;
    Clay_SetDebugModeEnabled(_is_debug);
}

Gui::~Gui() { free(_temp_render_buffer); }

void Gui::draw() {
    Clay_SetLayoutDimensions({.width = static_cast<float>(GetScreenWidth()),
                              .height = static_cast<float>(GetScreenHeight())});

    Vector2 mousePosition = GetMousePosition();
    Vector2 scrollDelta = GetMouseWheelMoveV();
    Clay_SetPointerState({mousePosition.x, mousePosition.y},
                         IsMouseButtonDown(0));
    Clay_UpdateScrollContainers(true, {scrollDelta.x, scrollDelta.y},
                                GetFrameTime());

    int fps = GetFPS();
    auto text_field = TextFormat("%i FPS", fps);

    auto text =
        (Clay_String){.isStaticallyAllocated = false,
                      .length = static_cast<int32_t>(strlen(text_field)),
                      .chars = text_field};
    Clay_Color color = COLOR_LIGHT;

    if ((fps < 30) && (fps >= 15))
        color = COLOR_ORANGE; // Warning FPS
    else if (fps < 15)
        color = COLOR_RED; // Low FPS
    Clay_BeginLayout();

    // Remove warning visible in clay things
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    CLAY({.layout = {.sizing = {CLAY_SIZING_FIT(0), CLAY_SIZING_FIT(0)},
                     .padding = CLAY_PADDING_ALL(8)},
          .backgroundColor = COLOR_ORANGE,
          .cornerRadius = {12, 12, 12, 12}}) {
        // Child element 1
        CLAY({.layout = {.padding = CLAY_PADDING_ALL(0)}}) {
            CLAY_TEXT(text, CLAY_TEXT_CONFIG({.textColor = color,
                                              .fontSize = 25,
                                              .letterSpacing = 3}));
        }
    }
#pragma GCC diagnostic pop
    render(Clay_EndLayout());
}

GuiElement::GuiElement(float x, float y, float width, float heigt)
    : _x{x}, _y{y}, _width{width}, _height{heigt} {}

void GuiElement::draw() {}
