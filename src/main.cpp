#include "game.hpp"

/* const char dotfile[20] = "graph.dot"; */

/* void pp_int_rail_vector(Grid *g, FILE *f, std::vector<int> v) { */
/*     fprintf(f, "["); */
/*     for (long unsigned i = 0; i < v.size(); ++i) { */
/*         if (i > 0) */
/*             fprintf(f, ", "); */
/*         g->get_rail(v[i])->pp(f); */
/*     } */
/*     fprintf(f, "]\n"); */
/* }; */

int main() {
    Game *game = new Game(1000, 1000, "HexaTTD");
    game->start();
    game->wait();
    delete game;
    return 0;
}
