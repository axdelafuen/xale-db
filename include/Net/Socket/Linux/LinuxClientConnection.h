#ifndef NET_LINUX_CLIENT_CONNECTION_H
#define NET_LINUX_CLIENT_CONNECTION_H

#if defined(__linux__) || defined(linux) || defined(__GNUG__)

#include <Logger.h>

#include "Net/Socket/IClientConnection.h"

#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <cstdint>

namespace Xale::Net
{
    /**
     * @brief Raw TCP client connection (Linux, no SSL)
     */
    class LinuxClientConnection : public IClientConnection
    {
        public:
            /**
             * @brief Construct from an accepted socket file descriptor
             * @param fd Accepted client socket fd
             */
            explicit LinuxClientConnection(int fd);

            /**
             * @brief Closes the connection if still open
             */
            ~LinuxClientConnection() override;

            /**
             * @brief Read data from the client socket
             */
            int read(std::vector<uint8_t>& buffer, size_t size) override;

            /**
             * @brief Send data to the client socket
             */
            int respond(const std::vector<uint8_t>* data, size_t size) override;

            /**
             * @brief Close the client socket
             */
            void close() override;

        private:
            int _fd;
            Xale::Logger::Logger<LinuxClientConnection>& _logger;
    };
}

#endif // linux
#endif // NET_LINUX_CLIENT_CONNECTION_H
