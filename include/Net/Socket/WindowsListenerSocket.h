#if defined(_WIN32) || defined(_WIN64)

#ifndef NET_WINDOWS_LISTENER_SOCKET_H
#define NET_WINDOWS_LISTENER_SOCKET_H

#include <Logger.h>

#include "Net/Socket/IListenerSocket.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

namespace Xale::Net
{
    class WindowsListenerSocket : public IListenerSocket
    {
        public:
            WindowsListenerSocket();
            ~WindowsListenerSocket();
            bool open(int port) override;
            int listen(std::string& buffer, size_t size) override;
            int respond(const std::string* data, size_t size) override;
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

