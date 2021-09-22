#include "communication/socketserver.h"
#include "game/game.h"

SocketServer *server;
Game *game;

void *serverRun(void *) {
    try {
        server->receive();
    } catch (const char *error) {
        perror(error);
    }
    pthread_exit(nullptr);
}

int main() {
    server = new SocketServer;
    pthread_t threadServer;
    pthread_create(&threadServer, nullptr, serverRun, nullptr);
    pthread_detach(threadServer);

    game = new Game;
    while (game->getWindow().isOpen()) {
        game->run(SocketServer::playerKeyPressed);
    }

    delete server;
    delete game;
    cout << "SERVER CONNECTION CLOSED" << endl;
    return EXIT_SUCCESS;
}