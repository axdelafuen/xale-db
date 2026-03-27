#ifndef QUERY_I_TOKENIZER_H
#define QUERY_I_TOKENIZER_H

#include "Query/Token.h"

#include <string>
#include <vector>

namespace Xale::Query
{
    /**
	 * @brief Interface for SQL tokenizer that converts an input string into a sequence of tokens.
     */
    class ITokenizer
    {
        public:
            /**
             * @brief Set the input SQL string to be tokenized
             * @param input The SQL query string
             */
            virtual void setInput(const std::string& input) = 0;

            /**
             * @brief Retrieve the next token from the input
             * @return The next token in the sequence
             */
            virtual Token nextToken() = 0;

            /**
             * @brief Peek at the next token without consuming it
             * @return The next token in the sequence
             */
            virtual Token peekToken() const = 0;

            /**
             * @brief Reset the tokenizer state to the beginning of the input
             */
            virtual void reset() = 0;

            /**
             * @brief Tokenize the entire input into a vector of tokens
             * @return A vector containing all tokens from the input
             */
            virtual std::vector<Token> tokenize() = 0;
    };
}

#endif // QUERY_I_TOKENIZER_H
