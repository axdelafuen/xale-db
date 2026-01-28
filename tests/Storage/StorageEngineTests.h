#ifndef STORAGE_ENGINE_TESTS_H
#define STORAGE_ENGINE_TESTS_H

#include "TestsHelper.h"
#include "Storage/StorageEngine.h"

#define DECLARE_ENGINE_TEST(name) DECLARE_TEST(STORAGE, storage_engine_##name)

namespace Xale::Tests
{
	DECLARE_ENGINE_TEST(startup_success)
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

	DECLARE_ENGINE_TEST(multiple_startup_success)
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