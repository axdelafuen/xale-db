#include "Logger.h"
#include "Core/ExceptionHandler.h"
#include "Core/AssertException.h"
#include "Core/Setup.h"

/**
 * @brief Debug entrypoint
 */
int main(int argc, char *argv[])
{
    auto& logger = Xale::Logger::Logger<void>::getInstance();

    if (argc > 1)
    {
        logger.error("No arguments expected !");
        return -1;
    }

    std::string error;
    if (!Xale::Core::Init::setup(error))
    {
        logger.error("Setup failed: " + error);
        return -1;
    }

    logger.debug("Test debug");

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
    
    return 0;
}
