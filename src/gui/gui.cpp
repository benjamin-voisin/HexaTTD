// This file will contain the implementation of Raygui, and only this file
#include <memory>
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
const Clay_Color COLOR_BLUE = {28, 66, 168, 255};
const Clay_Color COLOR_GREEN = {66, 168, 28, 255};
const Clay_Color COLOR_ORANGE = {225, 138, 50, 255};
const Clay_Color COLOR_BLACK = {0, 0, 0, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    Log::Error << errorData.errorText.chars;
}
Gui::Gui(float width, float height, Settings *settings, Jukebox *jukebox) : _settings{settings}, _jukebox{jukebox} {
	_custom_elements["MASTER VOLUME SLIDER"] = new Slider("MASTER VOLUME SLIDER");
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(
        clayRequiredMemory, malloc(clayRequiredMemory));
    Clay_Initialize(
        clayMemory, {.width = width, .height = height},
        {HandleClayErrors, nullptr}); // This final argument is new since the
                                      // video was published
#ifdef DEBUG
	Clay_SetDebugModeEnabled(true);
#endif
    /* Font fonts[1]; */
    /* fonts[FONT_ID_BODY_16] = LoadFontEx("resources/Roboto-Regular.ttf", 48,
     * 0, 400); */
    /* SetTextureFilter(fonts[FONT_ID_BODY_16].texture,
     * TEXTURE_FILTER_BILINEAR); */
    Clay_SetMeasureTextFunction(measure_text, _font);
	_button_pressed_data.jukebox = _jukebox;
	_button_pressed_data.settings = _settings;
}

void HandleButtonInteraction(Clay_ElementId elementId,
                             Clay_PointerData pointerInfo, intptr_t userData) {
	button_pressed_data* args = reinterpret_cast<button_pressed_data*>(userData);
    if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
		args->jukebox->play_sound("click");
        if (elementId.id == CLAY_ID("DEBUG_BUTTON").id) {
            args->settings->toggle_debug();
        }
        if (elementId.id == CLAY_ID("EXIT GAME BUTTON").id) {
			args->settings->set_state(State::Quit);
        }
        if (elementId.id == CLAY_ID("NEW GAME BUTTON").id) {
			args->settings->set_state(State::NewGame);
        }
		if (elementId.id == CLAY_ID("SETTINGS BUTTON").id) {
			args->settings->set_state(State::Settings);
		}
		if (elementId.id == CLAY_ID("BACK SETTINGS BUTTON").id) {
			args->settings->set_state(State::Menu);
		}
    }
}

Gui::~Gui() { free(_temp_render_buffer); }

void Gui::draw_game() {
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
        // An orange button that turns blue when hovered
        CLAY({.id = CLAY_ID("DEBUG_BUTTON")}) {
            Clay_OnHover(HandleButtonInteraction,
                         reinterpret_cast<intptr_t>(&_button_pressed_data));
            CLAY_TEXT(
                _settings->is_debug() ? CLAY_STRING("Debug")
                                      : CLAY_STRING("No debug"),
                CLAY_TEXT_CONFIG(
                    {.textColor = Clay_Hovered() ? COLOR_RED : COLOR_LIGHT,
                     .fontSize = 25,
                     .letterSpacing = 3}));
        }
    }
#pragma GCC diagnostic pop
    render(Clay_EndLayout());
}

void Gui::draw_settings() {
    Clay_BeginLayout();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    CLAY({.id = CLAY_ID("OuterContainer"),
          .layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                     .childAlignment = {.x = CLAY_ALIGN_X_CENTER,
                                        .y = CLAY_ALIGN_Y_CENTER}},
          .backgroundColor = {100, 100, 100, 150}},) {
		CLAY({.id = CLAY_ID("SETTINGS WINDOW"),
				.layout = {.sizing = {.width = CLAY_SIZING_PERCENT(0.7), .height = CLAY_SIZING_PERCENT(0.7)},
					.padding = CLAY_PADDING_ALL(20),
					.childGap = 10,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
				},
				.backgroundColor = {150, 150, 150, 255},
				.cornerRadius = {32,32,32,32},
				.clip = {.vertical = true, .childOffset = Clay_GetScrollOffset()},
				}) {
			CLAY({.layout = {.sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIT(0)}, .childGap = 10, .layoutDirection = CLAY_LEFT_TO_RIGHT}}) {
				CLAY_TEXT(CLAY_STRING("Master volume"),
						CLAY_TEXT_CONFIG({.textColor = COLOR_BLACK, .fontSize = 30}));
				CLAY({.layout = {.sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIT(0)}}}) {};
				CLAY({ .id = CLAY_ID("MASTER VOLUME SLIDER"), .layout {.sizing = {.width = CLAY_SIZING_PERCENT(0.5), .height = CLAY_SIZING_GROW(0)}}, .custom = { .customData = (_custom_elements["MASTER VOLUME SLIDER"]) } }) {
				}
			}
            CLAY({.id = CLAY_ID("EXIT GAME BUTTON"),
                  .layout = {.padding = CLAY_PADDING_ALL(15)},
                  .backgroundColor = {200, 200, 200, 255},
                  .cornerRadius = {12, 12, 12, 12}}) {
                Clay_OnHover(HandleButtonInteraction,
                             reinterpret_cast<intptr_t>(&_button_pressed_data));
                CLAY_TEXT(
                    CLAY_STRING("Exit"),
                    CLAY_TEXT_CONFIG(
                        {.textColor = Clay_Hovered() ? COLOR_RED : COLOR_BLACK,
                         .fontSize = 50}));
            }
            CLAY({.id = CLAY_ID("BACK SETTINGS BUTTON"),
                  .layout = {.padding = CLAY_PADDING_ALL(15)},
                  .backgroundColor = {200, 200, 200, 255},
                  .cornerRadius = {12, 12, 12, 12}}) {
                Clay_OnHover(HandleButtonInteraction,
                             reinterpret_cast<intptr_t>(&_button_pressed_data));
                CLAY_TEXT(
                    CLAY_STRING("Back"),
                    CLAY_TEXT_CONFIG(
                        {.textColor = Clay_Hovered() ? COLOR_RED : COLOR_BLACK,
                         .fontSize = 50}));
            }
		}
	}
	render(Clay_EndLayout());
}

void Gui::draw_menu() {
    Clay_BeginLayout();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    CLAY({.id = CLAY_ID("OuterContainer"),
          .layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                     .childAlignment = {.x = CLAY_ALIGN_X_CENTER,
                                        .y = CLAY_ALIGN_Y_CENTER}},
          .backgroundColor = {100, 100, 100, 150}}) {
        CLAY({
            .layout =
                {
                    .sizing = {CLAY_SIZING_FIT(0), CLAY_SIZING_FIT(0)},
                    .padding = CLAY_PADDING_ALL(1),
                    .childGap = 50,
                    .childAlignment = {.x = CLAY_ALIGN_X_CENTER},
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
        }) {
            CLAY({.id = CLAY_ID("NEW GAME BUTTON"),
                  .layout = {.padding = CLAY_PADDING_ALL(15)},
                  .backgroundColor = {200, 200, 200, 255},
                  .cornerRadius = {12, 12, 12, 12}}) {
                Clay_OnHover(HandleButtonInteraction,
                             reinterpret_cast<intptr_t>(&_button_pressed_data));
                CLAY_TEXT(
                    CLAY_STRING("New game"),
                    CLAY_TEXT_CONFIG(
                        {.textColor = Clay_Hovered() ? COLOR_RED : COLOR_BLACK,
                         .fontSize = 50}));
            }
            CLAY({.id = CLAY_ID("SETTINGS BUTTON"),
                  .layout = {.padding = CLAY_PADDING_ALL(15)},
                  .backgroundColor = {200, 200, 200, 255},
                  .cornerRadius = {12, 12, 12, 12}}) {
				Clay_OnHover(HandleButtonInteraction,
						reinterpret_cast<intptr_t>(&_button_pressed_data));
                CLAY_TEXT(
                    CLAY_STRING("Settings"),
                    CLAY_TEXT_CONFIG(
                        {.textColor = Clay_Hovered() ? COLOR_RED : COLOR_BLACK,
                         .fontSize = 50}));
            }
            CLAY({.id = CLAY_ID("EXIT GAME BUTTON"),
                  .layout = {.padding = CLAY_PADDING_ALL(15)},
                  .backgroundColor = {200, 200, 200, 255},
                  .cornerRadius = {12, 12, 12, 12}}) {
                Clay_OnHover(HandleButtonInteraction,
                             reinterpret_cast<intptr_t>(&_button_pressed_data));
                CLAY_TEXT(
                    CLAY_STRING("Exit"),
                    CLAY_TEXT_CONFIG(
                        {.textColor = Clay_Hovered() ? COLOR_RED : COLOR_BLACK,
                         .fontSize = 50}));
            }
        }
    }
#pragma GCC diagnostic pop
    render(Clay_EndLayout());
}

void Gui::draw() {
    Clay_SetLayoutDimensions({.width = static_cast<float>(GetScreenWidth()),
                              .height = static_cast<float>(GetScreenHeight())});

    Vector2 mousePosition = GetMousePosition();
    Vector2 scrollDelta = GetMouseWheelMoveV();
    Clay_SetPointerState({mousePosition.x, mousePosition.y},
                         IsMouseButtonDown(0));
    Clay_UpdateScrollContainers(true, {scrollDelta.x, scrollDelta.y},
                                GetFrameTime());

    switch (_settings->get_state()) {
    case State::Menu:
        draw_menu();
        break;
    case State::Game:
        draw_game();
        break;
	case State::Settings:
		draw_settings();
		break;
    default:
        break;
    }
}

// GuiElement::GuiElement(float x, float y, float width, float heigt)
//     : _x{x}, _y{y}, _width{width}, _height{heigt} {}

// void GuiElement::draw() {}
