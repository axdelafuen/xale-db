#ifndef STORAGE_FILE_MANAGER_H
#define STORAGE_FILE_MANAGER_H

#include "Storage/IFileManager.h"
#include "Core/ExceptionHandler.h"
#include <Logger.h>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <cstdint>

namespace Xale::Storage 
{
    /**
	 * @brief Concrete implementation of IFileManager for binary file operations
     */
    class BinaryFileManager : public IFileManager
    {
        public:
            BinaryFileManager();
            BinaryFileManager(const BinaryFileManager&) = delete;
            ~BinaryFileManager();
            bool open(const std::filesystem::path& path) override;
            void close() override;
            std::size_t readAt(std::uint64_t offset, void* buffer, std::size_t size) override;
            std::size_t writeAt(std::uint64_t offset, const void* buffer, std::size_t size) override;
            bool sync() override;
            std::uint64_t size() const override;
            BinaryFileManager& operator=(const BinaryFileManager&) = delete;
        private:
            mutable std::mutex _mutex;
            std::filesystem::path _path;
            std::fstream _file;
            Xale::Logger::Logger<BinaryFileManager>& _logger;
    };
}

#endif // STORAGE_FILE_MANAGER_H
