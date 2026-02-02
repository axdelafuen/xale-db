#include "Net/LinuxSocket.h"

namespace Xale::Net
{
    /*
     * @brief TODO()
     */
    LinuxSocket::LinuxSocket() :
        _logger(Xale::Logger::Logger<FileStorageEngine>::getInstance()),
        _address({})
    {}

    /*
     * @brief Open a socket as a server using listening behavior
     */
    LinuxSocket::listen(int port)
    {
        _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (_serverSocket == -1) {
            logger.error("Socket creation failed");
            return -1; // THROW
        }

        int opt = 1;
        setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        _address.sin_family = AF_INET;
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            logger.error("Bind failed");
            close(_serverSocket);
            return -1; // THROW
        }

        if (listen(_serverSocket, 3) < 0) {
            logger.error("Listen failed");
            close(_serverSocket);
            return -1; // THROW
        }

        logger.info("Server listening on port " + std::to_string(port) + " ...");
    }

    /*
     * @brief Open a socket as a client using connect behavior
     */
    LinuxSocket::connect(const std::string hostAddress, uint16_t port)
    {

    }
}
