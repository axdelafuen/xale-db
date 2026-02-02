#ifndef CLIENT_CLI_CLIENT_H
#define CLIENT_CLI_CLIENT_H

#include "Client/IClient.h"

namespace Xale::Client
{
    class CLIClient : public IClient
    {
        public:
            void start() override;
            std::string getInput(bool* isExit) override;
            void displayOutput(const std::string& output) override;
            void close();
    };
}

#endif // CLIENT_CLI_CLIENT_H