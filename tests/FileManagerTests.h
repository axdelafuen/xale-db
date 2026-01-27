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

        return std::filesystem::exists(currentPath + fileName);
    }
}

#endif // FILE_MANAGER_TESTS_H
