#include <Logger.h>

#include "Core/Setup.h"
#include "Engine/QueryEngine.h"
#include "Net/TcpServer.h"

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

    // Start server
    auto& queryEngine = setup.getQueryEngine();
    Xale::Net::TcpServer server(queryEngine);

    if (!server.start(8080)) {
        logger.error("Failed to start the server");
        return -1;
    }

    server.stop();
    setup.shutdown();
    
	return 0;
}
