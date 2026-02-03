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
    class WindowsSocket : public ISocket
    {
        public:
            WindowsSocket();
            ~WindowsSocket();
            bool connect(const std::string& hostAddress, int port) override;
            int send(const std::string* data, size_t size) override;
            int receive(std::string* buffer, size_t size) override;
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