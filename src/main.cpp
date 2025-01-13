#include "game.hpp"
#include "log.hpp"
#include "raylib.h"

// We need to define theese at first, in global scope
std::mutex Log::_lock;
Log *Log::_logPtr = Log::Get();

int main() {
    SetTraceLogCallback(raylib_log); // We ask raylib to use our logging system
    Game *game = new Game(1000, 1000, "HexaTTD");
    Log::Get()->print_log_level();
    Log::Get()->set_log_level(LOG_DEBUG);
    Log::Get()->print_log_level();
    game->start();
    game->wait();
    delete game;
    return 0;
}
