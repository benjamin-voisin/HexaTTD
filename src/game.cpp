#include "game.hpp"
#include "raylib.h"

void Game::update() {

    auto target = std::chrono::milliseconds(16);
    auto target_fast = std::chrono::milliseconds(1);

    while (_grid.is_running()) {
        _grid.update();

        if (IsKeyDown(KEY_TAB)) {
            std::this_thread::sleep_for(target_fast);
        } else {
            std::this_thread::sleep_for(target);
        }
    }
}

void pp_int_rail_vector(Grid *g, FILE *f, std::vector<int> v) {
    fprintf(f, "[");
    for (long unsigned i = 0; i < v.size(); ++i) {
        if (i > 0)
            fprintf(f, ", ");
        g->get_rail(v[i])->pp(f);
    }
    fprintf(f, "]\n");
};

void Game::draw() {

    Hex last_cursor = _grid.xy_to_hex(GetMouseX(), GetMouseY());
    Hex last_cursor_pers = _grid.xy_to_hex(GetMouseX(), GetMouseY());

    Hex start_construct = _grid.xy_to_hex(GetMouseX(), GetMouseY());

    char *texte = (char *)calloc(1000, sizeof(char));
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGREEN);
        // Move the map
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = GetMouseDelta();
            _grid.layout.origin.x += delta.x;
            _grid.layout.origin.y += delta.y;
        }
        // Update the zoom level
        _grid.update_zoom(WHEEL_FACTOR, true);

        // Draw the main thing
        _grid.draw();

        // Draw hightlighted hex
        Hex under_cursor = _grid.xy_to_hex(GetMouseX(), GetMouseY());

        // Draw hightlighted rails
        Vector pos = {(float)GetMouseX(), (float)GetMouseY()};
        if (_grid.on_grid(under_cursor)) {
            Tile *t = _grid.tile_from_hex(under_cursor);
            std::set<int> on_tile_tracks = t->get_rails_on_tile();
            std::vector<int> selected_rails = {};
            for (auto n = on_tile_tracks.begin(); n != on_tile_tracks.end();
                 ++n) {
                Rail *r = _grid.get_rail(*n);
                if (r->is_on_track(&_grid.layout, pos)) {
                    selected_rails.push_back(*n);
                }
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                for (long unsigned i = 0; i < selected_rails.size(); ++i) {
                    _grid.del_rail(selected_rails[i]);
                }
            } else {
                for (long unsigned i = 0; i < selected_rails.size(); ++i) {
                    Rail *r = _grid.get_rail(selected_rails[i]);
                    if (!r->deleted)
                        r->draw(&_grid.layout, ORANGE, 1);
                }
            }
            if (_debug_toggle.is_pressed()) {
                // Print selected rails on debug mode
                FILE *f = fmemopen(texte, 1000, "w");
                fprintf(f, "selected_rails= ");
                pp_int_rail_vector(&_grid, f, selected_rails);
                fprintf(f, "layout.x=%.2f\n", _grid.layout.size.x);
                t->pp(f);
                fclose(f);
                DrawText(texte, 10, 40, 30, BLACK);
            }
        }

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
                        _grid.add_rail(last_cursor_pers, diff_src.direction(),
                                       diff_dst.direction(), 4);
                        start_construct = last_cursor_pers;
                    }
                }
            }
        }

        _grid.hightlight(under_cursor, GREEN);

        DrawFPS(10, 10);

        _debug_toggle.draw();
        /* if (debug_toggle.is_pressed()) { */
        /* // Hightlight */
        /* grid.hightlight(last_cursor_pers, BLUE); */
        /* grid.hightlight(start_construct, BLACK); */
        /* } */

        last_cursor = under_cursor;
        EndDrawing();
    }
    free(texte);
}

Game::Game(int width, int height, std::string name)
    : _grid{Grid(layout_flat, Vector2{100, 100},
                 Vector2{(float)width / 2, (float)height / 2}, -10, 10, -10,
                 10)},
      _name{name},
      _debug_toggle{GuiToggleElement(width - 200, 10, 80, 20, "debug", false)} {
    _grid.add_rail(Hex(0, 0), 1, 5, 5);
    _grid.add_rail(Hex(1, -1), 2, 5, 5);
    _grid.add_rail(Hex(1, -1) + Hex(1, -1), 2, 0, 5);
    _grid.add_rail(Hex(0, 1), 1, 4, 5);
    _grid.add_train(0, 6);

    _grid.add_station(0, "Test");
}

#ifdef PLATFORM_WEB
void Game::start() {
    _update_thread = std::thread([this]() { update(); });
    InitWindow(1920, 1080, this->_name.c_str());
    SetTargetFPS(60);
    draw();
    _grid.stop();
}
#else
void Game::start() {
    _update_thread = std::thread([this]() { update(); });
    _draw_thread = std::thread([this]() {
        InitWindow(1000, 1000, this->_name.c_str());
        SetTargetFPS(60);
        draw();
        this->_grid.stop();
    });
}
#endif

void Game::wait() {
    _draw_thread.join();
    _update_thread.join();
}
