#include "Net/TcpClient.h"

namespace Xale::Net
{
    /*
     * @brief Constructor
     */
    TcpClient::TcpClient() : 
        _socket(nullptr)
    {}

    /*
     * @brief Destructor
     */
    TcpClient::~TcpClient()
    {
        close();
    }

    /*
     * @brief Connects to the server at the specified IP and port
     */
    bool TcpClient::connect(const std::string& ip, int port)
    {
        _socket = Xale::Net::SocketFactory::createSocket();
        if (!_socket->connect(ip, port)) {
            close();
            return false;
        }

        return true;
    }

    /*
     * @brief Sends data to the server
     */
    int TcpClient::send(const std::string* data, size_t size)
    {
        if (!_socket)
            return -1;

        return _socket->send(data, size);
    }

    /*
     * @brief Receives data from the server
     */
    int TcpClient::receive(std::string* buffer, size_t size)
    {
        if (!_socket)
            return -1;

        return _socket->receive(buffer, size);
    }

    /*
     * @brief Closes the connection
     */
    void TcpClient::close()
    {
        if (_socket) {
            _socket->close();
            _socket.reset();
        }
    }
}