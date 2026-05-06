#include "Net/Socket/Linux/LinuxSSLClientConnection.h"

namespace Xale::Net
{
    LinuxSSLClientConnection::LinuxSSLClientConnection(int fd, SSL* ssl)
        : _fd(fd),
          _ssl(ssl),
          _logger(Xale::Logger::Logger<LinuxSSLClientConnection>::getInstance())
    {}

    LinuxSSLClientConnection::~LinuxSSLClientConnection()
    {
        cleanup();
    }

    int LinuxSSLClientConnection::read(std::vector<uint8_t>& buffer, size_t size)
    {
        if (!_ssl) return -1;

        char* tmp = new char[size];
        int bytesRead = SSL_read(_ssl, tmp, static_cast<int>(size));

        if (bytesRead > 0)
            buffer.assign(tmp, tmp + bytesRead);
        else if (bytesRead == 0)
            _logger.info("SSL client disconnected (clean)");
        else
            _logger.info("SSL client connection lost");

        delete[] tmp;
        return bytesRead;
    }

    int LinuxSSLClientConnection::respond(const std::vector<uint8_t>* data, size_t size)
    {
        if (!_ssl || !data || size == 0) return -1;
        return SSL_write(_ssl, data->data(), static_cast<int>(size));
    }

    void LinuxSSLClientConnection::close()
    {
        cleanup();
    }

    void LinuxSSLClientConnection::cleanup()
    {
        if (_ssl)
        {
            SSL_shutdown(_ssl);
            SSL_free(_ssl);
            _ssl = nullptr;
        }
        if (_fd != -1)
        {
            ::close(_fd);
            _fd = -1;
        }
    }
}
