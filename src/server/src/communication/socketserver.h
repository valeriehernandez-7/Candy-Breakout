#ifndef SERVER_SOCKETSERVER_H
#define SERVER_SOCKETSERVER_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

struct PlayerSocket {
    int descriptor;
    sockaddr_in info;
};

class SocketServer {
public:
    SocketServer();

    static string playerKeyPressed;

    void receive();

    void sendData(const char *msn);

private:
    int descriptor;
    sockaddr_in info;
    vector<int> players;

    bool connection();

    bool listening();

    static void *controller(void *obj);
};

#endif //SERVER_SOCKETSERVER_H