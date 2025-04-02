// This file will contain the implementation of Raygui, and only this file
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#define CLAY_IMPLEMENTATION
#include "clay.h"
#undef CLAY_IMPLEMENTATION
#pragma GCC diagnostic pop

#include "gui.hpp"
#include "log.hpp"
#include "raylib.h"

const Clay_Color COLOR_LIGHT = (Clay_Color){224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color){168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color){225, 138, 50, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    Log::Error << errorData.errorText.chars;
}
Gui::Gui(float width, float height) : _is_debug{false} {
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(
        clayRequiredMemory, malloc(clayRequiredMemory));
    Clay_Initialize(
        clayMemory, (Clay_Dimensions){.width = width, .height = height},
        (Clay_ErrorHandler){HandleClayErrors,
                            nullptr}); // This final argument is new since the
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
    Clay_SetLayoutDimensions((Clay_Dimensions){
        .width = (float)GetScreenWidth(), .height = (float)GetScreenHeight()});

    Vector2 mousePosition = GetMousePosition();
    Vector2 scrollDelta = GetMouseWheelMoveV();
    Clay_SetPointerState((Clay_Vector2){mousePosition.x, mousePosition.y},
                         IsMouseButtonDown(0));
    Clay_UpdateScrollContainers(
        true, (Clay_Vector2){scrollDelta.x, scrollDelta.y}, GetFrameTime());

    int fps = GetFPS();
    auto text = (Clay_String){.length = 6, .chars = TextFormat("%2i FPS", fps)};
    Clay_Color color = COLOR_LIGHT;

    if ((fps < 30) && (fps >= 15))
        color = COLOR_ORANGE; // Warning FPS
    else if (fps < 15)
        color = COLOR_RED; // Low FPS
    Clay_BeginLayout();

    // An example of laying out a UI with a fixed width sidebar and flexible
    // width main content Parent element with 8px of padding
    CLAY({.layout = {.padding = CLAY_PADDING_ALL(8)}}) {
        // Child element 1
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({.textColor = color,
                                          .fontSize = 24,
                                          .letterSpacing = 3}));
    }
    render(Clay_EndLayout());
}

GuiElement::GuiElement(float x, float y, float width, float heigt)
    : _x{x}, _y{y}, _width{width}, _height{heigt} {}

void GuiElement::draw() {}
