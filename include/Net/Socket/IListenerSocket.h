#ifndef NET_I_LISTENER_SOCKET_H
#define NET_I_LISTENER_SOCKET_H

#include <vector>
#include <cstdint>

namespace Xale::Net
{
    class IListenerSocket
    {
        public:
            virtual bool open(int port) = 0;
            virtual int listen(std::vector<uint8_t>& buffer, size_t size) = 0;
            virtual int respond(const std::vector<uint8_t>* data, size_t size) = 0;
            virtual void close() = 0;
    };
}

#endif // NET_I_LISTENER_SOCKET_H
