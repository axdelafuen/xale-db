#if defined(__linux__) || defined(linux) || defined(__GNUG__)

#include "Net/Socket/Linux/LinuxClientConnection.h"

namespace Xale::Net
{
    LinuxClientConnection::LinuxClientConnection(int fd)
        : _fd(fd),
          _logger(Xale::Logger::Logger<LinuxClientConnection>::getInstance())
    {}

    LinuxClientConnection::~LinuxClientConnection()
    {
        close();
    }

    int LinuxClientConnection::read(std::vector<uint8_t>& buffer, size_t size)
    {
        char* tmp = new char[size];
        int bytesRead = ::read(_fd, tmp, size);

        if (bytesRead > 0)
            buffer.assign(tmp, tmp + bytesRead);
        else if (bytesRead == 0)
            _logger.info("Client disconnected (clean)");
        else
            _logger.info("Client connection lost");

        delete[] tmp;
        return bytesRead;
    }

    int LinuxClientConnection::respond(const std::vector<uint8_t>* data, size_t size)
    {
        if (_fd == -1 || !data) return -1;
        return ::send(_fd, data->data(), size, MSG_NOSIGNAL);
    }

    void LinuxClientConnection::close()
    {
        if (_fd != -1)
        {
            ::close(_fd);
            _fd = -1;
        }
    }
}

#endif // linux
