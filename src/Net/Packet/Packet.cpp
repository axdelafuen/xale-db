#include "Net/Packet/Packet.h"

namespace Xale::Net
{
    /**
     * @brief Constructs a Packet with command, payload, and optional token (token ignored for now)
     * @param cmd Command type
     * @param data Payload data
     * @param tokenData Optional authentication token (ignored)
     */
    Packet::Packet(CommandType cmd, std::vector<uint8_t> data, std::vector<uint8_t> tokenData)
        : command(cmd), payload(std::move(data)), token({}) // ignore token for now
    {}

    /**
     * @brief Serializes the packet to a byte vector (no token support for now)
     * @return Serialized packet as vector of bytes
     */
    std::vector<uint8_t> Packet::serialize() const 
    {
        std::vector<uint8_t> buffer;

        // Header: MAGIC_NUMBER (4 bytes), VERSION (2 bytes), command (1 byte), length (4 bytes)
        buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&MAGIC_NUMBER),
            reinterpret_cast<const uint8_t*>(&MAGIC_NUMBER) + 4);
        buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&VERSION), 
            reinterpret_cast<const uint8_t*>(&VERSION) + 2);
        buffer.push_back(static_cast<uint8_t>(command));

        uint32_t length = static_cast<uint32_t>(payload.size());
        buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&length),
            reinterpret_cast<const uint8_t*>(&length) + 4);

        // Only payload (no token)
        if (!payload.empty())
            buffer.insert(buffer.end(), payload.begin(), payload.end());

        return buffer;
    }

    /**
     * @brief Deserializes the packet from a byte vector (no token support for now)
     * @param buffer Byte vector containing serialized packet
     */
    void Packet::deserialize(const std::vector<uint8_t>& buffer) 
    {
        // Header: MAGIC_NUMBER (4 bytes), VERSION (2 bytes), command (1 byte), length (4 bytes)
        if (buffer.size() < 11)
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::PacketError, "Buffer too small for header");

        uint32_t magic;
        std::memcpy(&magic, buffer.data(), 4);
        if (magic != MAGIC_NUMBER)
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::PacketError, "Invalid magic number");

        uint16_t version;
        std::memcpy(&version, buffer.data() + 4, 2);
        if (version != VERSION)
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::PacketError, "Unsupported version");

        command = static_cast<CommandType>(buffer[6]);

        uint32_t length;
        std::memcpy(&length, buffer.data() + 7, 4);

        if (buffer.size() < 11 + length)
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::PacketError, "Buffer too small for payload");

        token.clear(); // not used
        payload.clear();

        // Only payload (no token)
        if (length > 0)
            payload.insert(payload.end(), buffer.begin() + 11, buffer.begin() + 11 + length);
    }

    /**
     * @brief Gets the total size of the packet in bytes (no token)
     * @return Size of the packet
     */
    uint32_t Packet::size() const 
    {
        return static_cast<uint32_t>(payload.size());
    }

    /**
     * @brief Gets the payload data of the packet
     * @return Payload as vector of bytes
     */
    std::vector<uint8_t> Packet::getPayload() const 
    {
        return payload;
    }
}
