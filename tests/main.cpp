#include <Logger.h>
#include "FileManagerTests.h"
#include "StorageEngineTests.h"

#include <string>
#include <iostream>

const int total = 7;
int failed = 0;
int passed = 0;

#define RUN_TEST(func) \
    runTest(#func, func)

void runTest(
    const std::string& testName,
    bool (*testFunc)())
{
    if (!testFunc())
    {
        std::cout << "\t[FAIL] " << testName << std::endl;
        ++failed;
    }
    else
    {
        std::cout << "\t[PASS] " << testName << std::endl;
        ++passed;
    }
}

int main()
{
    using namespace Xale::Tests;

    // Disable logs for tests
    Xale::Logger::Logger<void>::setIsDebugEnable(false);
    Xale::Logger::Logger<void>::setLogToConsole(false);
    Xale::Logger::Logger<void>::setLogToFile(false);

    // FileManagerTests
    std::cout << "------------------------------------" << std::endl;
    std::cout << "TESTS> Xale::Storage::FileManager" << std::endl;
    RUN_TEST(test_open_file_success);
	RUN_TEST(test_write_and_read_file_success);
	RUN_TEST(test_close_file_success);
    RUN_TEST(test_write_file_no_buffer_fail);
    RUN_TEST(test_sync_with_no_file_fail);

    // StorageEngineTests
    std::cout << "\n------------------------------------" << std::endl;
    std::cout << "TESTS> Xale::Storage::StorageEngine" << std::endl;
	RUN_TEST(test_storage_engine_startup_success);
	RUN_TEST(test_storage_engine_multiple_startup_success);

    // Tests results
    if (failed != 0 || passed + failed != total)
    {
        std::cout << failed << " test(s) failed (" << passed << "/" << total << " passed)" << std::endl;
        return 1;
    }

    std::cout << std::endl << "All tests passed (" << passed << "/" << total << " passed)" << std::endl;
    
    return 0;
}
