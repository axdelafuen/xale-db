#ifndef NET_I_LISTENER_SOCKET_H
#define NET_I_LISTENER_SOCKET_H

#include "Net/Socket/IClientConnection.h"

#include <memory>
#include <vector>
#include <cstdint>

namespace Xale::Net
{
    /**
     * @brief Server-side socket: binds a port and produces per-client connections
     */
    class IListenerSocket
    {
        public:
            virtual ~IListenerSocket() = default;

            /**
             * @brief Bind and start listening on the given port
             * @param port TCP port to listen on
             * @return True on success
             */
            virtual bool open(int port) = 0;

            /**
             * @brief Block until a new client connects, then return a connection object
             * @return Unique pointer to an IClientConnection, or nullptr on error
             */
            virtual std::unique_ptr<IClientConnection> acceptClient() = 0;

            /**
             * @brief Close the listening socket
             */
            virtual void close() = 0;
    };
}

#endif // NET_I_LISTENER_SOCKET_H
