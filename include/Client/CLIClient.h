#ifndef CLIENT_CLI_CLIENT_H
#define CLIENT_CLI_CLIENT_H

#include "Client/IClient.h"

namespace Xale::Client
{
    class CLIClient : public IClient
    {
        public:
            /**
             * @brief Starts the CLI client interface
             */
            void start() override;
            
            /**
             * @brief Gets input from the command line
             * @param isExit A pointer to a boolean that will be set to true if the client wants to exit
             * @return The input string from the command line
             */
            std::string getInput(bool* isExit) override;
            
            /**
             * @brief Displays output to the command line
             * @param output The output string to be displayed
             */
            void displayOutput(const std::string& output) override;
            
            /**
             * @brief Closes the CLI client interface
             */
            void close() override;
    };
}

#endif // CLIENT_CLI_CLIENT_H
