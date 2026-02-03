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
    class LinuxSocket : public ISocket
    {
        public:
            LinuxSocket();
            bool connect(const std::string hostAddress&, int port) override;
            int send(const std::string* data, size_t size) override; // should be SecurePaquet in the future
            int receive(std::string* buffer, size_t size) override;
            void close() override;
        private:
            Xale::Logger::Logger<LinuxSocket>& _logger;
            int _socket;
            sockaddr_in _address;
    };
}

#endif // NET_LINUX_SOCKET_H

#endif
