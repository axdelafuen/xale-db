#ifndef FILE_MANAGER_TESTS_H
#define FILE_MANAGER_TESTS_H

#include "Storage/FileManager.h"
#include "Core/ConfigurationPath.h"

#include <string>
#include <filesystem>

namespace Xale::Tests
{
    bool test_create_file()
    {
        Xale::Storage::FileManager fileManager = Xale::Storage::FileManager();
        const std::string currentPath = Xale::Core::Helper::getExecutableFolderPath();
        const std::string fileName = "storage-file-tus.bin";

        fileManager.open(currentPath + fileName);

        const bool isCreated = std::filesystem::exists(currentPath + fileName);

        // TODO: clean file

        return isCreated;
    }
}

#endif // FILE_MANAGER_TESTS_H
