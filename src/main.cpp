#include "Logger.h"
#include "Core/ExceptionHandler.h"
#include "Core/AssertException.h"
#include "Core/Setup.h"
#include "Storage/StorageEngine.h"
#include <vector>
#include <string>

/**
 * @brief Debug entrypoint
 */
int main(int argc, char *argv[])
{
	// Setup logger
    auto& logger = Xale::Logger::Logger<void>::getInstance();

    if (argc > 1)
    {
        logger.error("No arguments expected !");
        return -1;
    }

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
    Xale::Storage::StorageEngine engine("test_storage.bin");
    if (!engine.startup())
    {
        logger.error("StorageEngine startup failed");
        return -1;
    }

    auto& fm = engine.fileManager();

    const std::string payload = "XALE_BINARY_TEST";
    const std::size_t written = fm.writeAt(0, payload.data(), payload.size());
    fm.sync();

    std::vector<char> readbuf(payload.size());
    const std::size_t read = fm.readAt(0, readbuf.data(), readbuf.size());

    if (read != payload.size() || std::string(readbuf.begin(), readbuf.end()) != payload)
    {
        logger.error("Storage read/write verification failed");
    }
    else
    {
        logger.debug("Storage read/write verification succeeded");
    }

    engine.shutdown();

    return 0;
}
