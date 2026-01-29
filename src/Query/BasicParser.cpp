#include "Query/BasicParser.h"

namespace Xale::Query
{
    /*
     * @brief Default constructor
     */
    BasicParser::BasicParser()
        : _tokenizer(nullptr)
    {}

    /*
     * @brief Constructor with tokenizer
     * @param tokenizer Pointer to a tokenizer instance
     */
    BasicParser::BasicParser(ITokenizer* tokenizer)
        : _tokenizer(tokenizer)
    {}

    /*
     * @brief Parse a SQL query string
     * @param query The SQL query to parse
     * @return Unique pointer to the parsed statement
     * @throws DbException if parsing fails
     */
    std::unique_ptr<Statement> BasicParser::parse(const std::string& query)
    {
        if (!_tokenizer)
            throwError("No tokenizer set");

        _tokenizer->setInput(query);
        _tokenizer->reset();
        advance();

        auto stmt = parseStatement();

        if (!match(TokenType::EndOfInput))
            throwError("Unexpected tokens after statement");

        return stmt;
    }

    /*
     * @brief Set the tokenizer to use
     * @param tokenizer Pointer to a tokenizer instance
     */
    void BasicParser::setTokenizer(ITokenizer* tokenizer)
    {
        _tokenizer = tokenizer;
    }

    /*
     * @brief Advance to next token
     */
    void BasicParser::advance()
    {
        if (_tokenizer)
            _currentToken = _tokenizer->nextToken();
    }

    /*
     * @brief Check if current token matches type
     * @param type Token type to match
     * @return True if matches, false otherwise
     */
    bool BasicParser::match(TokenType type)
    {
        return _currentToken.type == type;
    }

    /*
     * @brief Check if current token is a specific keyword
     * @param keyword Keyword to match (case-insensitive)
     * @return True if matches, false otherwise
     */
    bool BasicParser::matchKeyword(const std::string& keyword)
    {
        if (_currentToken.type != TokenType::ManipulationKeyword &&
            _currentToken.type != TokenType::QueryKeyword &&
            _currentToken.type != TokenType::DefinitionKeyword &&
            _currentToken.type != TokenType::LogicalKeyword)
            return false;

        std::string upper = _currentToken.lexeme;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        
        return upper == keyword;
    }

    /*
     * @brief Check if current token is a specific identifier
     * @param identifier Identifier to match (case-insensitive)
     * @return True if matches, false otherwise
     */
    bool BasicParser::matchIdentifier(const std::string& identifier)
    {
        if (_currentToken.type != TokenType::Identifier)
            return false;

        std::string upper = _currentToken.lexeme;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        
        return upper == identifier;
    }

    /*
     * @brief Expect a specific token type or throw exception
     * @param type Expected token type
     * @param errorMsg Error message if expectation fails
     * @throws DbException if token doesn't match
     */
    void BasicParser::expect(TokenType type, const std::string& errorMsg)
    {
        if (!match(type))
            throwError(errorMsg);
    }

    /*
     * @brief Expect a specific keyword or throw exception
     * @param keyword Expected keyword
     * @param errorMsg Error message if expectation fails
     * @throws DbException if keyword doesn't match
     */
    void BasicParser::expectKeyword(const std::string& keyword, const std::string& errorMsg)
    {
        if (!matchKeyword(keyword))
            throwError(errorMsg);
    }

    /*
     * @brief Throw a parse error exception
     * @param message Error message
     * @throws DbException with parse error code
     */
    void BasicParser::throwError(const std::string& message)
    {
        std::string fullMessage = message + " at position " + std::to_string(_currentToken.position);
        THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ParseError, fullMessage);
    }

    /*
     * @brief Parse a SQL statement
     * @return Unique pointer to parsed statement
     * @throws DbException if statement is invalid
     */
    std::unique_ptr<Statement> BasicParser::parseStatement()
    {
        if (matchKeyword("SELECT"))
            return parseSelect();
        else if (matchKeyword("INSERT"))
            return parseInsert();
        else if (matchKeyword("UPDATE"))
            return parseUpdate();
        else if (matchKeyword("DELETE"))
            return parseDelete();
        else if (matchKeyword("CREATE"))
            return parseCreate();
        else if (matchKeyword("DROP"))
            return parseDrop();
        else
        {
            throwError("Expected SQL statement (SELECT, INSERT, UPDATE, DELETE, CREATE, DROP)");
            return nullptr;
        }
    }

    /*
     * @brief Parse SELECT statement
     * @return Unique pointer to SelectStatement
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<SelectStatement> BasicParser::parseSelect()
    {
        auto stmt = std::make_unique<SelectStatement>();

        expectKeyword("SELECT", "Expected SELECT keyword");
        advance();

        if (match(TokenType::Operator) && _currentToken.lexeme == "*")
        {
            stmt->columns.push_back(Expression(ExpressionType::Wildcard, "*"));
            advance();
        }
        else
        {
            do
            {
                if (match(TokenType::Identifier))
                {
                    stmt->columns.push_back(Expression(ExpressionType::Identifier, _currentToken.lexeme));
                    advance();

                    if (match(TokenType::Operator) && _currentToken.lexeme == ",")
                        advance();
                    else
                        break;
                }
                else
                    throwError("Expected column name");
            } while (true);
        }

        expectKeyword("FROM", "Expected FROM keyword");
        advance();

        expect(TokenType::Identifier, "Expected table name");
        stmt->tableName = _currentToken.lexeme;
        advance();

        if (matchKeyword("WHERE"))
            stmt->where = parseWhereClause();

        return stmt;
    }

    /*
     * @brief Parse INSERT statement
     * @return Unique pointer to InsertStatement
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<InsertStatement> BasicParser::parseInsert()
    {
        auto stmt = std::make_unique<InsertStatement>();

        expectKeyword("INSERT", "Expected INSERT keyword");
        advance();

        if (matchIdentifier("INTO"))
            advance();

        expect(TokenType::Identifier, "Expected table name");
        stmt->tableName = _currentToken.lexeme;
        advance();

        if (!matchIdentifier("VALUES"))
            throwError("Expected VALUES keyword");
        
        advance();

        do
        {
            auto expr = parsePrimary();
            if (expr)
                stmt->values.push_back(std::move(*expr));
            else
                throwError("Expected value");

            if (match(TokenType::Operator) && _currentToken.lexeme == ",")
                advance();
            else
                break;
        } while (true);

        return stmt;
    }

    /*
     * @brief Parse UPDATE statement
     * @return Unique pointer to UpdateStatement
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<UpdateStatement> BasicParser::parseUpdate()
    {
        auto stmt = std::make_unique<UpdateStatement>();

        expectKeyword("UPDATE", "Expected UPDATE keyword");
        advance();

        expect(TokenType::Identifier, "Expected table name");
        stmt->tableName = _currentToken.lexeme;
        advance();

        if (!matchIdentifier("SET"))
            throwError("Expected SET keyword");
        advance();

        do
        {
            expect(TokenType::Identifier, "Expected column name");
            std::string columnName = _currentToken.lexeme;
            advance();

            if (!match(TokenType::Operator) || _currentToken.lexeme != "=")
                throwError("Expected '=' operator");
            advance();

            auto value = parsePrimary();
            if (!value)
                throwError("Expected value");

            stmt->assignments.push_back({columnName, std::move(*value)});

            if (match(TokenType::Operator) && _currentToken.lexeme == ",")
                advance();
            else
                break;
        } while (true);

        if (matchKeyword("WHERE"))
            stmt->where = parseWhereClause();

        return stmt;
    }

    /*
     * @brief Parse DELETE statement
     * @return Unique pointer to DeleteStatement
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<DeleteStatement> BasicParser::parseDelete()
    {
        auto stmt = std::make_unique<DeleteStatement>();

        expectKeyword("DELETE", "Expected DELETE keyword");
        advance();

        expectKeyword("FROM", "Expected FROM keyword");
        advance();

        expect(TokenType::Identifier, "Expected table name");
        stmt->tableName = _currentToken.lexeme;
        advance();

        if (matchKeyword("WHERE"))
            stmt->where = parseWhereClause();

        return stmt;
    }

    /*
     * @brief Parse CREATE statement
     * @return Unique pointer to CreateStatement
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<CreateStatement> BasicParser::parseCreate()
    {
        auto stmt = std::make_unique<CreateStatement>();

        expectKeyword("CREATE", "Expected CREATE keyword");
        advance();

        if (!match(TokenType::Identifier))
            throwError("Expected TABLE keyword");
        
        std::string upper = _currentToken.lexeme;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        if (upper != "TABLE")
            throwError("Expected TABLE keyword");
        advance();

        expect(TokenType::Identifier, "Expected table name");
        stmt->tableName = _currentToken.lexeme;
        advance();

        return stmt;
    }

    /*
     * @brief Parse DROP statement
     * @return Unique pointer to DropStatement
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<DropStatement> BasicParser::parseDrop()
    {
        auto stmt = std::make_unique<DropStatement>();

        expectKeyword("DROP", "Expected DROP keyword");
        advance();

        if (!match(TokenType::Identifier))
            throwError("Expected TABLE keyword");
        
        std::string upper = _currentToken.lexeme;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        if (upper != "TABLE")
            throwError("Expected TABLE keyword");
        advance();

        expect(TokenType::Identifier, "Expected table name");
        stmt->tableName = _currentToken.lexeme;
        advance();

        return stmt;
    }

    /*
     * @brief Parse an expression
     * @return Unique pointer to Expression
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<Expression> BasicParser::parseExpression()
    {
        return parseComparison();
    }

    /*
     * @brief Parse a comparison expression
     * @return Unique pointer to Expression
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<Expression> BasicParser::parseComparison()
    {
        auto left = parsePrimary();
        if (!left)
            return nullptr;

        if (match(TokenType::Operator))
        {
            std::string op = _currentToken.lexeme;
            if (op == "=" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=")
            {
                advance();

                auto right = parsePrimary();
                if (!right)
                    throwError("Expected right-hand expression");

                auto binaryExpr = std::make_unique<BinaryExpression>(
                    std::move(left),
                    op,
                    std::move(right)
                );

                auto expr = std::make_unique<Expression>(ExpressionType::BinaryOp);
                expr->binary = std::move(binaryExpr);
                return expr;
            }
        }

        return left;
    }

    /*
     * @brief Parse a primary expression (identifier or literal)
     * @return Unique pointer to Expression
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<Expression> BasicParser::parsePrimary()
    {
        if (match(TokenType::Identifier))
        {
            auto expr = std::make_unique<Expression>(ExpressionType::Identifier, _currentToken.lexeme);
            advance();
            return expr;
        }
        else if (match(TokenType::StringLiteral))
        {
            auto expr = std::make_unique<Expression>(ExpressionType::StringLiteral, _currentToken.lexeme);
            advance();
            return expr;
        }
        else if (match(TokenType::NumericLiteral))
        {
            auto expr = std::make_unique<Expression>(ExpressionType::NumericLiteral, _currentToken.lexeme);
            advance();
            return expr;
        }
        else
        {
            throwError("Expected identifier or literal");
            return nullptr;
        }
    }

    /*
     * @brief Parse WHERE clause
     * @return Unique pointer to WhereClause
     * @throws DbException if syntax is invalid
     */
    std::unique_ptr<WhereClause> BasicParser::parseWhereClause()
    {
        expectKeyword("WHERE", "Expected WHERE keyword");
        advance();

        auto condition = parseExpression();
        if (!condition)
            throwError("Expected condition expression");

        return std::make_unique<WhereClause>(std::move(condition));
    }
}
