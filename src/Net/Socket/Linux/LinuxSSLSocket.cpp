#include "Net/Socket/Linux/LinuxSSLSocket.h"

namespace Xale::Net
{
    LinuxSSLSocket::LinuxSSLSocket() :
        _logger(Xale::Logger::Logger<LinuxSSLSocket>::getInstance()), 
        _socket(-1), 
        _ctx(nullptr), 
        _ssl(nullptr)
    {}

    bool LinuxSSLSocket::connect(const std::string& ip, int port)
    {
        const SSL_METHOD* method = TLS_client_method();
        _ctx = SSL_CTX_new(method);
        if (!_ctx) {
            _logger.error("Unable to create SSL context");
            return false;
        }

        _socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == -1) {
            _logger.error("Socket creation failed");
            SSL_CTX_free(_ctx);
            return false;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
            _logger.error("Invalid IP address");
            ::close(_socket);
            _socket = -1;
            SSL_CTX_free(_ctx);
            _ctx = nullptr;
            return false;
        }

        if (::connect(_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            _logger.error("TCP connection failed");
            ::close(_socket);
            _socket = -1;
            SSL_CTX_free(_ctx);
            _ctx = nullptr;
            return false;
        }

        _ssl = SSL_new(_ctx);
        SSL_set_fd(_ssl, _socket);

        if (SSL_connect(_ssl) <= 0) {
            _logger.error("SSL connection failed");
            SSL_free(_ssl);
            _ssl = nullptr;
            ::close(_socket);
            _socket = -1;
            SSL_CTX_free(_ctx);
            _ctx = nullptr;
            return false;
        }

        return true;
    }

    int LinuxSSLSocket::send(const std::vector<uint8_t>* data, size_t size)
    {
        if (!_ssl) {
            _logger.error("SSL connection not established");
            return -1;
        }

        if (!data || size == 0) {
            _logger.warning("No data to send");
            return 0;
        }

        int bytesSent = SSL_write(_ssl, data->data(), static_cast<int>(size));
        if (bytesSent <= 0) {
            _logger.error("SSL write failed");
            return -1;
        }

        return bytesSent;
    }

    int LinuxSSLSocket::receive(std::vector<uint8_t>* buffer, size_t size)
    {
        if (!_ssl) {
            _logger.error("SSL connection not established");
            return -1;
        }

        if (!buffer || size == 0) {
            _logger.warning("No buffer to receive data");
            return 0;
        }

        char* tempBuffer = new char[size];
        int bytesRead = SSL_read(_ssl, tempBuffer, size);
        
        if (bytesRead > 0)
            buffer->assign(tempBuffer, tempBuffer + bytesRead);

        delete[] tempBuffer;

        return bytesRead;
    }

    void LinuxSSLSocket::close()
    {
        if (_ssl) {
            SSL_free(_ssl);
            _ssl = nullptr;
        }

        if (_socket != -1) {
            ::close(_socket);
            _socket = -1;
        }

        if (_ctx) {
            SSL_CTX_free(_ctx);
            _ctx = nullptr;
        }
    }
}