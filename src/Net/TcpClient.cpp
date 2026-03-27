#include "Net/TcpClient.h"

namespace Xale::Net
{
    TcpClient::TcpClient() : 
        _socket(nullptr)
    {}

    TcpClient::~TcpClient()
    {
        close();
    }

    bool TcpClient::connect(const std::string& ip, int port)
    {
        _socket = Xale::Net::SocketFactory::createSocket();
        if (!_socket->connect(ip, port)) {
            close();
            return false;
        }

        return true;
    }

    int TcpClient::send(const std::string* data, size_t size)
    {
        if (!_socket)
            return -1;

        return _socket->send(data, size);
    }

    int TcpClient::receive(std::string* buffer, size_t size)
    {
        if (!_socket)
            return -1;

        return _socket->receive(buffer, size);
    }

    void TcpClient::close()
    {
        if (_socket) {
            _socket->close();
            _socket.reset();
        }
    }
}