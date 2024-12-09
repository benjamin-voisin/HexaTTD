#include <assert.h>
#include <cstdio>
#include <set>

#include "grid.hpp"
#include "rail.hpp"
#include "raylib.h"
#include "train.hpp"
#include "vector.hpp"
#include "gui/toggle.hpp"

const char dotfile[20] = "graph.dot";

void pp_int_vector(FILE *f, std::vector<int> v) {
    fprintf(f, "[");
    for (long unsigned i = 0; i < v.size(); ++i) {
        if (i > 0)
            fprintf(f, ", ");
        fprintf(f, "%d", v[i]);
    }
    fprintf(f, "]\n");
}

int main() {
    char *texte = (char *)malloc(1000 * sizeof(char));
    InitWindow(1000, 1000, "HexaTTD");

    Grid grid =
        Grid(layout_flat, Vector2{100, 100},
             Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2},
             -10, 10, -10, 10);

    grid.add_rail(Hex(0, 0), 1, 5, 5);
    grid.add_rail(Hex(1, -1), 2, 5, 5);
    grid.add_rail(Hex(1, -1) + Hex(1, -1), 2, 0, 5);
    grid.add_rail(Hex(0, 1), 1, 4, 5);
    grid.add_train(new Train(0));

    grid.add_station(0, "Test");

	auto debug_toggle = GuiToggleElement(grid.layout.screen_width - 200, 10, 80, 20, "debug");

    Hex last_cursor = grid.xy_to_hex(GetMouseX(), GetMouseY());
    Hex last_cursor_pers = grid.xy_to_hex(GetMouseX(), GetMouseY());

    Hex start_construct = grid.xy_to_hex(GetMouseX(), GetMouseY());

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        grid.update();
        Hex under_cursor = grid.xy_to_hex(GetMouseX(), GetMouseY());

        if (under_cursor != last_cursor)
            last_cursor_pers = last_cursor;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            start_construct = under_cursor;
        }

        // Build rails
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (last_cursor_pers.is_neighbor(start_construct) &&
                last_cursor_pers.is_neighbor(under_cursor)) {
                if ((start_construct != last_cursor_pers) &&
                    (start_construct != under_cursor)) {
                    Hex diff_src = start_construct - last_cursor_pers;
                    Hex diff_dst = under_cursor - last_cursor_pers;
                    if ((diff_src.direction() != diff_dst.direction()) &&
                        !(start_construct.is_neighbor(under_cursor))) {
                        grid.add_rail(last_cursor_pers, diff_src.direction(),
                                      diff_dst.direction(), 4);
                        start_construct = last_cursor_pers;
                    }
                }
            }
        }

        // Move the map
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = GetMouseDelta();
            grid.layout.origin.x += delta.x;
            grid.layout.origin.y += delta.y;
        }

        BeginDrawing();
        ClearBackground(DARKGREEN);
        grid.draw();

        DrawFPS(10, 10);

        grid.hightlight(under_cursor, GREEN);

        Vector pos = {(float)GetMouseX(), (float)GetMouseY()};

#ifndef NDEBUG
		debug_toggle.draw();
#endif
        if (debug_toggle.is_pressed()) {
            // Hightlight
            grid.hightlight(last_cursor_pers, BLUE);
            grid.hightlight(start_construct, BLACK);
        }

        if (grid.on_grid(under_cursor)) {
            Tile *t = grid.tile_from_hex(under_cursor);
            std::set<int> on_tile_tracks = t->get_rails_on_tile();
            std::vector<int> selected_rails = {};
            for (auto n = on_tile_tracks.begin(); n != on_tile_tracks.end();
                 ++n) {
                Rail r = grid.get_rail(*n);
                if (r.is_on_track(grid.layout, pos)) {
                    selected_rails.push_back(*n);
                }
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                for (long unsigned i = 0; i < selected_rails.size(); ++i) {
                    grid.del_rail(selected_rails[i]);
                }
            } else {
                for (long unsigned i = 0; i < selected_rails.size(); ++i) {
                    Rail r = grid.get_rail(selected_rails[i]);
                    if (!r.deleted)
                        r.draw(grid.layout, ORANGE, 1);
                }
            }
            if (debug_toggle.is_pressed()) {
                // Print selected rails on debug mode
                FILE *f = fmemopen(texte, 1000, "w");
                fprintf(f, "selected_rails= ");
                pp_int_vector(f, selected_rails);
                fprintf(f, "layout.x=%.2f\n", grid.layout.size.x);
                t->pp(f);
                fclose(f);
                DrawText(texte, 10, 40, 30, BLACK);
            }
        }
        EndDrawing();
        last_cursor = under_cursor;
    }
    grid.graph.to_dot(dotfile);
    free(texte);
    return 0;
}
