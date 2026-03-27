#ifndef NET_I_PACKET_H
#define NET_I_PACKET_H

#include <vector>
#include <cstdint>

namespace Xale::Net
{
    class IPacket
    {
        public:
            virtual ~IPacket() = default;
            virtual std::vector<uint8_t> serialize() const = 0;
            virtual void deserialize(const std::vector<uint8_t>& buffer) = 0;
            virtual uint32_t size() const = 0;
    };
}

#endif // NET_I_SOCKET_H
