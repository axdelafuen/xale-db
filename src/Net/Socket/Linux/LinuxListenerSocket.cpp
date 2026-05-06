#if defined(__linux__) || defined(linux) || defined(__GNUG__)

#include "Net/Socket/Linux/LinuxListenerSocket.h"

namespace Xale::Net
{
    LinuxListenerSocket::LinuxListenerSocket()
        : _logger(Xale::Logger::Logger<LinuxListenerSocket>::getInstance()),
          _socket(-1)
    {}

    bool LinuxListenerSocket::open(int port)
    {
        _socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == -1) {
            _logger.error("Socket creation failed");
            return false;
        }

        int opt = 1;
        ::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in address{};
        address.sin_family      = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port        = htons(port);

        if (::bind(_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
            _logger.error("Bind failed");
            close();
            return false;
        }

        if (::listen(_socket, 16) < 0) {
            _logger.error("Listen failed");
            close();
            return false;
        }

        return true;
    }

    std::unique_ptr<IClientConnection> LinuxListenerSocket::acceptClient()
    {
        sockaddr_in clientAddr{};
        socklen_t   clientLen = sizeof(clientAddr);

        int clientFd = ::accept(_socket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientFd < 0) {
            _logger.error("Accept failed");
            return nullptr;
        }

        _logger.info("New client connected");
        return std::make_unique<LinuxClientConnection>(clientFd);
    }

    void LinuxListenerSocket::close()
    {
        if (_socket != -1) {
            ::close(_socket);
            _socket = -1;
        }
    }
}

#endif // linux
