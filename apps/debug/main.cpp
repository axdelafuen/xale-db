#include "Logger.h"

#include "Core/Setup.h"
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

    // Setup config & engine
    auto& setup = Xale::Core::Setup::getInstance();
    if (!setup.initialize())
        return -1;
    auto& queryEngine = setup.getQueryEngine();

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

    // Stop setup
    setup.shutdown();

    return 0;
}
