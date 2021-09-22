#ifndef CLIENT_SOCKETCLIENT_H
#define CLIENT_SOCKETCLIENT_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

class SocketClient {
public:
    SocketClient();

    void connection();

    void sendData(const char *msn) const;

private:

    static void *controller(void *obj);

    int descriptor;
    sockaddr_in info;
};

#endif //CLIENT_SOCKETCLIENT_H