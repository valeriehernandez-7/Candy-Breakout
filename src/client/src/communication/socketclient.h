#ifndef CLIENT_SOCKETCLIENT_H
#define CLIENT_SOCKETCLIENT_H

/*!
 * \include arpa/inet.h
 * \include sys/socket.h
 * \include sys/types.h
 * \include netdb.h
 * \include unistd.h
 * \include pthread.h
 * \include cstring
 * \include string
 * \include iostream
 */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <string>
#include <iostream>

/*!
 * \namespace std General-purpose polymorphic function wrapper.
 */
using namespace std;

/*! \class SocketClient
 *  \brief Client side communication.
 */
class SocketClient {
public:
    SocketClient(); /*!< SocketClient constructor. */

    /*! \fn void connection()
     *  \brief Establishes the connection between the client and the server.
     *  Generates and checks the creation of the client socket. It also reports if the client is communicating with the server.
     *  \exception stderr standard error output stream.
     *  \example socketclient.cpp
     */
    void connection();

    /*! \fn void sendData(const char *msn) const
     *  \brief Sends N bytes of BUF to server socket.
     *  Sends the client's information to the server, and notifies the client if it is not possible to sent the information.
     *  \param msn set of characters sent to the server
     *  \return integer as the number of characters sent
     *  \exception stderr standard error output stream in case the number of bytes is equal to -1.
     *  \example socketclient.cpp
     */
    void sendData(const char *msn) const;

private:
    /*! \fn static void *controller(void *obj)
     *  \brief Reads N bytes into BUF from server socket.
     *  Reads the information from the server, and notifies the client if it is not possible to received the information.
     *  \param obj points to the attributes of the SocketClient Class
     *  \return integer as the number of characters received
     *  \exception stderr standard error output stream in case the number of bytes is less than or equal to 0.
     *  \example socketclient.cpp
     */
    static void *controller(void *obj);

    int descriptor; /*!< Client socket of type SOCK_STREAM in domain AF_INET, using protocol IPPROTO_TCP. */
    sockaddr_in info; /*!< Structure describing the Internet client socket address. */
};

#endif //CLIENT_SOCKETCLIENT_H