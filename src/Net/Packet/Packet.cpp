#include "Net/Packet/Packet.h"

namespace Xale::Net
{
    Packet::Packet(CommandType cmd, std::vector<uint8_t> data, std::vector<uint8_t> tokenData)
        : command(cmd), payload(std::move(data)), token(std::move(tokenData)) 
    {}

    std::vector<uint8_t> Packet::serialize() const 
    {
        std::vector<uint8_t> buffer;

        buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&MAGIC_NUMBER),
            reinterpret_cast<const uint8_t*>(&MAGIC_NUMBER) + 4);
        
        buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&VERSION), 
            reinterpret_cast<const uint8_t*>(&VERSION) + 2);
        
        buffer.push_back(static_cast<uint8_t>(command));

        uint32_t length = size();
        
        buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&length),
            reinterpret_cast<const uint8_t*>(&length) + 4);

        if (!token.empty())
            buffer.insert(buffer.end(), token.begin(), token.end());
        
        if (!payload.empty())
            buffer.insert(buffer.end(), payload.begin(), payload.end());

        return buffer;
    }

    void Packet::deserialize(const std::vector<uint8_t>& buffer) 
    {
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

        token.clear();
        payload.clear();

        size_t tokenSize = std::min(static_cast<size_t>(256), static_cast<size_t>(length));
        if (tokenSize > 0)
            token.insert(token.end(), buffer.begin() + 11, buffer.begin() + 11 + tokenSize);

        if (length > tokenSize)
            payload.insert(payload.end(), buffer.begin() + 11 + tokenSize, buffer.begin() + 11 + length);
    }

    uint32_t Packet::size() const 
    {
        return static_cast<uint32_t>(token.size() + payload.size());
    }
}
