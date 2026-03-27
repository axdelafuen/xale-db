#include <Logger.h>

#include "Core/Setup.h"
#include "Engine/QueryEngine.h"
#include "Net/TcpServer.h"
#include "Net/Socket/BasicSocketFactory.h"

/**
 * @brief Server entrypoint
 */
int main()
{
    // Setup
    auto& logger = Xale::Logger::Logger<Xale::Net::TcpServer>::getInstance();
    auto& setup = Xale::Core::Setup::getInstance();
    if (!setup.initialize())
        return -1;

    std::unique_ptr<Xale::Net::ISocketFactory> socketFactory = std::move(setup.getSocketFactory());

    // Start server
    auto& queryEngine = setup.getQueryEngine();
    Xale::Net::TcpServer server(queryEngine, std::move(socketFactory));

    if (!server.start(8080)) {
        logger.error("Failed to start the server");
        return -1;
    }

    server.stop();
    setup.shutdown();
    
	return 0;
}
