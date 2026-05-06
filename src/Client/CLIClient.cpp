#include "Client/CLIClient.h"

#include <fstream>
#include <sstream>
#include <termios.h>
#include <unistd.h>

const std::string PROMPT = "xale-db> ";
const std::string EXIT_COMMANDS[] = { "exit", "quit" };

namespace Xale::Client
{
    void CLIClient::start()
    {
        std::cout << "CLI Client started. Type '!help' for commands or 'exit' to quit." << std::endl;
    }

    std::string CLIClient::readLine()
    {
        // Fall back to plain getline when stdin is not a terminal (pipes, tests, etc.)
        if (!isatty(STDIN_FILENO))
        {
            std::string line;
            std::getline(std::cin, line);
            return line;
        }

        struct termios orig, raw;
        tcgetattr(STDIN_FILENO, &orig);
        raw = orig;
        raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
        raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
        raw.c_cflag |= CS8;
        raw.c_cc[VMIN]  = 1;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

        std::string buf;
        size_t cursor   = 0;
        int    histIdx  = static_cast<int>(_history.size()); // past-end = "current draft"
        std::string draft;                                    // saves what was typed before browsing history

        // Redraw current line in place
        auto redraw = [&]() {
            std::cout << "\r\x1b[K" << PROMPT << buf;
            if (cursor < buf.size())
                std::cout << "\x1b[" << (buf.size() - cursor) << "D";
            std::cout.flush();
        };

        redraw();

        while (true)
        {
            char c;
            if (::read(STDIN_FILENO, &c, 1) != 1) break;

            if (c == '\r' || c == '\n')
            {
                std::cout << "\r\n";
                break;
            }
            else if (c == '\x03') // Ctrl+C
            {
                buf.clear();
                std::cout << "\r\n";
                break;
            }
            else if (c == '\x7f' || c == '\x08') // Backspace
            {
                if (cursor > 0)
                {
                    buf.erase(cursor - 1, 1);
                    --cursor;
                    redraw();
                }
            }
            else if (c == '\x1b') // Escape sequence
            {
                char seq[2] = {0};
                if (::read(STDIN_FILENO, &seq[0], 1) != 1) continue;

                if (seq[0] == '[')
                {
                    if (::read(STDIN_FILENO, &seq[1], 1) != 1) continue;

                    if (seq[1] == 'A') // Up arrow — history previous
                    {
                        if (histIdx > 0)
                        {
                            if (histIdx == static_cast<int>(_history.size()))
                                draft = buf;
                            --histIdx;
                            buf    = _history[histIdx];
                            cursor = buf.size();
                            redraw();
                        }
                    }
                    else if (seq[1] == 'B') // Down arrow — history next
                    {
                        if (histIdx < static_cast<int>(_history.size()))
                        {
                            ++histIdx;
                            buf    = (histIdx == static_cast<int>(_history.size())) ? draft : _history[histIdx];
                            cursor = buf.size();
                            redraw();
                        }
                    }
                    else if (seq[1] == 'C') // Right arrow
                    {
                        if (cursor < buf.size())
                        {
                            ++cursor;
                            redraw();
                        }
                    }
                    else if (seq[1] == 'D') // Left arrow
                    {
                        if (cursor > 0)
                        {
                            --cursor;
                            redraw();
                        }
                    }
                    else if (seq[1] == '3') // Delete key: ESC[3~
                    {
                        char tilde;
                        if (::read(STDIN_FILENO, &tilde, 1) == 1 && tilde == '~')
                        {
                            if (cursor < buf.size())
                            {
                                buf.erase(cursor, 1);
                                redraw();
                            }
                        }
                    }
                    else if (seq[1] == 'H' || seq[1] == '1') // Home: ESC[H or ESC[1~
                    {
                        if (seq[1] == '1') { char t; ::read(STDIN_FILENO, &t, 1); }
                        cursor = 0;
                        redraw();
                    }
                    else if (seq[1] == 'F' || seq[1] == '4') // End: ESC[F or ESC[4~
                    {
                        if (seq[1] == '4') { char t; ::read(STDIN_FILENO, &t, 1); }
                        cursor = buf.size();
                        redraw();
                    }
                }
                else if (seq[0] == 'O') // SS3 sequences (some terminals)
                {
                    char ch;
                    if (::read(STDIN_FILENO, &ch, 1) == 1)
                    {
                        if (ch == 'H') { cursor = 0;          redraw(); } // Home
                        if (ch == 'F') { cursor = buf.size(); redraw(); } // End
                    }
                }
            }
            else if (static_cast<unsigned char>(c) >= 32) // Printable character
            {
                buf.insert(cursor, 1, c);
                ++cursor;
                redraw();
            }
        }

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
        return buf;
    }

    std::string CLIClient::getInput(bool* isExit)
    {
        while (true) {
            std::string input;
            input = readLine();

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
                    _history.push_back(input);
                    return content;
                }
                else
                {
                    std::cerr << "Unknown command: !" << cmd << "  (try !help)" << std::endl;
                    continue;
                }
            }

            // Save to history (avoid consecutive duplicates)
            if (_history.empty() || _history.back() != input)
                _history.push_back(input);

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