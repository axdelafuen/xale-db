#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "Storage/IFileManager.h"
#include "Core/ExceptionHandler.h"
#include <Logger.h>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <cstdint>

namespace Xale::Storage 
{
    class FileManager : public IFileManager
    {
        public:
            FileManager();
            FileManager(const FileManager&) = delete;
            ~FileManager();
            bool open(const std::filesystem::path& path) override;
            void close() override;
            std::size_t readAt(std::uint64_t offset, void* buffer, std::size_t size) override;
            std::size_t writeAt(std::uint64_t offset, const void* buffer, std::size_t size) override;
            bool sync() override;
            std::uint64_t size() const override;
            FileManager& operator=(const FileManager&) = delete;
        private:
            mutable std::mutex _mutex;
            std::filesystem::path _path;
            std::fstream _file;
            Xale::Logger::Logger<FileManager>& _logger;
    };
}

#endif // FILE_MANAGER_H
