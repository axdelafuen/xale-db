#include "Net/Socket/SocketFactory.h"
#include "Client/CLIClient.h"

#include <iostream>
#include <string>

/**
 * @brief CLI entrypoint
 */
int main(int argc, char* argv[])
{
    auto socket = Xale::Net::SocketFactory::createSocket();
    auto cliClient = Xale::Client::CLIClient(); // use abstraction in the future

    if (!socket->connect("127.0.0.1", 8080)) {
        std::cerr << "Connection failed. Is the server running?" << std::endl;
        return -1;
    }

    cliClient.start();

    while (true) {
        bool isExit = false;
        std::string query = cliClient.getInput(&isExit);

        if (isExit) {
            cliClient.close();
            break;
        }

        socket->send(&query, query.length());

        std::string buffer;
        int bytes_read = socket->receive(&buffer, 4096);
        
        if (bytes_read > 0) {
            cliClient.displayOutput(buffer);
        } else if (bytes_read == 0) {
            cliClient.displayOutput("Server closed the connection.");
            break;
        } else {
            cliClient.displayOutput("Error receiving response.");
            break;
        }
    }

    socket->close();
	return 0;
}
