#include "Net/Socket/Linux/LinuxSSLListenerSocket.h"

namespace Xale::Net
{
    LinuxSSLListenerSocket::LinuxSSLListenerSocket(std::string certFile, std::string keyFile)
        : _logger(Xale::Logger::Logger<LinuxSSLListenerSocket>::getInstance()), 
        _socket(-1),
        _clientSocket(-1),
        _ctx(nullptr),
        _ssl(nullptr),
        _certFile(certFile),
        _keyFile(keyFile)
    {}

    bool LinuxSSLListenerSocket::open(int port)
    {
        const SSL_METHOD* method = TLS_server_method();
        _ctx = SSL_CTX_new(method);
        if (!_ctx) 
        {
            _logger.error("Unable to create SSL context");
            return false;
        }

        if (SSL_CTX_use_certificate_file(_ctx, _certFile.c_str(), SSL_FILETYPE_PEM) <= 0) 
        {
            _logger.error("Unable to load certificate");
            return false;
        }

        if (SSL_CTX_use_PrivateKey_file(_ctx, _keyFile.c_str(), SSL_FILETYPE_PEM) <= 0) 
        {
            _logger.error("Unable to load private key");
            return false;
        }

        _socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (_socket == -1) 
        {
            _logger.error("Socket creation failed");
            return false;
        }

        int opt = 1;
        if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
        {
            _logger.error("setsockopt failed");
            close();
            return false;
        }

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (::bind(_socket, (struct sockaddr*)&address, sizeof(address)) < 0) 
        {
            _logger.error("Bind failed");
            close();
            return false;
        }

        if (::listen(_socket, 3) < 0) 
        {
            _logger.error("Listen failed");
            close();
            return false;
        }

        return true;
    }

    int LinuxSSLListenerSocket::listen(std::vector<uint8_t>& buffer, size_t size)
    {
        if (_clientSocket == -1) 
        {
            sockaddr_in client_address{};
            socklen_t client_len = sizeof(client_address);
            
            _clientSocket = ::accept(_socket, (struct sockaddr*)&client_address, &client_len);
            if (_clientSocket < 0) 
            {
                _logger.error("Accept failed");
                return -1;
            }
            _logger.info("Client connected");

            _ssl = SSL_new(_ctx);
            SSL_set_fd(_ssl, _clientSocket);

            if (SSL_accept(_ssl) <= 0) 
            {
                _logger.error("SSL accept failed");
                return -1;
            }
        }

        char* tempBuffer = new char[size];
        int bytesRead = SSL_read(_ssl, tempBuffer, size);

        if (bytesRead > 0) 
        {
            buffer.assign(tempBuffer, tempBuffer + bytesRead);
        } 
        else if (bytesRead == 0) 
        {
            _logger.info("Client disconnected");
            cleanupSSL();
        }

        return bytesRead;
    }

    int LinuxSSLListenerSocket::respond(const std::vector<uint8_t>* data, size_t size)
    {
        if (!_ssl || !data || size == 0) 
        {
            _logger.error("No client connected or invalid data to respond with");
            return -1;
        }
        
        int bytesWritten = SSL_write(_ssl, data->data(), size);
        
        if (bytesWritten <= 0)
        {
            _logger.error("SSL write failed");
            cleanupSSL();
            return -1;
        }

        return bytesWritten;
    }

    void LinuxSSLListenerSocket::close()
    {
        cleanupSSL();
        if (_socket != -1) 
        {
            ::close(_socket);
            _socket = -1;
        }
    }

    void LinuxSSLListenerSocket::cleanupSSL()
    {
        if (_ssl) 
        {
            SSL_shutdown(_ssl);
            SSL_free(_ssl);
            _ssl = nullptr;
        }
        if (_ctx) 
        {
            SSL_CTX_free(_ctx);
            _ctx = nullptr;
        }
        if (_clientSocket != -1) 
        {
            ::close(_clientSocket);
            _clientSocket = -1;
        }
    }
}