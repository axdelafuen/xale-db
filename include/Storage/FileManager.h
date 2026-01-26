#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "IFileManager.h"
#include <fstream>
#include <filesystem>
#include <mutex>
#include <cstdint>

class FileManager : public IFileManager
{
    public:
        bool open(const std::filesystem::path& path) override;
        void close() override;
        std::size_t readAt(std::uint64_t offset, void* buffer, std::size_t size) override;
        std::size_t writeAt(std::uint64_t offset, const void* buffer, std::size_t size) override;
        bool sync() override;
        std::uint64_t size() const override;
        FileManager(const FileManager&) = delete;
        FileManager& operator=(const FileManager&) = delete;
    private:
        mutable std::mutex mtx_;
        std::filesystem::path path_;
        std::fstream file_;
};

#endif // FILE_MANAGER_H
