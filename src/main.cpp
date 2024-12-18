#include "game.hpp"

int main() {
    Game *game = new Game(1000, 1000, "HexaTTD");
    game->start();
    game->wait();
    delete game;
    return 0;
}
