#ifndef SERVER_SOCKETSERVER_H
#define SERVER_SOCKETSERVER_H

/*!
 * \include arpa/inet.h
 * \include sys/socket.h
 * \include sys/types.h
 * \include netdb.h
 * \include unistd.h
 * \include pthread.h
 * \include vector
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
#include <vector>
#include <cstring>
#include <string>
#include <iostream>

/*!
 * \namespace std General-purpose polymorphic function wrapper.
 */
using namespace std;

/*! \struct PlayerSocket
 *  \brief Socket general information.
 */
struct PlayerSocket {
    int descriptor; /*!< Client socket ID. */
    sockaddr_in info; /*!< Structure describing the Internet client socket address. */
};

/*! \class SocketServer
 *  \brief Server side communication.
 */
class SocketServer {
public:
    SocketServer(); /*!< SocketServer constructor. */

    static string playerKeyPressed; /*!< Key code received from the client. */

    /*! \fn void receive()
     *  \brief Verify client-server communication.
     *  It verifies that the server establishes communication with the client, also verifies that the server can communicate and listen correctly to the client.
     *  It updates the list of connected players and creates the player's thread.
     *  \return new player thread and an updated players list
     *  \exception stderr standard error output stream in case the server is not listening or can not established the connection with the client.
     *  \example socketserver.cpp
     */
    void receive();

    /*! \fn void sendData(const char *msn)
     *  \brief Sends N bytes of BUF to clients socket.
     *  Sends the server's information to the clients, and notifies the server if it is not possible to sent the information.
     *  \param msn set of characters sent to the clients
     *  \return integer as the number of characters sent
     *  \exception stderr standard error output stream in case the number of bytes is equal to -1.
     *  \example socketserver.cpp
     */
    void sendData(const char *msn);

private:
    int descriptor; /*!< Server socket of type SOCK_STREAM in domain AF_INET, using protocol IPPROTO_TCP. */
    sockaddr_in info; /*!< Structure describing the Internet server socket address. */
    vector<int> players; /*!< Clients connected to the server. */

    /*! \fn bool connection()
     *  \brief Creates the server socket.
     *  Creates a new socket of type SOCK_STREAM in domain AF_INET, using protocol IPPROTO_TCP.
     *  In case of socket creation failure, it returns false. In case of successful creation the IP and port number is assigned
     *  to the server socket, finally it returns true.
     *  \return boolean
     *  \example socketserver.cpp
     */
    bool connection();

    /*! \fn bool listening()
     *  \brief Verify if the server is listening.
     *  In case no information is being received, it returns false.
     *  Otherwise, it will make 4 connection requests before completely rejecting the requests and will return true if it completes these requests.
     *  \return boolean
     *  \example socketserver.cpp
     */
    bool listening();

    /*! \fn static void *controller(void *obj)
     *  \brief Reads N bytes into BUF from client socket.
     *  Reads the information from the client, and notifies the server if it is not possible to received the information.
     *  \param obj points to the attributes of the SocketServer Class
     *  \return integer as the number of characters received
     *  \exception output stream in case the number of bytes is less than or equal to 0, this could happen in case the client gets disconnected from the server.
     *  \example socketserver.cpp
     */
    static void *controller(void *obj);
};

#endif //SERVER_SOCKETSERVER_H