#ifndef NET_PACKET_H
#define NET_PACKET_H

#include "Net/Packet/IPacket.h"
#include "Net/Packet/PacketConstants.h"
#include "Core/ExceptionHandler.h"

#include <cstring>

namespace Xale::Net
{
    /**
     * @brief Represents a network packet for communication in Xale DB.
     */
    class Packet : public IPacket {
        private:
            /**
             * @brief Command type of the packet.
             */
            CommandType command;
            /**
             * @brief Payload data of the packet.
             */
            std::vector<uint8_t> payload;
            /**
             * @brief Optional authentication token.
             */
            std::vector<uint8_t> token;
        public:
            /**
             * @brief Constructs a Packet with command, payload, and optional token.
             * @param cmd Command type
             * @param data Payload data
             * @param tokenData Optional authentication token
             */
            Packet(CommandType cmd, std::vector<uint8_t> data, std::vector<uint8_t> tokenData = {});
            /**
             * @brief Serializes the packet to a byte vector.
             * @return Serialized packet as vector of bytes
             */
            std::vector<uint8_t> serialize() const override;
            /**
             * @brief Deserializes the packet from a byte vector.
             * @param buffer Byte vector containing serialized packet
             */
            void deserialize(const std::vector<uint8_t>& buffer) override;
            /**
             * @brief Gets the total size of the packet in bytes.
             * @return Size of the packet
             */
            uint32_t size() const override;
    };
}

#endif // NET_PACKET_H
