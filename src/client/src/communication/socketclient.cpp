/*!
 * \headerfile socketclient.h "communication/socketclient.h"
 */
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
        pthread_t thread; /*!< Thread to handle client events. */
        pthread_create(&thread, nullptr, SocketClient::controller, (void *) this);
        pthread_detach(thread);
        cout << "PLAYER CONNECTED" << endl;
    }
}

void *SocketClient::controller(void *obj) {
    auto *playerData = (SocketClient *) obj; /*!< Reference to the attributes of the SocketClient Class. */

    while (true) {
        string message; /*!< Message received from the server. */
        char buffer[1024] = {0}; /*!< Holds control and states information for the socket layer. */

        while (true) {
            memset(buffer, 0, 1024);
            int bytes = recv(playerData->descriptor, buffer, 1024, 0); /*!< Bytes received from the server. */
            message.append(buffer, bytes);
            if (bytes <= 0) {
                fprintf(stderr, "ERROR: SERVER CONNECTION CLOSED\n");
                close(playerData->descriptor);
                exit(EXIT_FAILURE);
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
    if (send(descriptor, msn, strlen(msn), 0) == -1) {
        fprintf(stderr, "ERROR: SENDING FAILED\n");
    }
}