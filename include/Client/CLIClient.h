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
             *
             * Handles special commands prefixed with '!':
             *   - !help         : prints a command reference and re-prompts
             *   - !file <path>  : reads a .sql file and returns its contents as the query
             *
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

        private:
            /**
             * @brief Prints the built-in help reference
             */
            void printHelp() const;

            /**
             * @brief Reads a SQL file and returns its contents
             * @param path Path to the .sql file
             * @return File contents as a string, or empty string on error
             */
            std::string readSqlFile(const std::string& path) const;
    };
}

#endif // CLIENT_CLI_CLIENT_H
