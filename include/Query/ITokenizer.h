#ifndef QUERY_I_TOKENIZER_H
#define QUERY_I_TOKENIZER_H

#include <string>
#include <vector>

namespace Xale::Query
{
    class ITokenizer
    {
        public:
            virtual std::vector<std::string> tokenize(
                    const std::string& input) = 0;
    };
}

#endif // QUERY_I_TOKENIZER_H
