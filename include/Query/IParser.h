#ifndef QUERY_I_PARSER_H
#define QUERY_I_PARSER_H

#include "Query/Statement.h"
#include "Query/ITokenizer.h"

#include <memory>
#include <string>

namespace Xale::Query
{
    /**
     * @brief Interface for SQL parsers
     */
    class IParser
    {
        public:
            virtual ~IParser() = default;

            /**
             * @brief Parse a SQL query string
             * @param query The SQL query to parse
             * @return Unique pointer to the parsed statement
             */
            virtual std::unique_ptr<Statement> parse(const std::string& query) = 0;
            
            /**
             * @brief Set the tokenizer to use
             * @param tokenizer Pointer to a tokenizer instance
             */
            virtual void setTokenizer(ITokenizer* tokenizer) = 0;
    };
}

#endif // QUERY_I_PARSER_H