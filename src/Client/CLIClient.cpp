#include "Client/CLIClient.h"

#include <fstream>
#include <sstream>

const std::string PROMPT = "xale-db> ";
const std::string EXIT_COMMANDS[] = { "exit", "quit" };

namespace Xale::Client
{
    void CLIClient::start()
    {
        std::cout << "CLI Client started. Type '!help' for commands or 'exit' to quit." << std::endl;
    }

    std::string CLIClient::getInput(bool* isExit)
    {
        while (true) {
            std::string input;
            std::cout << PROMPT;
            std::getline(std::cin, input);

            if (input.empty())
                continue;

            // Check exit commands
            for (const auto& cmd : EXIT_COMMANDS) {
                if (input == cmd) {
                    if (isExit) *isExit = true;
                    return "";
                }
            }

            // Handle ! commands
            if (!input.empty() && input[0] == '!')
            {
                // Extract command and optional argument
                std::string cmd = input.substr(1);
                std::string arg;
                auto space = cmd.find(' ');
                if (space != std::string::npos)
                {
                    arg = cmd.substr(space + 1);
                    cmd = cmd.substr(0, space);
                }

                // Trim arg whitespace
                while (!arg.empty() && (arg.front() == ' ' || arg.front() == '\t')) arg.erase(arg.begin());
                while (!arg.empty() && (arg.back()  == ' ' || arg.back()  == '\t')) arg.pop_back();

                if (cmd == "help")
                {
                    printHelp();
                    continue;
                }
                else if (cmd == "file")
                {
                    if (arg.empty())
                    {
                        std::cerr << "Usage: !file <filename.sql>" << std::endl;
                        continue;
                    }
                    std::string content = readSqlFile(arg);
                    if (content.empty())
                        continue; // error already printed
                    return content;
                }
                else
                {
                    std::cerr << "Unknown command: !" << cmd << "  (try !help)" << std::endl;
                    continue;
                }
            }

            return input;
        }
    }

    void CLIClient::displayOutput(const std::string& output)
    {
        std::cout << output << std::endl;
    }

    void CLIClient::close()
    {
        std::cout << "Closing connection..." << std::endl;
    }

    void CLIClient::printHelp() const
    {
        std::cout <<
            "\n"
            "  xale-db — SQL command reference\n"
            "  ─────────────────────────────────────────────────────────\n"
            "  DDL\n"
            "    CREATE TABLE t (col TYPE [PRIMARY KEY] [REFERENCES t2(col2)], ...)\n"
            "    DROP TABLE t\n"
            "    LIST TABLE\n"
            "\n"
            "  DML\n"
            "    INSERT INTO t VALUES (v1, v2, ...)   -- parens optional\n"
            "    SELECT * FROM t [JOIN t2 ON t.c = t2.c] [WHERE col OP val]\n"
            "    UPDATE t SET col = val [, ...] [WHERE col OP val]\n"
            "    DELETE FROM t [WHERE col OP val]\n"
            "\n"
            "  Supported types : INT, FLOAT, VARCHAR / TEXT / STRING\n"
            "  Operators       : =  !=  <  >  <=  >=\n"
            "  String literals : 'single' or \"double\" quotes\n"
            "  Multi-query     : separate statements with ';'\n"
            "\n"
            "  CLI commands\n"
            "    !help              show this help\n"
            "    !file <path.sql>   execute all queries from a SQL file\n"
            "    exit / quit        close the connection\n"
            "  ─────────────────────────────────────────────────────────\n"
            << std::endl;
    }

    std::string CLIClient::readSqlFile(const std::string& path) const
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Error: cannot open file '" << path << "'" << std::endl;
            return "";
        }
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string content = ss.str();
        if (content.empty())
        {
            std::cerr << "Warning: file '" << path << "' is empty" << std::endl;
            return "";
        }
        return content;
    }
}