#include "communication/socketclient.h"
#include "game/handler.h"

SocketClient *player;

void *playerRun(void *) {
    try {
        player->connection();
    } catch (const char *error) {
        perror(error);
    }
    pthread_exit(nullptr);
}

int main() {
    player = new SocketClient;
    pthread_t threadPlayer;
    pthread_create(&threadPlayer, nullptr, playerRun, nullptr);
    pthread_detach(threadPlayer);

    Handler handler;

    while (player) {
        string key;
        key = handler.keyManager();

        if (key == "<Escape>") {
            player->sendData(key.c_str());
            pthread_exit(nullptr);
        }

        player->sendData(key.c_str());
    }

    delete player;
    exit(EXIT_SUCCESS);
}