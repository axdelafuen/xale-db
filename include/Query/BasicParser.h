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
    /**
     * @brief Basic implementation of SQL parser
     */
    /**
     * @brief Basic implementation of SQL parser
     */
    class BasicParser : public IParser
    {
        public:
            /**
             * @brief Default constructor
             */
            BasicParser();

            /**
             * @brief Constructor with tokenizer
             * @param tokenizer Pointer to a tokenizer instance
             */
            explicit BasicParser(ITokenizer* tokenizer);

            /**
             * @brief Parse a SQL query string
             * @param query The SQL query to parse
             * @return Unique pointer to the parsed statement
             * @throws DbException if parsing fails
             */
            std::unique_ptr<Statement> parse(const std::string& query) override;

            /**
             * @brief Set the tokenizer to use
             * @param tokenizer Pointer to a tokenizer instance
             */
            void setTokenizer(ITokenizer* tokenizer) override;

        private:
            ITokenizer* _tokenizer;
            Token _currentToken;

            /**
             * @brief Advance to next token
             */
            void advance();

            /**
             * @brief Check if current token matches type
             * @param type Token type to match
             * @return True if matches, false otherwise
             */
            bool match(TokenType type);

            /**
             * @brief Check if current token is a specific keyword
             * @param keyword Keyword to match (case-insensitive)
             * @return True if matches, false otherwise
             */
            bool matchKeyword(const std::string& keyword);

            /**
             * @brief Check if current token is a specific identifier
             * @param identifier Identifier to match (case-insensitive)
             * @return True if matches, false otherwise
             */
            bool matchIdentifier(const std::string& identifier);

            /**
             * @brief Expect a specific token type or throw exception
             * @param type Expected token type
             * @param errorMsg Error message if expectation fails
             * @throws DbException if token doesn't match
             */
            void expect(TokenType type, const std::string& errorMsg);

            /**
             * @brief Expect a specific keyword or throw exception
             * @param keyword Expected keyword
             * @param errorMsg Error message if expectation fails
             * @throws DbException if keyword doesn't match
             */
            void expectKeyword(const std::string& keyword, const std::string& errorMsg);

            /**
             * @brief Throw a parse error exception
             * @param message Error message
             * @throws DbException with parse error code
             */
            void throwError(const std::string& message);

            /**
             * @brief Parse a SQL statement
             * @return Unique pointer to parsed statement
             * @throws DbException if statement is invalid
             */
            std::unique_ptr<Statement> parseStatement();

            /**
             * @brief Parse SELECT statement
             * @return Unique pointer to SelectStatement
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<SelectStatement> parseSelect();

            /**
             * @brief Parse INSERT statement
             * @return Unique pointer to InsertStatement
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<InsertStatement> parseInsert();

            /**
             * @brief Parse UPDATE statement
             * @return Unique pointer to UpdateStatement
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<UpdateStatement> parseUpdate();

            /**
             * @brief Parse DELETE statement
             * @return Unique pointer to DeleteStatement
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<DeleteStatement> parseDelete();

            /**
             * @brief Parse CREATE statement
             * @return Unique pointer to CreateStatement
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<CreateStatement> parseCreate();

            /**
             * @brief Parse DROP statement
             * @return Unique pointer to DropStatement
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<DropStatement> parseDrop();

            /**
             * @brief Parse LIST statement
             */
            std::unique_ptr<ListStatement> parseList();

            /**
             * @brief Parse an expression
             * @return Unique pointer to Expression
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<Expression> parseExpression();

            /**
             * @brief Parse a comparison expression
             * @return Unique pointer to Expression
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<Expression> parseComparison();

            /**
             * @brief Parse a primary expression (identifier or literal)
             * @return Unique pointer to Expression
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<Expression> parsePrimary();

            /**
             * @brief Parse WHERE clause
             * @return Unique pointer to WhereClause
             * @throws DbException if syntax is invalid
             */
            std::unique_ptr<WhereClause> parseWhereClause();
    };
}

#endif // QUERY_BASIC_PARSER_H
