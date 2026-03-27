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

    int TcpClient::send(const Xale::Net::Packet* data, size_t size)
    {
        if (!_socket)
            return -1;

        std::vector<uint8_t> serializedData = data->serialize();
        std::string serializedStr(serializedData.begin(), serializedData.end());
        return _socket->send(&serializedStr, serializedStr.size());
    }

    int TcpClient::receive(Xale::Net::Packet* buffer, size_t size)
    {
        if (!_socket)
            return -1;

        std::string tempBuffer;
        int bytesRead = _socket->receive(&tempBuffer, size);
        if (bytesRead > 0) {
            std::vector<uint8_t> data(tempBuffer.begin(), tempBuffer.end());
            buffer->deserialize(data);
        }
        return bytesRead;
    }

    void TcpClient::close()
    {
        if (_socket) {
            _socket->close();
            _socket.reset();
        }
    }
}