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
    class LinuxListenerSocket : public IListenerSocket
    {
        public:
            LinuxListenerSocket();
            bool open(int port) override;
            int listen(std::string& buffer, size_t size) override;
            int respond(const std::string* data, size_t size) override; // should be SecurePaquet in the future
            void close() override;
        private:
            Xale::Logger::Logger<LinuxListenerSocket>& _logger;
            int _socket;
            int _clientSocket;
            sockaddr_in _address;
    };
}

#endif // NET_LINUX_LISTENER_SOCKET_H