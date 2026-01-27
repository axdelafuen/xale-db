#include <Logger.h>
#include "FileManagerTests.h"

#include <string>
#include <iostream>

const int total = 1;
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
        std::cout << "[FAIL] " << testName << std::endl;
        ++failed;
    }
    else
    {
        std::cout << "[PASS] " << testName << std::endl;
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
    RUN_TEST(test_create_file);

    // Tests results
    if (failed != 0 || passed + failed != total)
    {
        std::cout << failed << " test(s) failed (" << passed << "/" << total << " passed)" << std::endl;
        return 1;
    }

    std::cout << std::endl << "All tests passeded (" << passed << "/" << total << " passed)" << std::endl;
    
    return 0;
}
