#include "clay.h"
#include "gui.hpp"
#include "log.hpp"
#include "raylib.h"
#include "utils.hpp"

#include <cmath>
#include <memory.h>

#define CLAY_RECTANGLE_TO_RAYLIB_RECTANGLE(rectangle)                          \
    (Rectangle) {                                                              \
        .x = rectangle.x, .y = rectangle.y, .width = rectangle.width,          \
        .height = rectangle.height                                             \
    }
#define CLAY_COLOR_TO_RAYLIB_COLOR(color)                                      \
    (Color) {                                                                  \
        .r = (unsigned char)round(color.r),                                    \
        .g = (unsigned char)round(color.g),                                    \
        .b = (unsigned char)round(color.b), .a = (unsigned char)round(color.a) \
    }

Clay_Dimensions Gui::measure_text(Clay_StringSlice text,
                                  Clay_TextElementConfig *config,
                                  void *userData) {
    // ignore userData not used yet
    (void)userData;
    // Measure string size for Font
    auto textSize = Clay_Dimensions();

    float maxTextWidth = 0.0f;
    size_t number_of_letters = 0;
    float lineTextWidth = 0;

    float textHeight = config->fontSize;
    /* Font *fonts = (Font *)userData; */
    /* Font fontToUse = fonts[config->fontId]; */
    // Font failed to load, likely the fonts are in the wrong place relative to
    // the execution dir. RayLib ships with a default font, so we can continue
    // with that built in one.
    /* if (!fontToUse.glyphs) { */
    Font fontToUse = GetFontDefault();
    /* } */

    float scaleFactor = config->fontSize / (float)fontToUse.baseSize;
    if (config->fontId == 0) {
        config->letterSpacing = 1;
    }

    for (int i = 0; i < text.length; ++i) {
        if (text.chars[i] == '\n') {
            maxTextWidth =
                MAX(maxTextWidth, lineTextWidth + (config->letterSpacing *
                                                   (number_of_letters - 1)));
            lineTextWidth = 0;
            number_of_letters = 0;
            continue;
        }
        int index = text.chars[i] - 32;
        if (fontToUse.glyphs[index].advanceX != 0)
            lineTextWidth += fontToUse.glyphs[index].advanceX;
        else
            lineTextWidth +=
                (fontToUse.recs[index].width + fontToUse.glyphs[index].offsetX);
        number_of_letters++;
    }

    maxTextWidth = MAX(maxTextWidth, lineTextWidth + (config->letterSpacing *
                                                      (number_of_letters - 1)));

    textSize.width = maxTextWidth * scaleFactor;
    textSize.height = textHeight;

    return textSize;
}

void Gui::render(Clay_RenderCommandArray renderCommands) {
    for (int j = 0; j < renderCommands.length; j++) {
        Clay_RenderCommand *renderCommand =
            Clay_RenderCommandArray_Get(&renderCommands, j);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;
        switch (renderCommand->commandType) {
        case CLAY_RENDER_COMMAND_TYPE_TEXT: {
            Clay_TextRenderData *textData = &renderCommand->renderData.text;
            /* Font fontToUse = _font[textData->fontId]; */
            Font fontToUse = GetFontDefault();

            int strlen = textData->stringContents.length + 1;
            // Set the letterspacing for debug mode
            if (textData->fontId == 0) {
                textData->letterSpacing = 1;
            }

            if (strlen > _temp_render_buffer_len) {
                // Grow the temp buffer if we need a larger string
                if (_temp_render_buffer)
                    free(_temp_render_buffer);
                _temp_render_buffer = (char *)malloc(strlen);
                _temp_render_buffer_len = strlen;
            }

            // Raylib uses standard C strings so isn't compatible with cheap
            // slices, we need to clone the string to append null terminator
            memcpy(_temp_render_buffer, textData->stringContents.chars,
                   textData->stringContents.length);
            _temp_render_buffer[textData->stringContents.length] = '\0';
            DrawTextEx(fontToUse, _temp_render_buffer,
                       (Vector2){boundingBox.x, boundingBox.y},
                       (float)textData->fontSize,
                       (float)textData->letterSpacing,
                       CLAY_COLOR_TO_RAYLIB_COLOR(textData->textColor));

            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
            Texture2D imageTexture =
                *(Texture2D *)renderCommand->renderData.image.imageData;
            Clay_Color tintColor =
                renderCommand->renderData.image.backgroundColor;
            if (tintColor.r == 0 && tintColor.g == 0 && tintColor.b == 0 &&
                tintColor.a == 0) {
                tintColor = (Clay_Color){255, 255, 255, 255};
            }
            DrawTextureEx(imageTexture, (Vector2){boundingBox.x, boundingBox.y},
                          0, boundingBox.width / (float)imageTexture.width,
                          CLAY_COLOR_TO_RAYLIB_COLOR(tintColor));
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
            BeginScissorMode((int)roundf(boundingBox.x),
                             (int)roundf(boundingBox.y),
                             (int)roundf(boundingBox.width),
                             (int)roundf(boundingBox.height));
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
            EndScissorMode();
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
            Clay_RectangleRenderData *config =
                &renderCommand->renderData.rectangle;
            if (config->cornerRadius.topLeft > 0) {
                float radius = (config->cornerRadius.topLeft * 2) /
                               (float)((boundingBox.width > boundingBox.height)
                                           ? boundingBox.height
                                           : boundingBox.width);
                DrawRectangleRounded(
                    (Rectangle){boundingBox.x, boundingBox.y, boundingBox.width,
                                boundingBox.height},
                    radius, 8,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->backgroundColor));
            } else {
                DrawRectangle(
                    boundingBox.x, boundingBox.y, boundingBox.width,
                    boundingBox.height,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->backgroundColor));
            }
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_BORDER: {
            Clay_BorderRenderData *config = &renderCommand->renderData.border;
            // Left border
            if (config->width.left > 0) {
                DrawRectangle(
                    (int)roundf(boundingBox.x),
                    (int)roundf(boundingBox.y + config->cornerRadius.topLeft),
                    (int)config->width.left,
                    (int)roundf(boundingBox.height -
                                config->cornerRadius.topLeft -
                                config->cornerRadius.bottomLeft),
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color));
            }
            // Right border
            if (config->width.right > 0) {
                DrawRectangle(
                    (int)roundf(boundingBox.x + boundingBox.width -
                                config->width.right),
                    (int)roundf(boundingBox.y + config->cornerRadius.topRight),
                    (int)config->width.right,
                    (int)roundf(boundingBox.height -
                                config->cornerRadius.topRight -
                                config->cornerRadius.bottomRight),
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color));
            }
            // Top border
            if (config->width.top > 0) {
                DrawRectangle(
                    (int)roundf(boundingBox.x + config->cornerRadius.topLeft),
                    (int)roundf(boundingBox.y),
                    (int)roundf(boundingBox.width -
                                config->cornerRadius.topLeft -
                                config->cornerRadius.topRight),
                    (int)config->width.top,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color));
            }
            // Bottom border
            if (config->width.bottom > 0) {
                DrawRectangle((int)roundf(boundingBox.x +
                                          config->cornerRadius.bottomLeft),
                              (int)roundf(boundingBox.y + boundingBox.height -
                                          config->width.bottom),
                              (int)roundf(boundingBox.width -
                                          config->cornerRadius.bottomLeft -
                                          config->cornerRadius.bottomRight),
                              (int)config->width.bottom,
                              CLAY_COLOR_TO_RAYLIB_COLOR(config->color));
            }
            if (config->cornerRadius.topLeft > 0) {
                DrawRing(
                    (Vector2){
                        roundf(boundingBox.x + config->cornerRadius.topLeft),
                        roundf(boundingBox.y + config->cornerRadius.topLeft)},
                    roundf(config->cornerRadius.topLeft - config->width.top),
                    config->cornerRadius.topLeft, 180, 270, 10,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color));
            }
            if (config->cornerRadius.topRight > 0) {
                DrawRing(
                    (Vector2){
                        roundf(boundingBox.x + boundingBox.width -
                               config->cornerRadius.topRight),
                        roundf(boundingBox.y + config->cornerRadius.topRight)},
                    roundf(config->cornerRadius.topRight - config->width.top),
                    config->cornerRadius.topRight, 270, 360, 10,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color));
            }
            if (config->cornerRadius.bottomLeft > 0) {
                DrawRing(
                    (Vector2){
                        roundf(boundingBox.x + config->cornerRadius.bottomLeft),
                        roundf(boundingBox.y + boundingBox.height -
                               config->cornerRadius.bottomLeft)},
                    roundf(config->cornerRadius.bottomLeft - config->width.top),
                    config->cornerRadius.bottomLeft, 90, 180, 10,
                    CLAY_COLOR_TO_RAYLIB_COLOR(config->color));
            }
            if (config->cornerRadius.bottomRight > 0) {
                DrawRing((Vector2){roundf(boundingBox.x + boundingBox.width -
                                          config->cornerRadius.bottomRight),
                                   roundf(boundingBox.y + boundingBox.height -
                                          config->cornerRadius.bottomRight)},
                         roundf(config->cornerRadius.bottomRight -
                                config->width.bottom),
                         config->cornerRadius.bottomRight, 0.1, 90, 10,
                         CLAY_COLOR_TO_RAYLIB_COLOR(config->color));
            }
            break;
        }

        default: {
            Log::Error << "Error: unhandled render command.";
            exit(1);
        }
        }
    }
}
