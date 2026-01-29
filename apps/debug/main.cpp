#include "Logger.h"

#include "Core/ExceptionHandler.h"
#include "Core/AssertException.h"
#include "Core/Setup.h"
#include "Storage/BinaryFileManager.h"
#include "Storage/FileStorageEngine.h"
#include "DataStructure/BPlusTree.h"
#include "Query/BasicTokenizer.h"
#include "Query/BasicParser.h"

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

    return 0;
}
