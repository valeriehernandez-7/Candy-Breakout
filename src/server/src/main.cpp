#include "game/game.h"

int main() {
    srand(time(nullptr));
    Game game;
    game.run();
    return EXIT_SUCCESS;
}