#ifndef QUERY_STATEMENT_H
#define QUERY_STATEMENT_H

#include <string>
#include <vector>
#include <memory>
#include <variant>

namespace Xale::Query
{
    /*
     * @brief Types of SQL statements
     */
    enum class StatementType
    {
        Select,
        Insert,
        Update,
        Delete,
        Create,
        Drop,
        Unknown
    };

    /*
     * @brief Types of expression nodes
     */
    enum class ExpressionType
    {
        Identifier,
        StringLiteral,
        NumericLiteral,
        BinaryOp,
        Wildcard
    };

    /*
     * @brief Forward declarations
     */
    struct Expression;
    struct BinaryExpression;

    /*
     * @brief Base expression structure
     */
    struct Expression
    {
        ExpressionType type;
        std::string value;
        std::unique_ptr<BinaryExpression> binary;

        Expression() : type(ExpressionType::Identifier) {}
        explicit Expression(ExpressionType t, std::string val = "")
            : type(t), value(std::move(val)) {}
    };

    /*
     * @brief Binary expression
     */
    struct BinaryExpression
    {
        std::unique_ptr<Expression> left;
        std::string op;
        std::unique_ptr<Expression> right;

        BinaryExpression() = default;
        BinaryExpression(std::unique_ptr<Expression> l, std::string operation, std::unique_ptr<Expression> r)
            : left(std::move(l)), op(std::move(operation)), right(std::move(r)) {}
    };

    /*
     * @brief WHERE clause representation
     */
    struct WhereClause
    {
        std::unique_ptr<Expression> condition;
        
        WhereClause() = default;
        explicit WhereClause(std::unique_ptr<Expression> cond)
            : condition(std::move(cond)) {}
    };

    /*
     * @brief Base statement structure
     */
    struct Statement
    {
        StatementType type;

        explicit Statement(StatementType t) : type(t) {}
        virtual ~Statement() = default;
    };

    /*
     * @brief SELECT statement structure
     */
    struct SelectStatement : public Statement
    {
        std::vector<Expression> columns;
        std::string tableName;
        std::unique_ptr<WhereClause> where;

        SelectStatement() : Statement(StatementType::Select) {}
    };

    /*
     * @brief INSERT statement structure
     */
    struct InsertStatement : public Statement
    {
        std::string tableName;
        std::vector<std::string> columns;
        std::vector<Expression> values;

        InsertStatement() : Statement(StatementType::Insert) {}
    };

    /*
     * @brief UPDATE statement structure
     */
    struct UpdateStatement : public Statement
    {
        std::string tableName;
        std::vector<std::pair<std::string, Expression>> assignments;
        std::unique_ptr<WhereClause> where;

        UpdateStatement() : Statement(StatementType::Update) {}
    };

    /*
     * @brief DELETE statement structure
     */
    struct DeleteStatement : public Statement
    {
        std::string tableName;
        std::unique_ptr<WhereClause> where;

        DeleteStatement() : Statement(StatementType::Delete) {}
    };

    /*
     * @brief CREATE TABLE statement structure
     */
    struct CreateStatement : public Statement
    {
        std::string tableName;
        
        CreateStatement() : Statement(StatementType::Create) {}
    };

    /*
     * @brief DROP TABLE statement structure
     */
    struct DropStatement : public Statement
    {
        std::string tableName;

        DropStatement() : Statement(StatementType::Drop) {}
    };
}

#endif // QUERY_STATEMENT_H
