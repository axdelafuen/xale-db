#if defined(_WIN32) || defined(_WIN64)

#ifndef NET_WINDOWS_SOCKET_H
#define NET_WINDOWS_SOCKET_H

#include <Logger.h>

#include "Net/Socket/ISocket.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

namespace Xale::Net
{
    /**
     * @brief Windows implementation of ISocket
     */
    class WindowsSocket : public ISocket
    {
        public:
            /**
             * @brief Constructor - Initialize Winsock
             */
            WindowsSocket();

            /**
             * @brief Destructor - Cleanup Winsock
             */
            ~WindowsSocket();

            /**
             * @brief Connect a socket to a host/port
             * @param hostAddress IP address or hostname
             * @param port Port number
             * @return true if connection successful, false otherwise
             */
            bool connect(const std::string& hostAddress, int port) override;

            /**
             * @brief Send data through the socket
             * @param data Data to send
             * @param size Size of data
             * @return Number of bytes sent, or -1 on error
             */
            int send(const std::string* data, size_t size) override;

            /**
             * @brief Receive data from the socket
             * @param buffer Buffer to store received data
             * @param size Maximum size to receive
             * @return Number of bytes received, 0 if connection closed, -1 on error
             */
            int receive(std::string* buffer, size_t size) override;

            /**
             * @brief Close the socket
             */
            void close() override;
        private:
            Xale::Logger::Logger<WindowsSocket>& _logger;
            SOCKET _socket;
            sockaddr_in _address;
            bool _wsaInitialized;
    };
}

#endif // NET_WINDOWS_SOCKET_H

#endif