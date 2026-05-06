#include "Net/Socket/Linux/LinuxSSLListenerSocket.h"

namespace Xale::Net
{
    LinuxSSLListenerSocket::LinuxSSLListenerSocket(std::string certFile, std::string keyFile)
        : _logger(Xale::Logger::Logger<LinuxSSLListenerSocket>::getInstance()),
          _socket(-1),
          _ctx(nullptr),
          _certFile(std::move(certFile)),
          _keyFile(std::move(keyFile))
    {}

    bool LinuxSSLListenerSocket::open(int port)
    {
        const SSL_METHOD* method = TLS_server_method();
        _ctx = SSL_CTX_new(method);
        if (!_ctx)
        {
            _logger.error("Unable to create SSL context");
            return false;
        }

        if (SSL_CTX_use_certificate_file(_ctx, _certFile.c_str(), SSL_FILETYPE_PEM) <= 0)
        {
            _logger.error("Unable to load certificate");
            return false;
        }

        if (SSL_CTX_use_PrivateKey_file(_ctx, _keyFile.c_str(), SSL_FILETYPE_PEM) <= 0)
        {
            _logger.error("Unable to load private key");
            return false;
        }

        _socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == -1)
        {
            _logger.error("Socket creation failed");
            return false;
        }

        int opt = 1;
        ::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in address{};
        address.sin_family      = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port        = htons(port);

        if (::bind(_socket, (struct sockaddr*)&address, sizeof(address)) < 0)
        {
            _logger.error("Bind failed");
            close();
            return false;
        }

        if (::listen(_socket, 16) < 0)
        {
            _logger.error("Listen failed");
            close();
            return false;
        }

        return true;
    }

    std::unique_ptr<IClientConnection> LinuxSSLListenerSocket::acceptClient()
    {
        sockaddr_in clientAddr{};
        socklen_t   clientLen = sizeof(clientAddr);

        int clientFd = ::accept(_socket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientFd < 0)
        {
            _logger.error("Accept failed");
            return nullptr;
        }

        SSL* ssl = SSL_new(_ctx);
        SSL_set_fd(ssl, clientFd);

        if (SSL_accept(ssl) <= 0)
        {
            _logger.error("SSL handshake failed");
            SSL_free(ssl);
            ::close(clientFd);
            return nullptr;
        }

        _logger.info("New SSL client connected");
        return std::make_unique<LinuxSSLClientConnection>(clientFd, ssl);
    }

    void LinuxSSLListenerSocket::close()
    {
        if (_ctx)
        {
            SSL_CTX_free(_ctx);
            _ctx = nullptr;
        }
        if (_socket != -1)
        {
            ::close(_socket);
            _socket = -1;
        }
    }
}
