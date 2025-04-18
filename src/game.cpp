#include "game.hpp"
#include "raylib.h"

void Game::update() {

    auto target = std::chrono::milliseconds(16);
    auto target_fast = std::chrono::milliseconds(1);

    while (_grid.is_running()) {
        _grid.update();

        if (_is_fast) {
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

void Game::set_fullscreen() {
    auto display = GetCurrentMonitor();
    if (!IsWindowFullscreen()) {
        SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        ToggleFullscreen();
    }
}

void Game::draw() {

    Hex last_cursor = _grid.xy_to_hex(GetMouseX(), GetMouseY());
    Hex last_cursor_pers = _grid.xy_to_hex(GetMouseX(), GetMouseY());

    Hex start_construct = _grid.xy_to_hex(GetMouseX(), GetMouseY());

    char *texte = static_cast<char *>(calloc(1000, sizeof(char)));
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGREEN);
        // Move the map
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = GetMouseDelta();
            _grid.layout.origin.x += delta.x;
            _grid.layout.origin.y += delta.y;
        }

        // Change speed of the game
        if (IsKeyDown(KEY_TAB)) {
            _is_fast = true;
        } else {
            _is_fast = false;
        }

#ifdef DEBUG
        auto char_pressed = GetCharPressed();
        if (char_pressed == 'd') {
            _gui.togle_debug();
        }
#endif // DEBUG
       // Update the zoom level
        _grid.update_zoom(WHEEL_FACTOR, true);

        // Draw the main thing
        _grid.draw();
        _gui.draw();

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

        last_cursor = under_cursor;
        EndDrawing();
    }
    free(texte);
}

Game::Game(int width, int height, std::string name)
    : _grid{Grid(layout_flat, Vector2{100, 100},
                 Vector2{static_cast<float>(width) / 2,
                         static_cast<float>(height) / 2},
                 -10, 10, -10, 10)},
      _name{name},
      _gui{Gui(static_cast<float>(width), static_cast<float>(height))},
      _start_sema{0} {
    _is_fast = false;
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
    _update_thread = std::thread([this]() {
        this->_start_sema.acquire();
        update();
    });
    _draw_thread = std::thread([this]() {
        InitWindow(1000, 1000, this->_name.c_str());
        SetTargetFPS(144);
        set_fullscreen();
        _start_sema.release();
        draw();
        this->_grid.stop();
    });
}
#endif

void Game::wait() {
    _draw_thread.join();
    _update_thread.join();
}
