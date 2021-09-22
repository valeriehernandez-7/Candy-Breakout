#include "communication/socketserver.h"

string SocketServer::playerKeyPressed;

SocketServer::SocketServer() {}

bool SocketServer::connection() {
    descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (descriptor < 0) {
        return false;
    }

    info.sin_family = AF_INET;
    info.sin_addr.s_addr = INADDR_ANY;
    info.sin_port = htons(7520);
    memset(&info.sin_zero, 0, sizeof(info.sin_zero));
    return true;
}

bool SocketServer::listening() {
    if ((bind(descriptor, (sockaddr *) &info, (socklen_t) sizeof(info))) < 0) {
        return false;
    }
    listen(descriptor, 4);
    return true;
}

void SocketServer::receive() {
    if (!connection()) {
        fprintf(stderr, "ERROR: SOCKET CONNECTION FAILED\n");
    }
    if (!listening()) {
        fprintf(stderr, "ERROR: SERVER IS NOT LISTENING\n");
    }

    while (true) {
        PlayerSocket playerData;
        socklen_t clientLen = sizeof(playerData.info);
        playerData.descriptor = accept(descriptor, (sockaddr *) &playerData.info, &clientLen);

        if (playerData.descriptor < 0) {
            fprintf(stderr, "ERROR: PLAYER-SERVER CONNECTION FAILED\n");
            break;
        } else {
            players.push_back(playerData.descriptor);
            pthread_t thread;
            pthread_create(&thread, nullptr, SocketServer::controller, (void *) &playerData);
            pthread_detach(thread);
            cout << "PLAYER " + to_string(playerData.descriptor) + " CONNECTED" << endl;
        }
    }
    close(descriptor);
}

void *SocketServer::controller(void *obj) {
    auto *playerData = (PlayerSocket *) obj;

    while (true) {
        string message;
        char buffer[1024] = {0};

        while (true) {
            memset(buffer, 0, 1024);
            int bytes = recv(playerData->descriptor, buffer, 1024, 0);
            message.append(buffer, bytes);
            if (bytes <= 0) {
                close(playerData->descriptor);
                cout << "PLAYER " + to_string(playerData->descriptor) + " DISCONNECTED" << endl;
                pthread_exit(nullptr);
            }
            if (bytes < 1024) {
                break;
            }
        }
        SocketServer::playerKeyPressed = message;
    }
}

void SocketServer::sendData(const char *msn) {
    for (int p = 0; p < players.size(); p++) {
        send(players[p], msn, strlen(msn), 0);
        if (send(players[p], msn, strlen(msn), 0) == -1) {
            fprintf(stderr, "ERROR: SENDING FAILED\n");
        }
    }
}