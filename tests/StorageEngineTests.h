#ifndef STORAGE_ENGINE_TESTS_H
#define STORAGE_ENGINE_TESTS_H

#include "Storage/StorageEngine.h"
#include "TestsHelper.h"

namespace Xale::Tests
{
	bool test_storage_engine_startup_success()
	{
		using namespace Xale::Storage;
		StorageEngine engine(TEST_FILE_NAME);
		bool started = engine.startup();
		if (!started)
		{
			return false;
		}
		engine.shutdown();
		return true;
	}

	bool test_storage_engine_multiple_startup_success()
	{
		using namespace Xale::Storage;
		StorageEngine engine(TEST_FILE_NAME);
		bool started = engine.startup();
		if (!started)
		{
			return false;
		}
		started = engine.startup();
		if (!started)
		{
			return false;
		}
		engine.shutdown();
		return true;
	}
}

#endif // STORAGE_ENGINE_TESTS_H