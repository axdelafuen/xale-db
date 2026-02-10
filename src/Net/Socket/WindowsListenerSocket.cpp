#if defined(_WIN32) || defined(_WIN64)

#include "Net/Socket/WindowsListenerSocket.h"

namespace Xale::Net
{
    /**
     * @brief Constructor - Initialize Winsock
     */
    WindowsListenerSocket::WindowsListenerSocket()
        : _logger(Xale::Logger::Logger<WindowsListenerSocket>::getInstance()), 
        _socket(INVALID_SOCKET),
        _clientSocket(INVALID_SOCKET),
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

    /**
     * @brief Destructor - Cleanup Winsock
     */
    WindowsListenerSocket::~WindowsListenerSocket()
    {
        close();
        if (_wsaInitialized) {
            WSACleanup();
        }
    }

    /**
     * @brief Opens the listener socket on the specified port
     * @param port Port number to listen on
     * @return true if successful, false otherwise
     */
    bool WindowsListenerSocket::open(int port)
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
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(port);

        if (bind(_socket, (struct sockaddr*)&_address, sizeof(_address)) == SOCKET_ERROR) {
            _logger.error("Bind failed: " + std::to_string(WSAGetLastError()));
            close();
            return false;
        }

        if (::listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
            _logger.error("Listen failed: " + std::to_string(WSAGetLastError()));
            close();
            return false;
        }

        _logger.info("Listening on port " + std::to_string(port));
        return true;
    }

    /**
     * @brief Listens for incoming client connections and reads data into the buffer
     * @param buffer Buffer to store received data
     * @param size Maximum size to receive
     * @return Number of bytes received, 0 if connection closed, -1 on error
     */
    int WindowsListenerSocket::listen(std::string& buffer, size_t size)
    {
        // Accept new client if not already connected
        if (_clientSocket == INVALID_SOCKET) {
            sockaddr_in client_address{};
            int client_len = sizeof(client_address);
            
            _clientSocket = accept(_socket, (struct sockaddr*)&client_address, &client_len);
            if (_clientSocket == INVALID_SOCKET) {
                _logger.error("Accept failed: " + std::to_string(WSAGetLastError()));
                return -1;
            }
            
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_address.sin_addr, clientIP, INET_ADDRSTRLEN);
            _logger.info("Client connected from " + std::string(clientIP) + ":" + 
                        std::to_string(ntohs(client_address.sin_port)));
        }

        // Receive data from client
        char* tempBuffer = new char[size];
        int bytesRead = recv(_clientSocket, tempBuffer, static_cast<int>(size), 0);
        
        if (bytesRead > 0) {
            buffer.assign(tempBuffer, bytesRead);
        } else if (bytesRead == 0) {
            _logger.info("Client disconnected");
            closesocket(_clientSocket);
            _clientSocket = INVALID_SOCKET;
        } else {
            _logger.error("Receive failed: " + std::to_string(WSAGetLastError()));
            closesocket(_clientSocket);
            _clientSocket = INVALID_SOCKET;
        }

        delete[] tempBuffer;
        return bytesRead;
    }

    /**
     * @brief Responds to the connected client with the provided data
     * @param data Data to send
     * @param size Size of data
     * @return Number of bytes sent, or -1 on error
     */
    int WindowsListenerSocket::respond(const std::string* data, size_t size)
    {
        if (_clientSocket == INVALID_SOCKET) {
            _logger.error("No client connected to respond to");
            return -1;
        }

        int bytesSent = ::send(_clientSocket, data->c_str(), static_cast<int>(size), 0);
        if (bytesSent == SOCKET_ERROR) {
            _logger.error("Send failed: " + std::to_string(WSAGetLastError()));
            return -1;
        }
        return bytesSent;
    }

    /**
     * @brief Closes the listener and client sockets
     */
    void WindowsListenerSocket::close()
    {
        if (_clientSocket != INVALID_SOCKET) {
            closesocket(_clientSocket);
            _clientSocket = INVALID_SOCKET;
        }
        if (_socket != INVALID_SOCKET) {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
        }
    }
}

#endif
