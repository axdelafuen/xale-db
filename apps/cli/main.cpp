#include "Net/TcpClient.h"
#include "Net/Packet/Packet.h"
#include "Net/Packet/PacketConstants.h"
#include "Client/CLIClient.h"
#include "Net/Socket/BasicSocketFactory.h"
#include "Net/Socket/SSLSocketFactory.h"

#include <iostream>
#include <string>
#include <csignal>

/**
 * @brief CLI entrypoint
 */
int main(int argc, char* argv[])
{
    std::signal(SIGPIPE, SIG_IGN);
    auto cliClient = Xale::Client::CLIClient(); // use abstraction in the future

    // check if SSL is disable from command line arguments
    bool useSSL = true;
    for (int i = 1; i < argc; ++i) 
    {
        std::string arg = argv[i];
        if (arg == "--no-ssl") 
        {
            useSSL = false;
            break;
        }
    }

    std::unique_ptr<Xale::Net::ISocketFactory> socketFactory;
    if (useSSL) 
        socketFactory = std::make_unique<Xale::Net::SSLSocketFactory>("", "");
    else 
        socketFactory = std::make_unique<Xale::Net::BasicSocketFactory>();

    auto tcpClient = Xale::Net::TcpClient(std::move(socketFactory));
    if (!tcpClient.connect("127.0.0.1", 8080)) {
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

        Xale::Net::Packet packet(Xale::Net::CommandType::QUERY, std::vector<uint8_t>(query.begin(), query.end()));
        tcpClient.send(&packet, packet.size());

        Xale::Net::Packet responsePacket(Xale::Net::CommandType::RESPONSE, {});
        int bytes_read = tcpClient.receive(&responsePacket, 4096);

        if (bytes_read > 0) {
            // Convert payload to string for display
            const auto& payload = responsePacket.getPayload();
            std::string output(payload.begin(), payload.end());
            cliClient.displayOutput(output);
        } else if (bytes_read == 0) {
            cliClient.displayOutput("Server closed the connection.");
            break;
        } else {
            cliClient.displayOutput("Error receiving response.");
            break;
        }
    }

    tcpClient.close();
	return 0;
}
