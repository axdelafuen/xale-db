#ifndef CLIENT_I_CLIENT_H
#define CLIENT_I_CLIENT_H

#include <string>
#include <iostream>

namespace Xale::Client
{
    class IClient
    {
        public:
            virtual void start() = 0;
            virtual std::string getInput(bool* isExit) = 0;
            virtual void displayOutput(const std::string& output) = 0;
            virtual void close() = 0;
    };
}

#endif // CLIENT_I_CLIENT_H