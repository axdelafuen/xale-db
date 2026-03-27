#ifndef PACKET_TESTS_H
#define PACKET_TESTS_H

#include "TestsHelper.h"
#include "Net/Packet/Packet.h"
#include <vector>
#include <cstdint>
#include <cstring>

#define DECLARE_PACKET_TEST(name) DECLARE_TEST(NET, packet_##name)

namespace Xale::Tests 
{
    DECLARE_PACKET_TEST(construct_and_access) 
    {
        std::vector<uint8_t> payload = {1, 2, 3, 4};
        Xale::Net::Packet pkt(Xale::Net::CommandType::QUERY, payload);
        return pkt.getPayload() == payload && pkt.size() == payload.size();
    }

    DECLARE_PACKET_TEST(serialize_deserialize_roundtrip) 
    {
        std::vector<uint8_t> payload = {10, 20, 30, 40, 50};
        Xale::Net::Packet pkt(Xale::Net::CommandType::RESPONSE, payload);
        std::vector<uint8_t> data = pkt.serialize();
        Xale::Net::Packet pkt2(Xale::Net::CommandType::UNKNOWN, {});
        pkt2.deserialize(data);
        return pkt2.getPayload() == payload && pkt2.size() == payload.size();
    }

    DECLARE_PACKET_TEST(empty_payload) 
    {
        std::vector<uint8_t> payload = {};
        Xale::Net::Packet pkt(Xale::Net::CommandType::AUTH, payload);
        std::vector<uint8_t> data = pkt.serialize();
        Xale::Net::Packet pkt2(Xale::Net::CommandType::UNKNOWN, {});
        pkt2.deserialize(data);
        return pkt2.getPayload().empty() && pkt2.size() == 0;
    }

    DECLARE_PACKET_TEST(invalid_magic_throws) 
    {
        std::vector<uint8_t> data(11, 0); // too small and wrong magic
        bool threw = false;
        Xale::Net::Packet pkt(Xale::Net::CommandType::UNKNOWN, {});
        try 
        {
            pkt.deserialize(data);
        } catch (const Xale::Core::DbException& e) 
        {
            threw = (e.getCode() == Xale::Core::ExceptionCode::PacketError);
        }
        return threw;
    }

    DECLARE_PACKET_TEST(command_type_correctness) 
    {
        std::vector<uint8_t> payload = {99, 100};
        Xale::Net::Packet pkt(Xale::Net::CommandType::AUTH, payload);
        std::vector<uint8_t> data = pkt.serialize();
        Xale::Net::Packet pkt2(Xale::Net::CommandType::UNKNOWN, {});
        pkt2.deserialize(data);
        // Check that command type is AUTH after deserialization
        // We can't access command directly, but can check by re-serializing and comparing bytes
        std::vector<uint8_t> data2 = pkt2.serialize();
        // Command type is at offset 6
        return data2.size() >= 7 && data2[6] == static_cast<uint8_t>(Xale::Net::CommandType::AUTH);
    }
}

#endif // PACKET_TESTS_H
