#if defined(_WIN32) || defined(_WIN64)

#include "Net/Socket/WindowsSocket.h"

namespace Xale::Net
{
    WindowsSocket::WindowsSocket() :
        _logger(Xale::Logger::Logger<WindowsSocket>::getInstance()),
        _socket(INVALID_SOCKET),
        _address({}),
        _wsaInitialized(false)
    {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            _logger.error("WSAStartup failed: " + std::to_string(result));
        } else {
            _wsaInitialized = true;
        }
    }

    WindowsSocket::~WindowsSocket()
    {
        close();
        if (_wsaInitialized) {
            WSACleanup();
        }
    }

    bool WindowsSocket::connect(const std::string& hostAddress, int port)
    {
        if (!_wsaInitialized) {
            _logger.error("Winsock not initialized");
            return false;
        }

        _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_socket == INVALID_SOCKET) {
            _logger.error("Socket creation failed: " + std::to_string(WSAGetLastError()));
            return false;
        }

        _address.sin_family = AF_INET;
        _address.sin_port = htons(port);

        // Convert IP address from string to binary
        if (inet_pton(AF_INET, hostAddress.c_str(), &_address.sin_addr) <= 0) {
            _logger.error("Invalid address: " + hostAddress);
            close();
            return false;
        }

        if (::connect(_socket, (struct sockaddr*)&_address, sizeof(_address)) == SOCKET_ERROR) {
            _logger.error("Connection to " + hostAddress + ":" + std::to_string(port) + 
                         " failed: " + std::to_string(WSAGetLastError()));
            close();
            return false;
        }
     
        _logger.info("Connected to " + hostAddress + ":" + std::to_string(port));
        return true;
    }

    int WindowsSocket::send(const std::string* data, size_t size)
    {
        if (_socket == INVALID_SOCKET) {
            _logger.error("Socket not connected");
            return -1;
        }

        int result = ::send(_socket, data->c_str(), static_cast<int>(size), 0);
        if (result == SOCKET_ERROR) {
            _logger.error("Send failed: " + std::to_string(WSAGetLastError()));
            return -1;
        }
        return result;
    }

    int WindowsSocket::receive(std::string* buffer, size_t size)
    {
        if (_socket == INVALID_SOCKET) {
            _logger.error("Socket not connected");
            return -1;
        }

        char* tempBuffer = new char[size];
        int bytesRead = ::recv(_socket, tempBuffer, static_cast<int>(size), 0);
        
        if (bytesRead > 0) {
            buffer->assign(tempBuffer, bytesRead);
        } else if (bytesRead == 0) {
            _logger.info("Connection closed by peer");
        } else {
            _logger.error("Receive failed: " + std::to_string(WSAGetLastError()));
        }

        delete[] tempBuffer;
        return bytesRead;
    }

    void WindowsSocket::close()
    {
        if (_socket != INVALID_SOCKET) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
        }
    }
}

#endif
