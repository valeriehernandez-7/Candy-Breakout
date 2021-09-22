/*!
 * \include communication/socketclient.h
 * \include game/handler.h
 */
#include "communication/socketclient.h"
#include "game/handler.h"

SocketClient *player;  /*!< Pointer to SocketClient Class. */
Handler *handler;  /*!< Pointer to Handler Class. */

/*! \fn void *playerRun(void *)
 *  \brief Thread controller.
 *  It calls the method that creates the client thread, if the method does not work it terminate calling thread.
 *  \exception *error parameter is a problem during thread execution.
 *  \return void pthread_exit()
 */
void *playerRun(void *) {
    try {
        player->connection();
    } catch (const char *error) {
        perror(error);
    }
    pthread_exit(nullptr);
}

/*! \fn int main()
 *  \brief Client execution.
 *  Creates a new pointer to the SocketClient class, generates the thread that will handle client-side communication.
 *  As long as the client socket exists the program will send the information obtained from the Handler class.
 *  \return void exit(EXIT_SUCCESS)
 */
int main() {
    player = new SocketClient;
    pthread_t threadPlayer;  /*!< Thread to handle client-side communication. */
    pthread_create(&threadPlayer, nullptr, playerRun, nullptr);
    pthread_detach(threadPlayer);

    while (player) {
        string key; /*!< Player pressed key. */
        key = handler->keyManager();

        if (key == "<Escape>") {
            player->sendData(key.c_str());
            delete player;
            cout << "PLAYER DISCONNECTED" << endl;
            exit(EXIT_SUCCESS);
        }
        player->sendData(key.c_str());
    }
}