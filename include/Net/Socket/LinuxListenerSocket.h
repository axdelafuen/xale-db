#ifndef NET_LINUX_LISTENER_SOCKET_H
#define NET_LINUX_LISTENER_SOCKET_H

#include <Logger.h>

#include "Net/Socket/IListenerSocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

namespace Xale::Net
{
    /**
     * @brief Linux implementation of IListenerSocket
     */
    class LinuxListenerSocket : public IListenerSocket
    {
        public:
            /**
             * @brief Constructor
             */
            LinuxListenerSocket();

            /**
             * @brief Opens the listener socket on the specified port
             * @param port Port number
             */
            bool open(int port) override;

            /**
             * @brief Listens for incoming client connections and reads data into the buffer
             * @param buffer Buffer to store received data
             * @param size Maximum size to receive
             */
            int listen(std::string& buffer, size_t size) override;

            /**
             * @brief Responds to the connected client with the provided data
             * @param data Data to send
             * @param size Size of data
             */
            int respond(const std::string* data, size_t size) override; // should be SecurePaquet in the future

            /**
             * @brief Closes the listener and client sockets
             */
            void close() override;
        private:
            Xale::Logger::Logger<LinuxListenerSocket>& _logger;
            int _socket;
            int _clientSocket;
            sockaddr_in _address;
    };
}

#endif // NET_LINUX_LISTENER_SOCKET_H