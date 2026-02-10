#include "Client/CLIClient.h"

const std::string PROMPT = "xale-db> ";
const std::string EXIT_COMMANDS[] = { "exit", "quit" };

namespace Xale::Client
{
    void CLIClient::start()
    {
        std::cout << "CLI Client started. Type 'exit' to quit." << std::endl;
    }

    /**
     * @brief Gets input from the command line
     */
    std::string CLIClient::getInput(bool* isExit)
    {
        while (true) {
            std::string input;
            std::cout << PROMPT;
            std::getline(std::cin, input);

            if (input.empty())
                continue;

            for (const auto& cmd : EXIT_COMMANDS) {
                if (input == cmd) {
                    if (isExit) {
                        *isExit = true;
                    }
                    return "";
                }
            }

            return input;
        }
    }

    /**
     * @brief Displays output to the command line
     */
    void CLIClient::displayOutput(const std::string& output)
    {
        std::cout << output << std::endl;
    }

    /**
     * @brief Closes the client
     */
    void CLIClient::close()
    {
        std::cout << "Closing connection..." << std::endl;
    }
}