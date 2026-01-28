#include "Logger.h"

#include "Core/ExceptionHandler.h"
#include "Core/AssertException.h"
#include "Core/Setup.h"
#include "Storage/StorageEngine.h"
#include "DataStructure/Node.h"

#include <vector>
#include <string>

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

    Xale::Storage::StorageEngine engine("test_storage.bin");
    if (!engine.startup())
    {
        logger.error("StorageEngine startup failed");
        return -1;
    }

    auto& fm = engine.fileManager();

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
    logger.info("Test DataStructure::Node:");

    Xale::DataStructure::Node<int, std::string> leaf(true);

	logger.info("Leaf Node test:");
    
    leaf.insertLeaf(10, "ten");
    leaf.insertLeaf(5, "five");
    leaf.insertLeaf(20, "twenty");
    for (std::size_t i = 0; i < 3; ++i) {
        logger.warning("  key[" + std::to_string(i) + "] = "
			+ (i == 0 ? "5" : (i == 1 ? "10" : "20")));
    }

    logger.info("Inner Node test:");

    Xale::DataStructure::Node<int, std::string> inner(false);
    Xale::DataStructure::Node<int, std::string>* child1 = new Xale::DataStructure::Node<int, std::string>(true);
    Xale::DataStructure::Node<int, std::string>* child2 = new Xale::DataStructure::Node<int, std::string>(true);
    Xale::DataStructure::Node<int, std::string>* child3 = new Xale::DataStructure::Node<int, std::string>(true);

	child1->insertLeaf(1, "one");
	child1->insertLeaf(2, "two");

	child2->insertLeaf(11, "eleven");

	child3->insertLeaf(21, "twenty-one");
	child3->insertInner(0, child1); // as a leaf it should not work

    inner.insertInner(15, child2);
    inner.insertInner(4, child1);

	delete child1;
	delete child2;

    return 0;
}
