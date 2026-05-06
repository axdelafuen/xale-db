#ifndef NET_I_CLIENT_CONNECTION_H
#define NET_I_CLIENT_CONNECTION_H

#include <vector>
#include <cstdint>

namespace Xale::Net
{
    /**
     * @brief Represents an established per-client TCP connection
     *
     * Returned by IListenerSocket::acceptClient(). Each instance is independent
     * and safe to use from a dedicated thread.
     */
    class IClientConnection
    {
        public:
            virtual ~IClientConnection() = default;

            /**
             * @brief Read data from the client
             * @param buffer Buffer filled with received bytes
             * @param size   Maximum bytes to read
             * @return Bytes read, 0 on clean disconnect, <0 on error
             */
            virtual int read(std::vector<uint8_t>& buffer, size_t size) = 0;

            /**
             * @brief Send data to the client
             * @param data Data to send
             * @param size Number of bytes to send
             * @return Bytes sent, <0 on error
             */
            virtual int respond(const std::vector<uint8_t>* data, size_t size) = 0;

            /**
             * @brief Close this client connection
             */
            virtual void close() = 0;
    };
}

#endif // NET_I_CLIENT_CONNECTION_H
