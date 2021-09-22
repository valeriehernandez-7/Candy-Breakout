#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

/*!
 * \include communication/socketclient.h
 * \include SFML/Window.hpp
 */
#include "communication/socketclient.h"
#include "SFML/Window.hpp"

/*!
 * \namespace std General-purpose polymorphic function wrapper.
 * \namespace sf Simple and Fast Multimedia Library provides a simple interface to the various components of your PC.
 */
using namespace std;
using namespace sf;

/*! \class Handler
 *  \brief Keyboard event handler received by the client.
 */
class Handler {
public:
    /*! \fn static string keyManager()
     *  \brief Key pressed identification method.
     *  Returns the type of key pressed by the client at the time of execution.
     *  In case of not pressing one of the keys the client will send an empty case.
     *  \return a string with the identifier of the key pressed.
     *  \example handler.cpp
     */
    static string keyManager();
};

#endif //CLIENT_HANDLER_H