#ifndef NET_TCP_CLIENT_H
#define NET_TCP_CLIENT_H

#include "Net/Socket/SocketFactory.h"
#include "Net/Packet/Packet.h"

#include <string>
#include <memory>

namespace Xale::Net
{
    class TcpClient
    {
        public:
            TcpClient();
            ~TcpClient();
            
            /**
             * @brief Connects to a TCP server at the specified IP address and port.
             * @param ip The IP address of the server to connect to.
             * @param port The port number of the server to connect to.
             * @return True if the connection was successful, false otherwise.
             */
            bool connect(const std::string& ip, int port);
            
            /**
             * @brief Sends data to the connected TCP server.
             * @param data A pointer to the data to be sent.
             * @param size The size of the data to be sent in bytes.
             * @return The number of bytes sent, or -1 if an error occurred.
             * @deprecated Use send(const Xale::Net::Packet* data, size_t size) instead for better packet handling.
             */
            int send(const std::string* data, size_t size);

            /**
             * @brief Receives data from the connected TCP server.
             * @param buffer A pointer to a string where the received data will be stored.
             * @param size The maximum size of the data to be received in bytes.
             * @return The number of bytes received, or -1 if an error occurred.
             * @deprecated Use receive(Xale::Net::Packet* buffer, size_t size) instead for better packet handling.
             */
            int receive(std::string* buffer, size_t size);

            /**
             * @brief Sends a packet to the connected TCP server.
             * @param data A pointer to the packet to be sent.
             * @param size The size of the packet to be sent in bytes.
             * @return The number of bytes sent, or -1 if an error occurred.
             */
            int send(const Xale::Net::Packet* data, size_t size);

            /**
             * @brief Receives a packet from the connected TCP server.
             * @param buffer A pointer to the packet where the received data will be stored.
             * @param size The maximum size of the packet to be received in bytes.
             * @return The number of bytes received, or -1 if an error occurred.
             */
            int receive(Xale::Net::Packet* buffer, size_t size);

            /**
             * @brief Closes the connection to the TCP server.
             */
            void close();
        private:
            std::unique_ptr<Xale::Net::ISocket> _socket;

    };
}

#endif // NET_TCP_CLIENT_H
