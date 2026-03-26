#ifndef NET_PACKET_CONST_H
#define NET_PACKET_CONST_H

#include <cstdint>

constexpr uint32_t MAGIC_NUMBER = 0x58414C45; // "XALE"
constexpr uint16_t VERSION = 0x0100;          // 1.0

enum class CommandType : uint8_t 
{
    AUTH = 0x01,
    QUERY = 0x02,
    RESPONSE = 0x03
};

#endif // NET_PACKET_CONST_H
