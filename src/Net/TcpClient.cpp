#include "Net/TcpClient.h"

namespace Xale::Net
{
    TcpClient::TcpClient(std::unique_ptr<Xale::Net::ISocketFactory> socketFactory) : 
        _socket(nullptr),
        _socketFactory(std::move(socketFactory))
    {}

    TcpClient::~TcpClient()
    {
        close();
    }

    bool TcpClient::connect(const std::string& ip, int port)
    {
        _socket = _socketFactory->createSocket();
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

        return _socket->send(reinterpret_cast<const std::vector<uint8_t>*>(data), size);
    }

    int TcpClient::receive(std::string* buffer, size_t size)
    {
        if (!_socket)
            return -1;

        return _socket->receive(reinterpret_cast<std::vector<uint8_t>*>(buffer), size);
    }

    int TcpClient::send(const Xale::Net::Packet* data, size_t size)
    {
        if (!_socket)
            return -1;

        std::vector<uint8_t> serializedData = data->serialize();
        return _socket->send(&serializedData, serializedData.size());
    }

    int TcpClient::receive(Xale::Net::Packet* buffer, size_t size)
    {
        if (!_socket)
            return -1;

        std::vector<uint8_t> tempBuffer;
        int bytesRead = _socket->receive(&tempBuffer, size);
        if (bytesRead > 0) {
            buffer->deserialize(tempBuffer);
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