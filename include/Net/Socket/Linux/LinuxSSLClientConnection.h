#ifndef NET_LINUX_SSL_CLIENT_CONNECTION_H
#define NET_LINUX_SSL_CLIENT_CONNECTION_H

#include <Logger.h>

#include "Net/Socket/IClientConnection.h"

#include <openssl/ssl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <cstdint>

namespace Xale::Net
{
    /**
     * @brief SSL/TLS client connection (Linux)
     *
     * Takes ownership of the SSL* but NOT of the SSL_CTX* which stays
     * in the listener socket.
     */
    class LinuxSSLClientConnection : public IClientConnection
    {
        public:
            /**
             * @brief Construct from an accepted fd and its SSL object
             * @param fd  Accepted client socket fd
             * @param ssl SSL object already handshaked for this connection
             */
            LinuxSSLClientConnection(int fd, SSL* ssl);

            /**
             * @brief Shuts down SSL and closes the socket
             */
            ~LinuxSSLClientConnection() override;

            /**
             * @brief Read data through SSL
             */
            int read(std::vector<uint8_t>& buffer, size_t size) override;

            /**
             * @brief Send data through SSL
             */
            int respond(const std::vector<uint8_t>* data, size_t size) override;

            /**
             * @brief Shutdown SSL and close the socket
             */
            void close() override;

        private:
            int  _fd;
            SSL* _ssl;
            Xale::Logger::Logger<LinuxSSLClientConnection>& _logger;

            /**
             * @brief Internal helper: free SSL resources and close fd
             */
            void cleanup();
    };
}

#endif // NET_LINUX_SSL_CLIENT_CONNECTION_H
