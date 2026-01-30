#include <Logger.h>

#include "Core/Setup.h"
#include "Storage/BinaryFileManager.h"
#include "Storage/FileStorageEngine.h"
#include "Query/BasicTokenizer.h"
#include "Query/BasicParser.h"
#include "Execution/TableManager.h"
#include "Execution/BasicExecutor.h"
#include "Engine/QueryEngine.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

namespace Xale
{
    class Server{};
}

/**
 * @brief Server entrypoint
 */
int main()
{
    auto& logger = Xale::Logger::Logger<Xale::Server>::getInstance();

    std::string error;
    if (!Xale::Core::Init::setup(error))
    {
        logger.error("Setup failed: " + error);
        return -1;
    }
    
    // Setup Database Engine
    Xale::Storage::BinaryFileManager execFm;
	Xale::Storage::FileStorageEngine fileStorageEngine(execFm, "release-engine-storage.bin");
    if (!fileStorageEngine.startup())
	{
		logger.error("Executor StorageEngine startup failed");
		return -1;
	}
	
    // Setup parser
    Xale::Query::BasicTokenizer parserTokenizer;
    Xale::Query::BasicParser parser(&parserTokenizer);

    // Setup executor
    Xale::Execution::TableManager tableManager(fileStorageEngine, execFm);
	Xale::Execution::BasicExecutor executor(tableManager);

    // Setup engine
    Xale::Engine::QueryEngine queryEngine(&parser, &executor);

    // Setup TCP Server
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        logger.error("Socket creation failed");
        return -1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        logger.error("Bind failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        logger.error("Listen failed");
        close(server_fd);
        return -1;
    }

    logger.info("Server listening on port 8080...");

    // Accept connections and process queries
    while (true) {
        int client_socket;
        sockaddr_in client_address{};
        socklen_t client_len = sizeof(client_address);
        
        client_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_len);
        if (client_socket < 0) {
            logger.error("Accept failed");
            continue;
        }

        logger.info("Client connected");

        // Handle multiple queries from the same client
        while (true) {
            char buffer[4096] = {0};
            int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
            
            if (bytes_read <= 0) {
                logger.info("Client disconnected");
                break;
            }

            std::string query(buffer, bytes_read);
            logger.info("Received query: " + query);

            std::string response;
            // Execute query
            try 
            {
                queryEngine.run(query);
                response = queryEngine.getResultsToString();
            }
            catch (const std::exception& e)
            {
                std::string errorMsg = std::string("Error: ") + e.what();
                logger.error(errorMsg);
                response = errorMsg;
            }

            // Send response
            send(client_socket, response.c_str(), response.length(), 0);
            logger.info("Response sent");
        }

        close(client_socket);
    }

    // Cleanup
    close(server_fd);
    fileStorageEngine.shutdown();

	return 0;
}
