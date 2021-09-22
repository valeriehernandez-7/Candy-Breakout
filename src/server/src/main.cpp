/*!
 * \include communication/socketserver.h
 * \include game/game.h
 */
#include "communication/socketserver.h"
#include "game/game.h"

SocketServer *server; /*!< Pointer to SocketServer Class. */
Game *game; /*!< Pointer to Game Class. */

/*! \fn void *serverRun(void *)
 *  \brief Thread controller.
 *  It calls the method that creates the server thread, if the method does not work it terminate calling thread.
 *  \exception *error parameter is a problem during thread execution.
 *  \return void pthread_exit()
 */
void *serverRun(void *) {
    try {
        server->receive();
    } catch (const char *error) {
        perror(error);
    }
    pthread_exit(nullptr);
}

/*! \fn int main()
 *  \brief Server execution.
 *  Creates a new pointer to the SocketServer Class, generates the thread that will handle server-side communication.
 *  As long as the game window is open the program will received the information obtained from the client through the SocketServer Class.
 *  \return void exit(EXIT_SUCCESS)
 */
int main() {
    server = new SocketServer;
    pthread_t threadServer;  /*!< Thread to handle server-side communication. */
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