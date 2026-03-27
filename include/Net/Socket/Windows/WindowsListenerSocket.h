#if defined(_WIN32) || defined(_WIN64)

#ifndef NET_WINDOWS_LISTENER_SOCKET_H
#define NET_WINDOWS_LISTENER_SOCKET_H

#include <Logger.h>

#include "Net/Socket/IListenerSocket.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

namespace Xale::Net
{
    /**
     * @brief Windows implementation of IListenerSocket
     */
    class WindowsListenerSocket : public IListenerSocket
    {
        public:
            /**
             * @brief Constructor - Initialize Winsock
             */
            WindowsListenerSocket();

            /**
             * @brief Destructor - Cleanup Winsock
             */
            ~WindowsListenerSocket();

            /**
             * @brief Opens the listener socket on the specified port
             * @param port Port number to listen on
             * @return true if successful, false otherwise
             */
            bool open(int port) override;

            /**
             * @brief Listens for incoming client connections and reads data into the buffer
             * @param buffer Buffer to store received data
             * @param size Maximum size to receive
             * @return Number of bytes received, 0 if connection closed, -1 on error
             */
            int listen(std::string& buffer, size_t size) override;

            /**
             * @brief Responds to the connected client with the provided data
             * @param data Data to send
             * @param size Size of data
             * @return Number of bytes sent, or -1 on error
             */
            int respond(const std::string* data, size_t size) override;

            /**
             * @brief Closes the listener and client sockets
             */
            void close() override;
        private:
            Xale::Logger::Logger<WindowsListenerSocket>& _logger;
            SOCKET _socket;
            SOCKET _clientSocket;
            sockaddr_in _address;
            bool _wsaInitialized;
    };
}

#endif // NET_WINDOWS_LISTENER_SOCKET_H

#endif

