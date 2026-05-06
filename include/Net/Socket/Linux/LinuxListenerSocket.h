#ifndef NET_LINUX_LISTENER_SOCKET_H
#define NET_LINUX_LISTENER_SOCKET_H

#if defined(__linux__) || defined(linux) || defined(__GNUG__)

#include <Logger.h>

#include "Net/Socket/IListenerSocket.h"
#include "Net/Socket/Linux/LinuxClientConnection.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

namespace Xale::Net
{
    /**
     * @brief Linux raw TCP implementation of IListenerSocket
     */
    class LinuxListenerSocket : public IListenerSocket
    {
        public:
            /**
             * @brief Constructor
             */
            LinuxListenerSocket();

            /**
             * @brief Bind and start listening on the given port
             * @param port Port number
             */
            bool open(int port) override;

            /**
             * @brief Block until a client connects, return a connection object
             * @return IClientConnection for the accepted client, nullptr on error
             */
            std::unique_ptr<IClientConnection> acceptClient() override;

            /**
             * @brief Close the listening socket
             */
            void close() override;

        private:
            Xale::Logger::Logger<LinuxListenerSocket>& _logger;
            int _socket;
    };
}

#endif // linux
#endif // NET_LINUX_LISTENER_SOCKET_H