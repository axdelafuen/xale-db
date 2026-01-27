#ifndef FILE_MANAGER_TESTS_H
#define FILE_MANAGER_TESTS_H

#include "Storage/FileManager.h"
#include "Core/ConfigurationPath.h"
#include "Core/ExceptionHandler.h"
#include "TestsHelper.h"

#include <string>
#include <filesystem>
#include <random>
#include <vector>

namespace Xale::Tests
{
    bool test_open_file_success()
    {
        Xale::Storage::FileManager fileManager = Xale::Storage::FileManager();
        const std::string currentPath = Xale::Core::Helper::getExecutableFolderPath();

        fileManager.open(currentPath + TEST_FILE_NAME);

        const bool isCreated = std::filesystem::exists(currentPath + TEST_FILE_NAME);

        return isCreated;
    }

    bool test_write_and_read_file_success()
    {
        Xale::Storage::FileManager fileManager = Xale::Storage::FileManager();
        const std::string currentPath = Xale::Core::Helper::getExecutableFolderPath();
        fileManager.open(currentPath + TEST_FILE_NAME);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1000000);
        int randomInt = dist(gen);

        const std::string writeData = std::string(TEST_DATA_INPUT) + std::to_string(randomInt);
        fileManager.writeAt(0, writeData.data(), writeData.size());
        fileManager.sync();

        std::vector<char> readBuffer(writeData.size());
        fileManager.readAt(0, readBuffer.data(), readBuffer.size());
        const bool isDataEqual = (writeData == std::string(readBuffer.data(), readBuffer.size()));

        return isDataEqual;
    }

    bool test_close_file_success()
    {
        bool isWriteExceptionThrown = false;
        bool isReadExceptionThrown = false;

        Xale::Storage::FileManager fileManager = Xale::Storage::FileManager();
        const std::string currentPath = Xale::Core::Helper::getExecutableFolderPath();
        fileManager.open(currentPath + TEST_FILE_NAME);
        fileManager.close();

        try 
        {
            const std::string writeData = std::string(TEST_DATA_INPUT);
            fileManager.writeAt(0, writeData.data(), writeData.size());
        } 
        catch (const Xale::Core::DbException& e) 
        {
            isWriteExceptionThrown = e.getCode() == Xale::Core::ExceptionCode::WriteFile;
        }

        try 
        {
            std::vector<char> readBuffer(100);
            fileManager.readAt(0, readBuffer.data(), readBuffer.size());
		}
        catch (const Xale::Core::DbException& e) 
        {
			isReadExceptionThrown = e.getCode() == Xale::Core::ExceptionCode::ReadFile;
        }

        return isWriteExceptionThrown && isReadExceptionThrown;
	}

    bool test_write_file_no_buffer_fail()
    {
        Xale::Storage::FileManager fileManager = Xale::Storage::FileManager();
        const std::string currentPath = Xale::Core::Helper::getExecutableFolderPath();
        fileManager.open(currentPath + TEST_FILE_NAME);

        const std::string writeData = std::string(TEST_DATA_INPUT);

        try 
        {
            fileManager.writeAt(0, nullptr, writeData.size());
            fileManager.sync();
        }
        catch (const Xale::Core::DbException&)
        {
            return true;
		}

        return false;
    }

    bool test_sync_with_no_file_fail()
    {
        Xale::Storage::FileManager fileManager = Xale::Storage::FileManager();
        try
        {
			fileManager.sync();
        }
        catch (const Xale::Core::DbException& e)
        {
            return e.getCode() == Xale::Core::ExceptionCode::SyncFile;
        }

        return false;
    }
}

#endif // FILE_MANAGER_TESTS_H
