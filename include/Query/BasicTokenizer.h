#ifndef QUERY_BASIC_TOKENIZER_H
#define QUERY_BASIC_TOKENIZER_H

#include "Query/ITokenizer.h"

#include <cctype>
#include <algorithm>

namespace Xale::Query
{
    class BasicTokenizer : public ITokenizer
    {
        public:
            void setInput(const std::string& input) override;
            Token nextToken() override;
            Token peekToken() const override;
            void reset() override;
            std::vector<Token> tokenize() override;
        private:
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
