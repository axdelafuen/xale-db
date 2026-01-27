#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include "Core/ConfigurationPath.h"
#include "Storage/IStorageEngine.h"
#include "Storage/IFileManager.h"
#include "Storage/FileManager.h"
#include <Logger.h>
#include <filesystem>

namespace Xale::Storage 
{
    class StorageEngine : public IStorageEngine
    {
        public:
            explicit StorageEngine(const std::string path = 
                Xale::Core::Helper::getExecutableFolderPath() + "/xale-db-storage.bin");
            ~StorageEngine();
            bool startup() override;
            void shutdown() override;
            IFileManager& fileManager() override;

        private:
            std::filesystem::path _path;
            FileManager _fileManager;
            bool _started = false;
            Xale::Logger::Logger<StorageEngine>& _logger;
    };
}

#endif // STORAGE_ENGINE_H
