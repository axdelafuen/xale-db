#include "Net/TcpServer.h"

namespace Xale::Net
{
    TcpServer::TcpServer(Xale::Engine::QueryEngine& queryEngine, std::unique_ptr<Xale::Net::ISocketFactory> socketFactory) :
        _logger(Xale::Logger::Logger<TcpServer>::getInstance()),
        _serverSocket(nullptr),
        _socketFactory(std::move(socketFactory)),
        _queryEngine(queryEngine)
    {}

    TcpServer::~TcpServer()
    {
        stop();
    }

    bool TcpServer::start(int port)
    {
        _serverSocket = _socketFactory->createListenerSocket();
        if (!_serverSocket->open(port)) {
            _logger.error("Failed to open listener socket");
            return false;
        }

        _logger.info("Server listening on port " + std::to_string(port) + "...");
        while (true) {
            std::vector<uint8_t> buffer;
            int bytesRead = _serverSocket->listen(buffer, 4096);
            if (bytesRead <= 0) {
                _logger.error("Failed to read from client");
                continue;
            }

            // Deserialize incoming buffer to Packet
            Xale::Net::Packet packet(Xale::Net::CommandType::UNKNOWN, {});
            try {
                std::vector<uint8_t> data(buffer.begin(), buffer.end());
                packet.deserialize(data);
            } catch (const std::exception& e) {
                std::string errorMsg = std::string("Packet error: ") + e.what();
                _logger.error(errorMsg);
                // Respond with error packet
                Xale::Net::Packet errorPacket(Xale::Net::CommandType::RESPONSE, std::vector<uint8_t>(errorMsg.begin(), errorMsg.end()));
                auto errorSerialized = errorPacket.serialize();
                _serverSocket->respond(&errorSerialized, errorSerialized.size());
                continue;
            }

            std::vector<uint8_t> payload = packet.getPayload();
            std::string query(payload.begin(), payload.end());
            _logger.info("Received query: " + query);
            std::string response;
            try {
                _queryEngine.run(query);
                response = _queryEngine.getResultsToString();
            } catch (const std::exception& e) {
                std::string errorMsg = std::string("Error: ") + e.what();
                _logger.error(errorMsg);
                response = errorMsg;
            }

            // Send response as Packet
            Xale::Net::Packet responsePacket(Xale::Net::CommandType::RESPONSE, std::vector<uint8_t>(response.begin(), response.end()));
            auto serialized = responsePacket.serialize();
            _serverSocket->respond(&serialized, serialized.size());
            _logger.info("Response sent");
        }

        stop();
        return true;
    }

    void TcpServer::stop()
    {
        if (_serverSocket) {
            _serverSocket->close();
            _logger.info("Server stopped");
        }
    }
}