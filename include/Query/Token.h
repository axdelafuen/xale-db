#ifndef QUERY_TOKEN_H
#define QUERY_TOKEN_H

#include <string>
#include <unordered_set>

namespace Xale::Query
{
    /*
     * @brief Types of possible tokens
     */
    enum class TokenType
    {
        DefinitionKeyword,
        ManipulationKeyword,
        QueryKeyword,
        JoinKeyword,
        LogicalKeyword,
        Operator,
        Identifier,
        StringLiteral,
        NumericLiteral,
        EndOfInput,
        Unknown
    };

    /*
     * @brief Token struct defined by type, value and pos
     */
    struct Token
    {
        TokenType type;
        std::string lexeme;
        size_t position;
    };

    /* --------------------------------------------------
     * SQL (Basic) Keywords declaration
     * --------------------------------------------------
     */

#define DECLARE_TOKENS(name, ...) \
    const std::unordered_set<std::string> name({__VA_ARGS__})

    // Data definition keywords
    DECLARE_TOKENS(sql_definition_kw,
        "CREATE",
        "ALTER",
        "DROP",
        "LIST"
    );

    // Data manipulation keywords
    DECLARE_TOKENS(sql_manipulation_kw,
        "SELECT", 
        "INSERT", 
        "UPDATE", 
        "DELETE"
    );

    // Query keywords
    DECLARE_TOKENS(sql_query_kw,
        "FROM",
        "WHERE"
    );

    // Join keywords
    DECLARE_TOKENS(sql_join_kw,
        "JOIN", 
        "LEFT", 
        "RIGHT"
    );

    // Logical keywords
    DECLARE_TOKENS(sql_logical_kw,
        "AND", 
        "OR", 
        "NOT"
    );

    // Operators
    DECLARE_TOKENS(sql_operators,
        "*",
        ",",
        "(",
        ")",
        "=", 
        "!=",
        "<",
        ">",
        "<=",
        ">="
    );

    // Special token
    DECLARE_TOKENS(sql_end_query,
        ";"
    );

    // For debug purpose
    /*
	 * @brief Convert TokenType to string
	 * @return string representation of TokenType
     */
    inline const std::string to_string(TokenType type)
    {
        switch (type)
        {
            case TokenType::DefinitionKeyword:      return "DefinitionKeyword";
            case TokenType::ManipulationKeyword:    return "ManipulationKeyword";
            case TokenType::QueryKeyword:           return "QueryKeyword";
            case TokenType::JoinKeyword:            return "JoinKeyword";
            case TokenType::LogicalKeyword:         return "LogicalKeyword";
            case TokenType::Operator:               return "Operator";
            case TokenType::Identifier:             return "Identifier";
            case TokenType::StringLiteral:          return "StringLiteral";
            case TokenType::NumericLiteral:         return "NumericLiteral";
            case TokenType::EndOfInput:             return "EndOfInput";
            case TokenType::Unknown:                return "Unknown";
        }
        return "Invalid";
    }
}

#endif // QUERY_TOKEN_H
