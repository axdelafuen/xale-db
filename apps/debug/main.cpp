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

    // Setup fileManagement
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
    Xale::Execution::TableManager tableManager(execEngine, execFm);
	Xale::Execution::BasicExecutor executor(tableManager);

    // Setup engine
    Xale::Engine::QueryEngine queryEngine(&parser, &executor);

    // Create table and insert data (only if table doesn't exist)
    bool tableCreated = false;
    try{
        queryEngine.run("CREATE TABLE testTmp (id INT PRIMARY KEY, name STRING)");
        logger.info(queryEngine.getResultsToString());
        tableCreated = true;
    }
    catch(...)
    {
        logger.warning("Table already exists, loading from disk");
    }

    // Insert data only if we just created the table
    if (tableCreated)
    {
        queryEngine.run("INSERT INTO testTmp VALUES 1, 'Alice'");
        logger.info(queryEngine.getResultsToString());
        
        queryEngine.run("INSERT INTO testTmp VALUES 2, 'Bob'");
        logger.info(queryEngine.getResultsToString());
        
        queryEngine.run("INSERT INTO testTmp VALUES 3, 'Charlie'");
        logger.info(queryEngine.getResultsToString());
    }

    // Select data
    queryEngine.run("SELECT * FROM testTmp");
    logger.info("\n" + queryEngine.getResultsToString());

    // test using a 2nd table
    bool secTableCreated = false;
     try{
        queryEngine.run("CREATE TABLE xaletesting (key INT, value STRING)");
        logger.info(queryEngine.getResultsToString());
        secTableCreated = true;
    }
    catch(...)
    {
        logger.warning("Table already exists, loading from disk");
    }

    if (secTableCreated)
    {
        queryEngine.run("INSERT INTO xaletesting VALUES 17, 'vincento'");
        logger.info(queryEngine.getResultsToString());
        
        queryEngine.run("INSERT INTO xaletesting VALUES 9999, 'tom'");
        logger.info(queryEngine.getResultsToString());
    }

    queryEngine.run("SELECT * FROM xaletesting WHERE key = 17");
    logger.info("\n" + queryEngine.getResultsToString());

    execEngine.shutdown();

    return 0;
}
