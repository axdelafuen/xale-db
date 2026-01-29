#ifndef QUERY_BASIC_PARSER_H
#define QUERY_BASIC_PARSER_H

#include "Core/ExceptionHandler.h"
#include "Query/IParser.h"
#include "Query/ITokenizer.h"
#include "Query/Token.h"
#include "Query/Statement.h"

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>

namespace Xale::Query
{
    /*
     * @brief Basic implementation of SQL parser
     */
    class BasicParser : public IParser
    {
        public:
            BasicParser();
            explicit BasicParser(ITokenizer* tokenizer);
            std::unique_ptr<Statement> parse(const std::string& query) override;
            void setTokenizer(ITokenizer* tokenizer) override;

        private:
            ITokenizer* _tokenizer;
            Token _currentToken;

            void advance();
            bool match(TokenType type);
            bool matchKeyword(const std::string& keyword);
            bool matchIdentifier(const std::string& identifier);
            void expect(TokenType type, const std::string& errorMsg);
            void expectKeyword(const std::string& keyword, const std::string& errorMsg);
            [[noreturn]] void throwError(const std::string& message);

            std::unique_ptr<Statement> parseStatement();
            std::unique_ptr<SelectStatement> parseSelect();
            std::unique_ptr<InsertStatement> parseInsert();
            std::unique_ptr<UpdateStatement> parseUpdate();
            std::unique_ptr<DeleteStatement> parseDelete();
            std::unique_ptr<CreateStatement> parseCreate();
            std::unique_ptr<DropStatement> parseDrop();

            std::unique_ptr<Expression> parseExpression();
            std::unique_ptr<Expression> parseComparison();
            std::unique_ptr<Expression> parsePrimary();

            std::unique_ptr<WhereClause> parseWhereClause();
    };
}

#endif // QUERY_BASIC_PARSER_H