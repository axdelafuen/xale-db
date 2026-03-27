#ifndef CLIENT_I_CLIENT_H
#define CLIENT_I_CLIENT_H

#include <string>
#include <iostream>

namespace Xale::Client
{
    class IClient
    {
        public:
            /**
             * @brief Starts the client interface
             */

            virtual void start() = 0;
            /**
             * @brief Gets input from the client
             * @param isExit A pointer to a boolean that will be set to true if the client wants to exit
             * @return The input string from the client
             */

            virtual std::string getInput(bool* isExit) = 0;
            /**
             * @brief Displays output to the client
             * @param output The output string to be displayed
             */
            
            virtual void displayOutput(const std::string& output) = 0;
            /**
             * @brief Closes the client interface
             */
            virtual void close() = 0;
    };
}

#endif // CLIENT_I_CLIENT_H