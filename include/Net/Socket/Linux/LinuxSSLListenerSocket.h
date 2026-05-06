#ifndef NET_SOCKET_LINUX_SSL_LISTENER_SOCKET_H
#define NET_SOCKET_LINUX_SSL_LISTENER_SOCKET_H

#include <Logger.h>

#include "Net/Socket/IListenerSocket.h"

#include <string>
#include <vector>
#include <cstdint>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace Xale::Net
{
    class LinuxSSLListenerSocket : public IListenerSocket
    {
        public:
            LinuxSSLListenerSocket(std::string certFile, std::string keyFile);
            bool open(int port) override;
            int listen(std::vector<uint8_t>& buffer, size_t size) override;
            int respond(const std::vector<uint8_t>* data, size_t size) override;
            void close() override;

        private:
            Xale::Logger::Logger<LinuxSSLListenerSocket>& _logger;
            int _socket;
            int _clientSocket;
            SSL_CTX* _ctx = nullptr; 
            SSL* _ssl = nullptr;
            std::string _certFile;
            std::string _keyFile;

            void cleanupSSL();
    };
}

#endif // NET_SOCKET_LINUX_SSL_LISTENER_SOCKET_H