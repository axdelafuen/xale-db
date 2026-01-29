#ifndef BASIC_TOKENIZER_TESTS_H
#define BASIC_TOKENIZER_TESTS_H

#include "TestsHelper.h"
#include "Query/BasicTokenizer.h"

#include <string>
#include <vector>

#define DECLARE_TOKENIZER_TEST(name) DECLARE_TEST(QUERY, basic_tokenizer_##name)

namespace Xale::Tests
{
    DECLARE_TOKENIZER_TEST(tokenize_select_all)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("SELECT * FROM users");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 5 && // SELECT, *, FROM, users, EndOfInput
               tokens[0].type == Xale::Query::TokenType::ManipulationKeyword &&
               tokens[0].lexeme == "SELECT" &&
               tokens[1].type == Xale::Query::TokenType::Operator &&
               tokens[1].lexeme == "*" &&
               tokens[2].type == Xale::Query::TokenType::QueryKeyword &&
               tokens[2].lexeme == "FROM" &&
               tokens[3].type == Xale::Query::TokenType::Identifier &&
               tokens[3].lexeme == "users" &&
               tokens[4].type == Xale::Query::TokenType::EndOfInput;
    }

    DECLARE_TOKENIZER_TEST(tokenize_operators_equal)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("id = 42");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 4 && // id, =, 42, EndOfInput
               tokens[0].type == Xale::Query::TokenType::Identifier &&
               tokens[0].lexeme == "id" &&
               tokens[1].type == Xale::Query::TokenType::Operator &&
               tokens[1].lexeme == "=" &&
               tokens[2].type == Xale::Query::TokenType::NumericLiteral &&
               tokens[2].lexeme == "42" &&
               tokens[3].type == Xale::Query::TokenType::EndOfInput;
    }

    DECLARE_TOKENIZER_TEST(tokenize_operators_not_equal)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("status != 'active'");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 4 && // status, !=, 'active', EndOfInput
               tokens[0].type == Xale::Query::TokenType::Identifier &&
               tokens[0].lexeme == "status" &&
               tokens[1].type == Xale::Query::TokenType::Operator &&
               tokens[1].lexeme == "!=" &&
               tokens[2].type == Xale::Query::TokenType::StringLiteral &&
               tokens[2].lexeme == "'active'" &&
               tokens[3].type == Xale::Query::TokenType::EndOfInput;
    }

    DECLARE_TOKENIZER_TEST(tokenize_comparison_operators)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("age >= 18");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 4 && // age, >=, 18, EndOfInput
               tokens[0].type == Xale::Query::TokenType::Identifier &&
               tokens[1].type == Xale::Query::TokenType::Operator &&
               tokens[1].lexeme == ">=" &&
               tokens[2].type == Xale::Query::TokenType::NumericLiteral &&
               tokens[2].lexeme == "18";
    }

    DECLARE_TOKENIZER_TEST(tokenize_string_literal)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("'hello world'");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 2 && // 'hello world', EndOfInput
               tokens[0].type == Xale::Query::TokenType::StringLiteral &&
               tokens[0].lexeme == "'hello world'" &&
               tokens[1].type == Xale::Query::TokenType::EndOfInput;
    }

    DECLARE_TOKENIZER_TEST(tokenize_numeric_literal_integer)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("12345");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 2 && // 12345, EndOfInput
               tokens[0].type == Xale::Query::TokenType::NumericLiteral &&
               tokens[0].lexeme == "12345" &&
               tokens[1].type == Xale::Query::TokenType::EndOfInput;
    }

    DECLARE_TOKENIZER_TEST(tokenize_numeric_literal_decimal)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("123.45");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 2 && // 123.45, EndOfInput
               tokens[0].type == Xale::Query::TokenType::NumericLiteral &&
               tokens[0].lexeme == "123.45" &&
               tokens[1].type == Xale::Query::TokenType::EndOfInput;
    }

    DECLARE_TOKENIZER_TEST(tokenize_keywords_definition)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("CREATE TABLE users");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 4 && // CREATE, TABLE, users, EndOfInput
               tokens[0].type == Xale::Query::TokenType::DefinitionKeyword &&
               tokens[0].lexeme == "CREATE" &&
               tokens[1].type == Xale::Query::TokenType::Identifier &&
               tokens[1].lexeme == "TABLE" &&
               tokens[2].type == Xale::Query::TokenType::Identifier &&
               tokens[2].lexeme == "users";
    }

    DECLARE_TOKENIZER_TEST(tokenize_keywords_logical)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("age > 18 AND status = 'active'");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 8 && // age, >, 18, AND, status, =, 'active', EndOfInput
               tokens[0].lexeme == "age" &&
               tokens[1].lexeme == ">" &&
               tokens[2].lexeme == "18" &&
               tokens[3].type == Xale::Query::TokenType::LogicalKeyword &&
               tokens[3].lexeme == "AND" &&
               tokens[4].lexeme == "status" &&
               tokens[5].lexeme == "=" &&
               tokens[6].lexeme == "'active'";
    }

    DECLARE_TOKENIZER_TEST(tokenize_complex_query)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("SELECT * FROM users WHERE id != 5");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 9 && // SELECT, *, FROM, users, WHERE, id, !=, 5, EndOfInput
               tokens[0].type == Xale::Query::TokenType::ManipulationKeyword &&
               tokens[0].lexeme == "SELECT" &&
               tokens[1].type == Xale::Query::TokenType::Operator &&
               tokens[1].lexeme == "*" &&
               tokens[2].type == Xale::Query::TokenType::QueryKeyword &&
               tokens[2].lexeme == "FROM" &&
               tokens[3].type == Xale::Query::TokenType::Identifier &&
               tokens[3].lexeme == "users" &&
               tokens[4].type == Xale::Query::TokenType::QueryKeyword &&
               tokens[4].lexeme == "WHERE" &&
               tokens[5].type == Xale::Query::TokenType::Identifier &&
               tokens[5].lexeme == "id" &&
               tokens[6].type == Xale::Query::TokenType::Operator &&
               tokens[6].lexeme == "!=" &&
               tokens[7].type == Xale::Query::TokenType::NumericLiteral &&
               tokens[7].lexeme == "5";
    }

    DECLARE_TOKENIZER_TEST(peek_token)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("SELECT FROM");
        
        auto peek1 = tokenizer.peekToken();
        auto peek2 = tokenizer.peekToken();
        auto next = tokenizer.nextToken();
        
        return peek1.lexeme == "SELECT" &&
               peek2.lexeme == "SELECT" &&
               next.lexeme == "SELECT" &&
               peek1.type == Xale::Query::TokenType::ManipulationKeyword;
    }

    DECLARE_TOKENIZER_TEST(reset_tokenizer)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("SELECT *");
        
        auto token1 = tokenizer.nextToken();
        tokenizer.reset();
        auto token2 = tokenizer.nextToken();
        
        return token1.lexeme == "SELECT" &&
               token2.lexeme == "SELECT" &&
               token1.position == token2.position;
    }

    DECLARE_TOKENIZER_TEST(tokenize_empty_input)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 1 &&
               tokens[0].type == Xale::Query::TokenType::EndOfInput;
    }

    DECLARE_TOKENIZER_TEST(tokenize_whitespace_handling)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("   SELECT   *   FROM   users   ");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 5 && // SELECT, *, FROM, users, EndOfInput
               tokens[0].lexeme == "SELECT" &&
               tokens[1].lexeme == "*" &&
               tokens[2].lexeme == "FROM" &&
               tokens[3].lexeme == "users";
    }

    DECLARE_TOKENIZER_TEST(tokenize_identifier_with_underscore)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("_user_name_123");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 2 && // _user_name_123, EndOfInput
               tokens[0].type == Xale::Query::TokenType::Identifier &&
               tokens[0].lexeme == "_user_name_123";
    }

    DECLARE_TOKENIZER_TEST(tokenize_case_insensitive_keywords)
    {
        Xale::Query::BasicTokenizer tokenizer;
        tokenizer.setInput("select FROM where");
        
        auto tokens = tokenizer.tokenize();
        
        return tokens.size() == 4 && // select, FROM, where, EndOfInput
               tokens[0].type == Xale::Query::TokenType::ManipulationKeyword &&
               tokens[0].lexeme == "select" &&
               tokens[1].type == Xale::Query::TokenType::QueryKeyword &&
               tokens[1].lexeme == "FROM" &&
               tokens[2].type == Xale::Query::TokenType::QueryKeyword &&
               tokens[2].lexeme == "where";
    }
}

#endif // BASIC_TOKENIZER_TESTS_H
