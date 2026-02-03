#include "Net/TcpServer.h"

namespace Xale::Net
{
    /*
     * @brief Constructor
     */
    TcpServer::TcpServer(Xale::Engine::QueryEngine& queryEngine) :
        _logger(Xale::Logger::Logger<TcpServer>::getInstance()),
        _serverSocket(nullptr),
        _queryEngine(queryEngine)
    {}

    /*
     * @brief Destructor
     */
    TcpServer::~TcpServer()
    {
        stop();
    }

    /*
     * @brief Starts the TCP server on the specified port
     */
    bool TcpServer::start(int port)
    {
        _serverSocket = Xale::Net::SocketFactory::createListenerSocket();
        if (!_serverSocket->open(port)) {
            _logger.error("Failed to open listener socket");
            return false;
        }

        _logger.info("Server listening on port " + std::to_string(port) + "...");
        while (true) {
            std::string buffer;
            int bytesRead = _serverSocket->listen(buffer, 4096);
            if (bytesRead <= 0) {
                _logger.error("Failed to read from client");
                continue;
            }

            _logger.info("Received query: " + buffer);
            std::string response;
            // Execute query
            try 
            {
                _queryEngine.run(buffer);
                response = _queryEngine.getResultsToString();
            }
            catch (const std::exception& e)
            {
                std::string errorMsg = std::string("Error: ") + e.what();
                _logger.error(errorMsg);
                response = errorMsg;
            }

            // Send response
            _serverSocket->respond(&response, response.length());
            _logger.info("Response sent");
        }

        stop();
        return true;
    }

    /*
     * @brief Stops the TCP server
     */
    void TcpServer::stop()
    {
        if (_serverSocket) {
            _serverSocket->close();
            _logger.info("Server stopped");
        }
    }
}