#if defined(__linux__) || defined(linux) || defined(__GNUG__)

#ifndef NET_LINUX_SOCKET_H
#define NET_LINUX_SOCKET_H

#include <Logger.h>

#include "Net/Socket/ISocket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

namespace Xale::Net
{
    /**
     * @brief Linux implementation of ISocket
     */
    class LinuxSocket : public ISocket
    {
        public:
            /**
             * @brief Constructor
             */
            LinuxSocket();

            /**
             * @brief Connect a socket to a host/port
             * @param hostAddress Host address
             * @param port Port number
             */
            bool connect(const std::string& hostAddress, int port) override;

            /**
             * @brief Send data through the socket
             * @param data Data to send
             * @param size Size of data
             */
            int send(const std::string* data, size_t size) override; // should be SecurePaquet in the future

            /**
             * @brief Receive data from the socket
             * @param buffer Buffer to store received data
             * @param size Maximum size to receive
             */
            int receive(std::string* buffer, size_t size) override;

            /**
             * @brief Close the socket
             */
            void close() override;
        private:
            Xale::Logger::Logger<LinuxSocket>& _logger;
            int _socket;
            sockaddr_in _address;
    };
}

#endif // NET_LINUX_SOCKET_H

#endif
