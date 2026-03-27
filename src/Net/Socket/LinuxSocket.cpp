#if defined(__linux__) || defined(linux) || defined(__GNUG__)

#include "Net/Socket/LinuxSocket.h"

namespace Xale::Net
{
    LinuxSocket::LinuxSocket() :
        _logger(Xale::Logger::Logger<LinuxSocket>::getInstance()),
        _socket(-1),
        _address({})
    {}

    bool LinuxSocket::connect(const std::string& hostAddress, int port)
    {
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == -1) {
            _logger.error("Socket creation failed");
            return false;
        }

        _address.sin_family = AF_INET;
        _address.sin_port = htons(port);

        if (inet_pton(AF_INET, hostAddress.c_str(), &_address.sin_addr) <= 0) {
            _logger.error("Invalid address: " + hostAddress);
            close();
            return false;
        }

        if (::connect(_socket, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
            _logger.error("Connection to " + hostAddress + ":" + std::to_string(port) + " failed");
            close();
            return false;
        }
     
        return true;
    }

    int LinuxSocket::send(const std::string* data, size_t size)
    {
        return ::send(_socket, data->c_str(), size, 0);
    }

    int LinuxSocket::receive(std::string* buffer, size_t size)
    {
        char* tempBuffer = new char[size];
        int bytesRead = ::read(_socket, tempBuffer, size);
        
        if (bytesRead > 0)
            buffer->assign(tempBuffer, bytesRead);

        delete[] tempBuffer;
        return bytesRead;
    }

    void LinuxSocket::close()
    {
        ::close(_socket);
        _socket = -1;
    }
}

#endif
