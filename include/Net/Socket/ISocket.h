#ifndef NET_I_SOCKET_H
#define NET_I_SOCKET_H

#include <string>
#include <vector>
#include <cstdint>

namespace Xale::Net
{
    class ISocket
    {
        public:
            virtual bool connect(const std::string& hostAddress, int port) = 0;
            virtual int send(const std::vector<uint8_t>* data, size_t size) = 0;
            virtual int receive(std::vector<uint8_t>* buffer, size_t size) = 0;
            virtual void close() = 0;
    };
}

#endif // NET_I_SOCKET_H
