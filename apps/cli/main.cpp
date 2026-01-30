#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

/**
 * @brief CLI entrypoint
 */
int main(int argc, char* argv[])
{
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Connect to server
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed. Is the server running?" << std::endl;
        close(sock);
        return -1;
    }

    std::cout << "Connected to server. Type 'exit' to quit." << std::endl;

    // Loop to send multiple queries
    while (true) {
        std::string query;
        std::cout << "\nxale-db> ";
        std::getline(std::cin, query);

        if (query.empty()) {
            continue;
        }

        if (query == "exit" || query == "quit") {
            std::cout << "Closing connection..." << std::endl;
            break;
        }

        // Send query
        send(sock, query.c_str(), query.length(), 0);

        // Receive response
        char buffer[4096] = {0};
        int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
        
        if (bytes_read > 0) {
            std::string response(buffer, bytes_read);
            std::cout << response << std::endl;
        } else if (bytes_read == 0) {
            std::cout << "Server closed connection" << std::endl;
            break;
        } else {
            std::cerr << "Error receiving response" << std::endl;
            break;
        }
    }

    close(sock);
	return 0;
}
