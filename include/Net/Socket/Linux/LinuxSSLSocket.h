#ifndef NET_SOCKET_LINUX_SSL_SOCKET_H
#define NET_SOCKET_LINUX_SSL_SOCKET_H

#include <Logger.h>

#include "Net/Socket/ISocket.h"

#include <vector>
#include <string>
#include <cstdint>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace Xale::Net
{
    class LinuxSSLSocket : public ISocket
    {
        public:
            LinuxSSLSocket();
            bool connect(const std::string& ip, int port) override;
            int send(const std::vector<uint8_t>* data, size_t size) override;
            int receive(std::vector<uint8_t>* buffer, size_t size) override;
            void close() override;

        private:
            Xale::Logger::Logger<LinuxSSLSocket>& _logger;
            int _socket;
            SSL_CTX* _ctx = nullptr; 
            SSL* _ssl = nullptr;
    };
}

#endif // NET_SOCKET_LINUX_SSL_SOCKET_H