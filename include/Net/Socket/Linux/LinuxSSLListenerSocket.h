#ifndef NET_SOCKET_LINUX_SSL_LISTENER_SOCKET_H
#define NET_SOCKET_LINUX_SSL_LISTENER_SOCKET_H

#include <Logger.h>

#include "Net/Socket/IListenerSocket.h"
#include "Net/Socket/Linux/LinuxSSLClientConnection.h"

#include <string>
#include <memory>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace Xale::Net
{
    /**
     * @brief Linux SSL/TLS implementation of IListenerSocket
     */
    class LinuxSSLListenerSocket : public IListenerSocket
    {
        public:
            /**
             * @brief Construct with paths to PEM certificate and key files
             */
            LinuxSSLListenerSocket(std::string certFile, std::string keyFile);

            /**
             * @brief Bind and start listening, initialise SSL context
             */
            bool open(int port) override;

            /**
             * @brief Block until a client connects, perform SSL handshake, return connection
             * @return IClientConnection for the accepted client, nullptr on error
             */
            std::unique_ptr<IClientConnection> acceptClient() override;

            /**
             * @brief Close the listening socket and free SSL context
             */
            void close() override;

        private:
            Xale::Logger::Logger<LinuxSSLListenerSocket>& _logger;
            int      _socket;
            SSL_CTX* _ctx;
            std::string _certFile;
            std::string _keyFile;
    };
}

#endif // NET_SOCKET_LINUX_SSL_LISTENER_SOCKET_H