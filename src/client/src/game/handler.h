#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "SFML/Window.hpp"
#include "communication/socketclient.h"

using namespace std;
using namespace sf;

class Handler {
public:
    static string keyManager();
};

#endif //CLIENT_HANDLER_H