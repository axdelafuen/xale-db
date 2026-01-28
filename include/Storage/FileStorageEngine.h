#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include "Core/ConfigurationPath.h"
#include "Storage/IStorageEngine.h"
#include "Storage/IFileManager.h"
#include "Storage/BinaryFileManager.h"
#include <Logger.h>
#include <filesystem>
#include <string>

#define DEFAULT_FILE_STORAGE_FILE_PATH \
    Xale::Core::Helper::getExecutableFolderPath() + "/xale-db-storage.bin"

namespace Xale::Storage 
{
    /*
	 * @brief Implementation of the storage engine
     */
    class FileStorageEngine : public IStorageEngine
    {
        public:
            FileStorageEngine(
                IFileManager& fm,
                const std::string path = DEFAULT_FILE_STORAGE_FILE_PATH);
            ~FileStorageEngine();
            bool startup() override;
            void shutdown() override;
        private:
            std::filesystem::path _path;
			IFileManager& _fileManager;
            bool _started = false;
            Xale::Logger::Logger<FileStorageEngine>& _logger;
    };
}

#endif // STORAGE_ENGINE_H
