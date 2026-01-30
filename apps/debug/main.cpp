#include "Logger.h"

#include "Core/ExceptionHandler.h"
#include "Core/AssertException.h"
#include "Core/Setup.h"
#include "Storage/BinaryFileManager.h"
#include "Storage/FileStorageEngine.h"
#include "DataStructure/BPlusTree.h"
#include "Query/BasicTokenizer.h"
#include "Query/BasicParser.h"

#include "DataStructure/ResultSet.h"
#include "DataStructure/Table.h"

#include "Execution/TableManager.h"
#include "Execution/BasicExecutor.h"

#include "Engine/QueryEngine.h"

#include <vector>
#include <string>
#include <memory>

/**
 * @brief Debug entrypoint
 */
int main()
{
	// Setup logger
    auto& logger = Xale::Logger::Logger<void>::getInstance();

	// Setup core systems
    std::string error;
    if (!Xale::Core::Init::setup(error))
    {
        logger.error("Setup failed: " + error);
        return -1;
    }

    // Test exception handling
    try
    {
        int x = 5;
        DE_ASSERT(x == 5, "Math is broken?");
        DE_ASSERT(x != 5, "Math is still broken?");

        DE_THROW(Xale::Core::ExceptionCode::Unknown, "This is a test exception");
    } catch (const Xale::Core::DbException& e)
    {
        logger.error(std::string(e.what()));
    }

	// Test Storage Engine
    logger.info("");
    logger.info("");
    logger.info("Test StorageEngine:");

    //auto& fm = Xale::Storage::BinaryFileManager();
    Xale::Storage::BinaryFileManager fm;
    Xale::Storage::FileStorageEngine engine(fm, "debug-storage.bin");
    if (!engine.startup())
    {
        logger.error("StorageEngine startup failed");
        return -1;
    }

    const std::string payload = "XALE_BINARY_TEST";
    const std::size_t written = fm.writeAt(0, payload.data(), payload.size());

    if (written == 0)
        logger.error("Storage write failed!");

    fm.sync();

    std::vector<char> readbuf(payload.size());
    const std::size_t read = fm.readAt(0, readbuf.data(), readbuf.size());

    if (read != payload.size() || std::string(readbuf.begin(), readbuf.end()) != payload)
        logger.error("Storage read/write verification failed");
    else
        logger.debug("Storage read/write verification succeeded");

    engine.shutdown();
    

    // Test DataStructure
    logger.info("");
    logger.info("");
    logger.info("Test DataStructure::BPlusTree:");
   
    // Test Tokenizer
    logger.info("");
    logger.info("");
    logger.info("Test Query::Tokenizer:");

    std::unique_ptr<Xale::Query::ITokenizer> tokenizer = std::make_unique<Xale::Query::BasicTokenizer>();
    tokenizer->setInput("SELECT * FROM users WHERE id != 5");

    for (const auto& token : tokenizer->tokenize())
    {
        logger.debug(
                "[" + to_string(token.type) + "] " +
                "'" + token.lexeme + "'");
    }

    /*
    // Test Parser
    logger.info("");
    logger.info("");
    logger.info("Test Query::Parser:");

    Xale::Query::BasicTokenizer parserTokenizer;
    Xale::Query::BasicParser parser(&parserTokenizer);

    try
    {
        logger.info("Parsing: SELECT * FROM users WHERE age > 18");
        auto selectStmt = parser.parse("SELECT * FROM users WHERE age > 18");
        auto select = dynamic_cast<Xale::Query::SelectStatement*>(selectStmt.get());
        if (select)
        {
            logger.debug("  Table: " + select->tableName);
            logger.debug("  Columns: " + std::to_string(select->columns.size()));
            logger.debug("  Has WHERE: " + std::string(select->where ? "yes" : "no"));
        }
    }
    catch (const Xale::Core::DbException& e)
    {
        logger.error("Parse error: " + std::string(e.what()));
    }

    try
    {
        logger.info("Parsing: INSERT INTO users VALUES 1, 'John', 25");
        auto insertStmt = parser.parse("INSERT INTO users VALUES 1, 'John', 25");
        auto insert = dynamic_cast<Xale::Query::InsertStatement*>(insertStmt.get());
        if (insert)
        {
            logger.debug("  Table: " + insert->tableName);
            logger.debug("  Values: " + std::to_string(insert->values.size()));
        }
    }
    catch (const Xale::Core::DbException& e)
    {
        logger.error("Parse error: " + std::string(e.what()));
    }

    try
    {
        logger.info("Parsing: UPDATE users SET name = 'Jane' WHERE id = 1");
        auto updateStmt = parser.parse("UPDATE users SET name = 'Jane' WHERE id = 1");
        auto update = dynamic_cast<Xale::Query::UpdateStatement*>(updateStmt.get());
        if (update)
        {
            logger.debug("  Table: " + update->tableName);
            logger.debug("  Assignments: " + std::to_string(update->assignments.size()));
            logger.debug("  Has WHERE: " + std::string(update->where ? "yes" : "no"));
        }
    }
    catch (const Xale::Core::DbException& e)
    {
        logger.error("Parse error: " + std::string(e.what()));
    }

    try
    {
        logger.info("Parsing: DELETE FROM users WHERE id = 1");
        auto deleteStmt = parser.parse("DELETE FROM users WHERE id = 1");
        auto del = dynamic_cast<Xale::Query::DeleteStatement*>(deleteStmt.get());
        if (del)
        {
            logger.debug("  Table: " + del->tableName);
            logger.debug("  Has WHERE: " + std::string(del->where ? "yes" : "no"));
        }
    }
    catch (const Xale::Core::DbException& e)
    {
        logger.error("Parse error: " + std::string(e.what()));
    }

    try
    {
        logger.info("Parsing: CREATE TABLE users");
        auto createStmt = parser.parse("CREATE TABLE users");
        auto create = dynamic_cast<Xale::Query::CreateStatement*>(createStmt.get());
        if (create)
        {
            logger.debug("  Table: " + create->tableName);
        }
    }
    catch (const Xale::Core::DbException& e)
    {
        logger.error("Parse error: " + std::string(e.what()));
    }

    try
    {
        logger.info("Parsing invalid query: INVALID STATEMENT");
        auto invalidStmt = parser.parse("INVALID STATEMENT");
    }
    catch (const Xale::Core::DbException& e)
    {
        logger.warning("Parse error (expected): " + std::string(e.what()));
    }
    
    logger.info("");
    logger.info("");
    logger.info("Test Execution::BasicExecutor:");
	
    Xale::Storage::BinaryFileManager execFm;
	Xale::Storage::FileStorageEngine execEngine(execFm, "executor-storage.bin");
	
    if (!execEngine.startup())
	{
		logger.error("Executor StorageEngine startup failed");
		return -1;
	}
	
    Xale::Execution::TableManager tableManager(execEngine);
	Xale::Execution::BasicExecutor executor(tableManager);
	
    // Create a table
    auto createTableStmt = std::make_unique<Xale::Query::CreateStatement>();
    createTableStmt->tableName = "users";
	auto createResult = executor.execute(createTableStmt.get());
    auto* usersTable = tableManager.getTable("users");
    if (usersTable)
    {
        usersTable->addColumn(Xale::DataStructure::ColumnDefinition("id", Xale::DataStructure::FieldType::Integer, true));
        usersTable->addColumn(Xale::DataStructure::ColumnDefinition("name", Xale::DataStructure::FieldType::String));
        usersTable->addColumn(Xale::DataStructure::ColumnDefinition("age", Xale::DataStructure::FieldType::Integer));
    }

    // Insert a row
    auto insertStmt = std::make_unique<Xale::Query::InsertStatement>();
    insertStmt->tableName = "users";
    insertStmt->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "1"));
    insertStmt->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::StringLiteral, "'John'"));
    insertStmt->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "25"));
	auto insertResult = executor.execute(insertStmt.get());
	
    // Select rows
    auto selectStmt = std::make_unique<Xale::Query::SelectStatement>();
    selectStmt->tableName = "users";
    selectStmt->columns.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::Wildcard, "*"));
    auto selectResult = executor.execute(selectStmt.get());

    logger.info("Select Result Rows: " + std::to_string(selectResult->getRowCount()));
    for (const auto& row : selectResult->getRows())
    {
        logger.info(" Row:");
        for (const auto& field : row.fields)
        {
            std::string valueStr = std::visit([](auto&& arg) -> std::string {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::monostate>)
                    return "NULL";
                else if constexpr (std::is_same_v<T, int>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, double>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, std::string>)
                    return arg;
                else
                    return "UNKNOWN";
                }, field.value);

            logger.info("   " + field.name + " = " + valueStr);
        }
    }

	// Insert more rows
    auto insertStmt1 = std::make_unique<Xale::Query::InsertStatement>();
    insertStmt1->tableName = "users";
    insertStmt1->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "2"));
    insertStmt1->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::StringLiteral, "'Tom'"));
    insertStmt1->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "200"));
    auto insertResult1 = executor.execute(insertStmt1.get());

    auto insertStmt2 = std::make_unique<Xale::Query::InsertStatement>();
    insertStmt2->tableName = "users";
    insertStmt2->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "3"));
    insertStmt2->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::StringLiteral, "'Fred'"));
    insertStmt2->values.push_back(Xale::Query::Expression(Xale::Query::ExpressionType::NumericLiteral, "10000"));
    auto insertResult2 = executor.execute(insertStmt2.get());

	// Select specific row
    // where 'id' is equal to '2'
    
	auto selectStmt2 = std::make_unique<Xale::Query::SelectStatement>();
	selectStmt2->tableName = "users";
    
    selectStmt2->where = std::make_unique<Xale::Query::WhereClause>();
	selectStmt2->where->condition = std::make_unique<Xale::Query::Expression>(Xale::Query::ExpressionType::BinaryOp);
	selectStmt2->where->condition->binary = std::make_unique<Xale::Query::BinaryExpression>();
	selectStmt2->where->condition->binary->left = std::make_unique<Xale::Query::Expression>(Xale::Query::ExpressionType::Identifier, "id");
	selectStmt2->where->condition->binary->op = "=";
	selectStmt2->where->condition->binary->right = std::make_unique<Xale::Query::Expression>(Xale::Query::ExpressionType::NumericLiteral, "2");
	
    auto selectResult2 = executor.execute(selectStmt2.get());

	logger.info("Select Result Rows: " + std::to_string(selectResult2->getRowCount()));

	for (const auto& row : selectResult2->getRows())
	{
		logger.info(" Row:");
		for (const auto& field : row.fields)
		{
			std::string valueStr = std::visit([](auto&& arg) -> std::string {
				using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::monostate>)
                    return "NULL";
                else if constexpr (std::is_same_v<T, int>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, double>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, std::string>)
                    return arg;
                else
                    return "UNKNOWN";
				}, field.value);
			logger.info("   " + field.name + " = " + valueStr);
		}
	}

    auto selectStmt3 = parser.parse("SELECT * FROM users WHERE id = 2");
    auto selectResult3 = executor.execute(selectStmt3.get());

    logger.info("Select Result Rows: " + std::to_string(selectResult3->getRowCount()));

	for (const auto& row : selectResult3->getRows())
	{
		logger.info(" Row:");
		for (const auto& field : row.fields)
		{
			std::string valueStr = std::visit([](auto&& arg) -> std::string {
				using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::monostate>)
                    return "NULL";
                else if constexpr (std::is_same_v<T, int>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, double>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, std::string>)
                    return arg;
                else
                    return "UNKNOWN";
				}, field.value);
			logger.info("   " + field.name + " = " + valueStr);
		}
	}

    execEngine.shutdown();
    */

    logger.info("");
    logger.info("");
    logger.info("Test Execution::BasicExecutor:");

    Xale::Storage::BinaryFileManager execFm;
	Xale::Storage::FileStorageEngine execEngine(execFm, "debug-engine-storage.bin");
	
    if (!execEngine.startup())
	{
		logger.error("Executor StorageEngine startup failed");
		return -1;
	}
	
    // Setup parser
    Xale::Query::BasicTokenizer parserTokenizer;
    Xale::Query::BasicParser parser(&parserTokenizer);

    // Setup executor
    Xale::Execution::TableManager tableManager(execEngine);
	Xale::Execution::BasicExecutor executor(tableManager);

    // Setup engine
    Xale::Engine::QueryEngine queryEngine(&parser, &executor);

    logger.info("");
    logger.info("=== Testing QueryEngine with different statement types ===");
    logger.info("");

    queryEngine.run("CREATE TABLE testTmp (id INT PRIMARY KEY, name STRING)");
    logger.info(queryEngine.getResultsToString());

    queryEngine.run("INSERT INTO testTmp VALUES 1, 'Alice'");
    logger.info(queryEngine.getResultsToString());
    
    queryEngine.run("INSERT INTO testTmp VALUES 2, 'Bob'");
    logger.info(queryEngine.getResultsToString());
    
    queryEngine.run("INSERT INTO testTmp VALUES 3, 'Charlie'");
    logger.info(queryEngine.getResultsToString());

    logger.info("");
    queryEngine.run("SELECT * FROM testTmp WHERE id != 2");
    logger.info("\n" + queryEngine.getResultsToString());

    execEngine.shutdown();

    return 0;
}
