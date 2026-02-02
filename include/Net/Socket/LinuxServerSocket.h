#if defined(__linux__) || defined(linux) || defined(__GNUG__)

#ifndef NET_LINUX_SOCKER_H
#define NET_LINUX_SOCKER_H

#include <Logger.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>

namespace Xale::Net
{
    class LinuxSocket : public ISocket
    {
        public:
            LinuxSocket();
            int listen(int port) override;
            bool connect(const std::string hostAddress, uint16_t port) override;
            int send(const std::string* data, size_t size) override; // should be SecurePaquet in the future
            int receive(std::string* buffer, size_t size) overrid;
            void close() override;
        private:
            Xale::Logger::Logger<LinuxSocket>& _logger;
            int _serverSocket;
            int _clientSocket;
            sockaddr_in _address;
    }
}

#endif // NET_LINUX_SOCKER_H

#endif
