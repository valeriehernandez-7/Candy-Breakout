#include "communication/socketclient.h"

SocketClient::SocketClient() {}

void SocketClient::connection() {
    descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (descriptor < 0) {
        fprintf(stderr, "ERROR: SOCKET CONNECTION FAILED\n");
    }

    info.sin_family = AF_INET;
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = ntohs(7520);
    memset(&info.sin_zero, 0, sizeof(info.sin_zero));
    cout << "SOCKET SUCCESSFULLY CREATED" << endl;

    if ((connect(descriptor, (sockaddr *) &info, (socklen_t) sizeof(info))) < 0) {
        perror("ERROR: PLAYER-SERVER CONNECTION FAILED ");
        exit(EXIT_FAILURE);
    } else {
        pthread_t thread;
        pthread_create(&thread, nullptr, SocketClient::controller, (void *) this);
        pthread_detach(thread);
        cout << "PLAYER CONNECTED" << endl;
    }
}

void *SocketClient::controller(void *obj) {
    auto *playerData = (SocketClient *) obj;

    while (true) {
        string message;
        char buffer[1024] = {0};

        while (true) {
            memset(buffer, 0, 1024);
            int bytes = recv(playerData->descriptor, buffer, 1024, 0);
            message.append(buffer, bytes);
            if (bytes <= 0) {
                cout << "PLAYER DISCONNECTED" << endl;
                close(playerData->descriptor);
                exit(EXIT_SUCCESS);
            }
            if (bytes < 1024) {
                break;
            }
        }
        cout << message << endl;
    }
}

void SocketClient::sendData(const char *msn) const {
    send(descriptor, msn, strlen(msn), 0);
}