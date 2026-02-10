#include "Query/BasicTokenizer.h"

namespace Xale::Query
{
    /**
     * @brief Set the input string to tokenize
     */
    void BasicTokenizer::setInput(const std::string& input) 
    {
        _input = input;
        reset();
    }

    /**
     * @brief Reset the cursor and the current token
     */
    void BasicTokenizer::reset()
    {
        _pos = 0;
        _hasPeeked = false;
    }

    /**
     * @brief Get the next token
     */
    Token BasicTokenizer::nextToken()
    {
        if (_hasPeeked)
        {
            _hasPeeked = false;
            return _peekedToken;
        }
        return readToken();
    }
    
    /**
     * @brief Get the current token
     */
    Token BasicTokenizer::peekToken() const
    {
        if (!_hasPeeked)
        {
            _peekedToken = const_cast<BasicTokenizer*>(this)->readToken();
            _hasPeeked = true;
        }
        return _peekedToken;
    }

    /**
     * @brief Tokenize all the input
     */
    std::vector<Token> BasicTokenizer::tokenize()
    {
        std::vector<Token> tokens;
        Token token;

        do
        {
            token = nextToken();
            tokens.push_back(token);
        }
        while (token.type != TokenType::EndOfInput);

        return tokens;
    }

    /**
     * @brief Core method of this class, logic of the tokenization
     * This method is private
     */
    Token BasicTokenizer::readToken()
    {
        skipWhitespace();

        size_t start = _pos;

        if (isAtEnd())
        {
            return { TokenType::EndOfInput, "", _pos };
        }

        char c = goNextChar();

        // Identifiers / keywords
        if (std::isalpha(c) || c == '_')
        {
            while (!isAtEnd() && (std::isalnum(currentChar()) || currentChar() == '_'))
                goNextChar();

            std::string text = _input.substr(start, _pos - start);
            std::string upper = text;
            std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

            if (sql_definition_kw.count(upper))
                return { TokenType::DefinitionKeyword, text, start };

            if (sql_manipulation_kw.count(upper))
                return { TokenType::ManipulationKeyword, text, start };

            if (sql_query_kw.count(upper))
                return { TokenType::QueryKeyword, text, start };

            if (sql_join_kw.count(upper))
                return { TokenType::JoinKeyword, text, start };

            if (sql_logical_kw.count(upper))
                return { TokenType::LogicalKeyword, text, start };

            return { TokenType::Identifier, text, start };
        }

        // Numbers
        if (std::isdigit(c))
        {
            while (!isAtEnd() && std::isdigit(currentChar()))
                goNextChar();

            if (!isAtEnd() && currentChar() == '.')
            {
                goNextChar();
                while (!isAtEnd() && std::isdigit(currentChar()))
                    goNextChar();
            }

            return { 
                TokenType::NumericLiteral,
                _input.substr(start, _pos - start),
                start 
            };
        }

        // String literals: 'text'
        if (c == '\'')
        {
            while (!isAtEnd() && currentChar() != '\'')
                goNextChar();

            if (!isAtEnd())
                goNextChar();

            return { 
                TokenType::StringLiteral,
                _input.substr(start, _pos - start),
                start 
            };
        }

        // Operators
        // Try two-character operators
        if (!isAtEnd())
        {
            std::string twoChar = std::string(1, c) + std::string(1, currentChar());
            if (sql_operators.count(twoChar))
            {
                goNextChar();
                return { 
                    TokenType::Operator,
                    twoChar,
                    start
                };
            }
        }

        // Try single-character operators
        std::string singleChar = std::string(1, c);
        if (sql_operators.count(singleChar))
        {
            return { 
                TokenType::Operator,
                singleChar,
                start
            };
        }

        return { 
            TokenType::Unknown,
            std::string(1, c),
            start
        };
    }

    /* --------------------------------------------------
     * Helpers methods
     * --------------------------------------------------
     */

    void BasicTokenizer::skipWhitespace()
    {
        while (!isAtEnd() && std::isspace(currentChar()))
            goNextChar();
    }
    
    char BasicTokenizer::currentChar() const
    {
        return _input[_pos];
    }
    
    char BasicTokenizer::goNextChar()
    {
        return _input[_pos++];
    }

    bool BasicTokenizer::isAtEnd() const
    {
        return _pos >= _input.size();
    }
}
