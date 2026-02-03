#include "Net/Socket/LinuxListenerSocket.h"

namespace Xale::Net
{
    /*
     * @brief Constructor
     */
    LinuxListenerSocket::LinuxListenerSocket()
        : _logger(Xale::Logger::Logger<LinuxListenerSocket>::getInstance()), 
        _socket(-1),
        _clientSocket(-1),
        _address({})
    {}

    /*
     * @brief Opens the listener socket on the specified port
     */
    bool LinuxListenerSocket::open(int port)
    {
        _socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == -1) {
            _logger.error("Socket creation failed");
            return false;
        }

        _address.sin_family = AF_INET;
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(port);

        if (::bind(_socket, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
            _logger.error("Bind failed");
            close();
            return false;
        }

        if (::listen(_socket, 3) < 0) {
            _logger.error("Listen failed");
            close();
            return false;
        }

        return true;
    }

    /*
     * @brief Listens for incoming client connections and reads data into the buffer
     */
    int LinuxListenerSocket::listen(std::string& buffer, size_t size)
    {
        if (_clientSocket == -1) {
            sockaddr_in client_address{};
            socklen_t client_len = sizeof(client_address);
            
            _clientSocket = ::accept(_socket, (struct sockaddr*)&client_address, &client_len);
            if (_clientSocket < 0) {
                _logger.error("Accept failed");
                return -1;
            }
            _logger.info("Client connected");
        }

        char* tempBuffer = new char[size];
        int bytesRead = ::read(_clientSocket, tempBuffer, size);
        
        if (bytesRead > 0) {
            buffer.assign(tempBuffer, bytesRead);
        } else if (bytesRead == 0) {
            _logger.info("Client disconnected");
            ::close(_clientSocket);
            _clientSocket = -1;
        }

        delete[] tempBuffer;
        return bytesRead;
    }

    /*
     * @brief Responds to the connected client with the provided data
     */
    int LinuxListenerSocket::respond(const std::string* data, size_t size)
    {
        if (_clientSocket == -1) {
            _logger.error("No client connected to respond to");
            return -1;
        }

        int bytesSent = ::send(_clientSocket, data->c_str(), size, 0);
        return bytesSent;
    }

    /*
     * @brief Closes the listener and client sockets
     */
    void LinuxListenerSocket::close()
    {
        if (_socket != -1)
        {
            ::close(_socket);
            _socket = -1;
        }
        if (_clientSocket != -1)
        {
            ::close(_clientSocket);
            _clientSocket = -1;
        }
    }
}
