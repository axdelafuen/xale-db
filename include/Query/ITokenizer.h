#ifndef QUERY_I_TOKENIZER_H
#define QUERY_I_TOKENIZER_H

#include "Query/Token.h"

#include <string>
#include <vector>

namespace Xale::Query
{
    class ITokenizer
    {
        public:
            virtual void setInput(const std::string& input) = 0;
            virtual Token nextToken() = 0;
            virtual Token peekToken() const = 0;
            virtual void reset() = 0;
            virtual std::vector<Token> tokenize() = 0;
    };
}

#endif // QUERY_I_TOKENIZER_H
