#ifndef BASIC_EXECUTOR_TESTS_H
#define BASIC_EXECUTOR_TESTS_H

#include "TestsHelper.h"
#include "Execution/BasicExecutor.h"
#include "Execution/TableManager.h"
#include "Query/Statement.h"
#include "Storage/BinaryFileManager.h"
#include "Storage/FileStorageEngine.h"
#include "DataStructure/DataTypes.h"
#include "Core/ExceptionHandler.h"

#define DECLARE_EXECUTOR_TEST(name) DECLARE_TEST(EXECUTION, basic_executor_##name)

namespace Xale::Tests
{
    DECLARE_EXECUTOR_TEST(create_table)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-executor-create_table.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage, fm);
            Xale::Execution::BasicExecutor executor(manager);
            
            auto createStmt = std::make_unique<Xale::Query::CreateStatement>();
            createStmt->tableName = "users";
            
            auto result = executor.execute(createStmt.get());
            
            bool success = manager.tableExists("users") &&
                          result != nullptr &&
                          result->isEmpty();
            
            storage.shutdown();
            return success;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_EXECUTOR_TEST(insert_row)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-executor-insert_row.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage, fm);
            Xale::Execution::BasicExecutor executor(manager);
            
            // Create table
            auto createStmt = std::make_unique<Xale::Query::CreateStatement>();
            createStmt->tableName = "users";
            executor.execute(createStmt.get());
            
            // Add columns
            auto* table = manager.getTable("users");
            table->addColumn(Xale::DataStructure::ColumnDefinition("id", Xale::DataStructure::FieldType::Integer, true));
            table->addColumn(Xale::DataStructure::ColumnDefinition("name", Xale::DataStructure::FieldType::String));
            table->addColumn(Xale::DataStructure::ColumnDefinition("age", Xale::DataStructure::FieldType::Integer));
            
            // Insert row
            auto insertStmt = std::make_unique<Xale::Query::InsertStatement>();
            insertStmt->tableName = "users";
            insertStmt->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "1"));
            insertStmt->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::StringLiteral, "'John'"));
            insertStmt->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "25"));
            
            executor.execute(insertStmt.get());
            
            bool success = table->getRowCount() == 1;
            
            storage.shutdown();
            return success;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_EXECUTOR_TEST(select_all)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-executor-select_all.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage, fm);
            Xale::Execution::BasicExecutor executor(manager);
            
            // Create and setup table
            auto createStmt = std::make_unique<Xale::Query::CreateStatement>();
            createStmt->tableName = "users";
            executor.execute(createStmt.get());
            
            auto* table = manager.getTable("users");
            table->addColumn(Xale::DataStructure::ColumnDefinition("id", Xale::DataStructure::FieldType::Integer));
            table->addColumn(Xale::DataStructure::ColumnDefinition("name", Xale::DataStructure::FieldType::String));
            
            // Insert rows
            auto insert1 = std::make_unique<Xale::Query::InsertStatement>();
            insert1->tableName = "users";
            insert1->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "1"));
            insert1->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::StringLiteral, "'John'"));
            executor.execute(insert1.get());
            
            auto insert2 = std::make_unique<Xale::Query::InsertStatement>();
            insert2->tableName = "users";
            insert2->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "2"));
            insert2->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::StringLiteral, "'Jane'"));
            executor.execute(insert2.get());
            
            // Select all
            auto selectStmt = std::make_unique<Xale::Query::SelectStatement>();
            selectStmt->tableName = "users";
            selectStmt->columns.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::Wildcard, "*"));
            
            auto result = executor.execute(selectStmt.get());
            
            bool success = result->getRowCount() == 2 &&
                          result->getColumnCount() == 2;
            
            storage.shutdown();
            return success;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_EXECUTOR_TEST(select_where)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-executor-select_where.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage, fm);
            Xale::Execution::BasicExecutor executor(manager);
            
            // Create and setup table
            auto createStmt = std::make_unique<Xale::Query::CreateStatement>();
            createStmt->tableName = "users";
            executor.execute(createStmt.get());
            
            auto* table = manager.getTable("users");
            table->addColumn(Xale::DataStructure::ColumnDefinition("id", Xale::DataStructure::FieldType::Integer));
            table->addColumn(Xale::DataStructure::ColumnDefinition("age", Xale::DataStructure::FieldType::Integer));
            
            // Insert rows
            for (int i = 1; i <= 5; ++i)
            {
                auto insert = std::make_unique<Xale::Query::InsertStatement>();
                insert->tableName = "users";
                insert->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, std::to_string(i)));
                insert->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, std::to_string(i * 10)));
                executor.execute(insert.get());
            }
            
            // Select with WHERE id = 3
            auto selectStmt = std::make_unique<Xale::Query::SelectStatement>();
            selectStmt->tableName = "users";
            selectStmt->where = std::make_unique<Xale::Query::WhereClause>();
            selectStmt->where->condition = std::make_unique<Xale::Query::Expression>(Xale::Query::ExpressionType::BinaryOp);
            selectStmt->where->condition->binary = std::make_unique<Xale::Query::BinaryExpression>();
            selectStmt->where->condition->binary->left = std::make_unique<Xale::Query::Expression>(Xale::Query::ExpressionType::Identifier, "id");
            selectStmt->where->condition->binary->op = "=";
            selectStmt->where->condition->binary->right = std::make_unique<Xale::Query::Expression>(Xale::Query::ExpressionType::NumericLiteral, "3");
            
            auto result = executor.execute(selectStmt.get());
            
            bool success = result->getRowCount() == 1;
            
            storage.shutdown();
            return success;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_EXECUTOR_TEST(select_where_greater_than)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-executor-select_where_greater_than.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage, fm);
            Xale::Execution::BasicExecutor executor(manager);
            
            // Create and setup table
            auto createStmt = std::make_unique<Xale::Query::CreateStatement>();
            createStmt->tableName = "users";
            executor.execute(createStmt.get());
            
            auto* table = manager.getTable("users");
            table->addColumn(Xale::DataStructure::ColumnDefinition("id", Xale::DataStructure::FieldType::Integer));
            table->addColumn(Xale::DataStructure::ColumnDefinition("age", Xale::DataStructure::FieldType::Integer));
            
            // Insert rows (ages: 10, 20, 30, 40, 50)
            for (int i = 1; i <= 5; ++i)
            {
                auto insert = std::make_unique<Xale::Query::InsertStatement>();
                insert->tableName = "users";
                insert->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, std::to_string(i)));
                insert->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, std::to_string(i * 10)));
                executor.execute(insert.get());
            }
            
            // Select WHERE age > 25
            auto selectStmt = std::make_unique<Xale::Query::SelectStatement>();
            selectStmt->tableName = "users";
            selectStmt->columns.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::Wildcard, "*"));
            selectStmt->where = std::make_unique<Xale::Query::WhereClause>();
            selectStmt->where->condition = std::make_unique<Xale::Query::Expression>(Xale::Query::ExpressionType::BinaryOp);
            selectStmt->where->condition->binary = std::make_unique<Xale::Query::BinaryExpression>();
            selectStmt->where->condition->binary->left = std::make_unique<Xale::Query::Expression>(Xale::Query::ExpressionType::Identifier, "age");
            selectStmt->where->condition->binary->op = ">";
            selectStmt->where->condition->binary->right = std::make_unique<Xale::Query::Expression>(Xale::Query::ExpressionType::NumericLiteral, "25");
            
            auto result = executor.execute(selectStmt.get());
            
            // Should return 3 rows (age 30, 40, 50)
            bool success = result->getRowCount() == 3;
            
            storage.shutdown();
            return success;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_EXECUTOR_TEST(drop_table)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-executor-drop_table.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage, fm);
            Xale::Execution::BasicExecutor executor(manager);
            
            // Create table
            auto createStmt = std::make_unique<Xale::Query::CreateStatement>();
            createStmt->tableName = "users";
            executor.execute(createStmt.get());
            
            bool existsBeforeDrop = manager.tableExists("users");
            
            // Drop table
            auto dropStmt = std::make_unique<Xale::Query::DropStatement>();
            dropStmt->tableName = "users";
            executor.execute(dropStmt.get());
            
            bool existsAfterDrop = manager.tableExists("users");
            
            bool success = existsBeforeDrop && !existsAfterDrop;
            
            storage.shutdown();
            return success;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_EXECUTOR_TEST(select_nonexistent_table)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-executor-select_nonexistent_table.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage, fm);
            Xale::Execution::BasicExecutor executor(manager);
            
            auto selectStmt = std::make_unique<Xale::Query::SelectStatement>();
            selectStmt->tableName = "nonexistent";
            selectStmt->columns.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::Wildcard, "*"));
            
            executor.execute(selectStmt.get());
            
            storage.shutdown();
            return false; // Should have thrown
        }
        catch (const Xale::Core::DbException&)
        {
            return true; // Expected exception
        }
    }

    DECLARE_EXECUTOR_TEST(insert_wrong_value_count)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-executor-insert_wrong_value_count.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage, fm);
            Xale::Execution::BasicExecutor executor(manager);
            
            // Create table with 2 columns
            auto createStmt = std::make_unique<Xale::Query::CreateStatement>();
            createStmt->tableName = "users";
            executor.execute(createStmt.get());
            
            auto* table = manager.getTable("users");
            table->addColumn(Xale::DataStructure::ColumnDefinition("id", Xale::DataStructure::FieldType::Integer));
            table->addColumn(Xale::DataStructure::ColumnDefinition("name", Xale::DataStructure::FieldType::String));
            
            // Try to insert 3 values
            auto insertStmt = std::make_unique<Xale::Query::InsertStatement>();
            insertStmt->tableName = "users";
            insertStmt->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "1"));
            insertStmt->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::StringLiteral, "'John'"));
            insertStmt->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "25"));
            
            executor.execute(insertStmt.get());
            
            storage.shutdown();
            return false; // Should have thrown
        }
        catch (const Xale::Core::DbException&)
        {
            return true; // Expected exception
        }
    }
}

#endif // BASIC_EXECUTOR_TESTS_H
