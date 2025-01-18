#include "game.hpp"
#include "log.hpp"
#include "raylib.h"

int main() {
    SetTraceLogCallback(raylib_log); // We ask raylib to use our logging system
    Log::Debug << "Debug";
    Log::Info << "Info";
    Log::Info.log("test number %d", 8);
    Game *game = new Game(1000, 1000, "HexaTTD");
    game->start();
    game->wait();
    delete game;
    return 0;
}
