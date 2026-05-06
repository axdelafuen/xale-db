#include "Net/TcpServer.h"

#include <thread>

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
            auto conn = _serverSocket->acceptClient();
            if (!conn) {
                _logger.error("Accept failed, retrying...");
                continue;
            }

            // Spawn a detached thread per client — each thread owns its connection
            std::thread([this, c = std::move(conn)]() mutable {
                handleClient(std::move(c));
            }).detach();
        }

        stop();
        return true;
    }

    void TcpServer::handleClient(std::unique_ptr<IClientConnection> conn)
    {
        _logger.info("Client handler started");

        while (true) {
            std::vector<uint8_t> buffer;
            int bytesRead = conn->read(buffer, 4096);

            if (bytesRead == 0) {
                // Clean disconnect
                break;
            }
            if (bytesRead < 0) {
                _logger.error("Read error from client");
                break;
            }

            // Deserialize incoming buffer to Packet
            Xale::Net::Packet packet(Xale::Net::CommandType::UNKNOWN, {});
            try {
                packet.deserialize(buffer);
            } catch (const std::exception& e) {
                std::string errorMsg = std::string("Packet error: ") + e.what();
                _logger.error(errorMsg);
                Xale::Net::Packet errorPacket(Xale::Net::CommandType::RESPONSE,
                    std::vector<uint8_t>(errorMsg.begin(), errorMsg.end()));
                auto serialized = errorPacket.serialize();
                conn->respond(&serialized, serialized.size());
                continue;
            }

            std::vector<uint8_t> payload = packet.getPayload();
            std::string query(payload.begin(), payload.end());
            _logger.info("Received query: " + query);

            std::string response;
            try {
                std::lock_guard<std::mutex> lock(_queryMutex);
                _queryEngine.run(query);
                response = _queryEngine.getResultsToString();
            } catch (const std::exception& e) {
                response = std::string("Error: ") + e.what();
                _logger.error(response);
            }

            Xale::Net::Packet responsePacket(Xale::Net::CommandType::RESPONSE,
                std::vector<uint8_t>(response.begin(), response.end()));
            auto serialized = responsePacket.serialize();
            conn->respond(&serialized, serialized.size());
            _logger.info("Response sent");
        }

        conn->close();
        _logger.info("Client handler finished");
    }

    void TcpServer::stop()
    {
        if (_serverSocket) {
            _serverSocket->close();
            _logger.info("Server stopped");
        }
    }
}
