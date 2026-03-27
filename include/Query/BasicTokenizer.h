#ifndef QUERY_BASIC_TOKENIZER_H
#define QUERY_BASIC_TOKENIZER_H

#include "Query/ITokenizer.h"

#include <cctype>
#include <algorithm>

namespace Xale::Query
{
    /**
     * @brief Basic implementation of SQL tokenizer
	 */
    /**
     * @brief Basic implementation of SQL tokenizer
     */
    class BasicTokenizer : public ITokenizer
    {
        public:
            /**
             * @brief Set the input string to tokenize
             */
            void setInput(const std::string& input) override;

            /**
             * @brief Get the next token
             */
            Token nextToken() override;

            /**
             * @brief Get the current token
             */
            Token peekToken() const override;

            /**
             * @brief Reset the cursor and the current token
             */
            void reset() override;

            /**
             * @brief Tokenize all the input
             */
            std::vector<Token> tokenize() override;
        private:
            /**
             * @brief Core method of this class, logic of the tokenization
             * This method is private
             */
            Token readToken();

            void skipWhitespace();
            char currentChar() const;
            char goNextChar();
            bool isAtEnd() const;

            std::string _input;
            size_t _pos = 0;
            mutable bool _hasPeeked = false;
            mutable Token _peekedToken;
    };
}

#endif // BASIC_QUERY_TOKENIZER_H
