#ifndef BASIC_PARSER_TESTS_H
#define BASIC_PARSER_TESTS_H

#include "TestsHelper.h"
#include "Query/BasicParser.h"
#include "Query/BasicTokenizer.h"
#include "Core/ExceptionHandler.h"

#define DECLARE_PARSER_TEST(name) DECLARE_TEST(QUERY, basic_parser_##name)

namespace Xale::Tests
{
    DECLARE_PARSER_TEST(parse_select_all)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("SELECT * FROM users");
            
            if (!stmt || stmt->type != Xale::Query::StatementType::Select)
                return false;
            
            auto selectStmt = dynamic_cast<Xale::Query::SelectStatement*>(stmt.get());
            if (!selectStmt)
                return false;
            
            return selectStmt->tableName == "users" &&
                   selectStmt->columns.size() == 1 &&
                   selectStmt->columns[0].type == Xale::Query::ExpressionType::Wildcard;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_PARSER_TEST(parse_select_columns)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("SELECT id, name, age FROM users");
            
            if (!stmt || stmt->type != Xale::Query::StatementType::Select)
                return false;
            
            auto selectStmt = dynamic_cast<Xale::Query::SelectStatement*>(stmt.get());
            if (!selectStmt)
                return false;
            
            return selectStmt->tableName == "users" &&
                   selectStmt->columns.size() == 3 &&
                   selectStmt->columns[0].value == "id" &&
                   selectStmt->columns[1].value == "name" &&
                   selectStmt->columns[2].value == "age";
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_PARSER_TEST(parse_select_with_where)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("SELECT * FROM users WHERE age > 18");
            
            if (!stmt || stmt->type != Xale::Query::StatementType::Select)
                return false;
            
            auto selectStmt = dynamic_cast<Xale::Query::SelectStatement*>(stmt.get());
            if (!selectStmt)
                return false;
            
            return selectStmt->tableName == "users" &&
                   selectStmt->where != nullptr &&
                   selectStmt->where->condition != nullptr;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_PARSER_TEST(parse_insert)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("INSERT INTO users VALUES 1, 'John', 25");
            
            if (!stmt || stmt->type != Xale::Query::StatementType::Insert)
                return false;
            
            auto insertStmt = dynamic_cast<Xale::Query::InsertStatement*>(stmt.get());
            if (!insertStmt)
                return false;
            
            return insertStmt->tableName == "users" &&
                   insertStmt->values.size() == 3;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_PARSER_TEST(parse_update)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("UPDATE users SET name = 'Jane' WHERE id = 1");
            
            if (!stmt || stmt->type != Xale::Query::StatementType::Update)
                return false;
            
            auto updateStmt = dynamic_cast<Xale::Query::UpdateStatement*>(stmt.get());
            if (!updateStmt)
                return false;
            
            return updateStmt->tableName == "users" &&
                   updateStmt->assignments.size() == 1 &&
                   updateStmt->assignments[0].first == "name" &&
                   updateStmt->where != nullptr;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_PARSER_TEST(parse_update_multiple_columns)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("UPDATE users SET name = 'Jane', age = 30");
            
            if (!stmt || stmt->type != Xale::Query::StatementType::Update)
                return false;
            
            auto updateStmt = dynamic_cast<Xale::Query::UpdateStatement*>(stmt.get());
            if (!updateStmt)
                return false;
            
            return updateStmt->tableName == "users" &&
                   updateStmt->assignments.size() == 2 &&
                   updateStmt->assignments[0].first == "name" &&
                   updateStmt->assignments[1].first == "age";
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_PARSER_TEST(parse_delete)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("DELETE FROM users WHERE id = 1");
            
            if (!stmt || stmt->type != Xale::Query::StatementType::Delete)
                return false;
            
            auto deleteStmt = dynamic_cast<Xale::Query::DeleteStatement*>(stmt.get());
            if (!deleteStmt)
                return false;
            
            return deleteStmt->tableName == "users" &&
                   deleteStmt->where != nullptr;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_PARSER_TEST(parse_create_table)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("CREATE TABLE users");
            
            if (!stmt || stmt->type != Xale::Query::StatementType::Create)
                return false;
            
            auto createStmt = dynamic_cast<Xale::Query::CreateStatement*>(stmt.get());
            if (!createStmt)
                return false;
            
            return createStmt->tableName == "users";
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_PARSER_TEST(parse_drop_table)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("DROP TABLE users");
            
            if (!stmt || stmt->type != Xale::Query::StatementType::Drop)
                return false;
            
            auto dropStmt = dynamic_cast<Xale::Query::DropStatement*>(stmt.get());
            if (!dropStmt)
                return false;
            
            return dropStmt->tableName == "users";
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_PARSER_TEST(parse_error_invalid_statement)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("INVALID STATEMENT");
            
            return false;
        }
        catch (const Xale::Core::DbException&)
        {
            return true;
        }
    }

    DECLARE_PARSER_TEST(parse_error_missing_table_name)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("SELECT * FROM");
            
            return false;
        }
        catch (const Xale::Core::DbException&)
        {
            return true;
        }
    }

    DECLARE_PARSER_TEST(parse_error_missing_where_condition)
    {
        try
        {
            Xale::Query::BasicTokenizer tokenizer;
            Xale::Query::BasicParser parser(&tokenizer);
            
            auto stmt = parser.parse("SELECT * FROM users WHERE");
            
            return false;
        }
        catch (const Xale::Core::DbException&)
        {
            return true;
        }
    }
}

#endif // BASIC_PARSER_TESTS_H
