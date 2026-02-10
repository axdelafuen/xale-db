#ifndef STORAGE_I_FILE_MANAGER_H
#define STORAGE_I_FILE_MANAGER_H

#include <filesystem>
#include <string_view>

namespace Xale::Storage 
{
    /**
	 * @brief Interface for file management operations
     */
    class IFileManager
    {
        public:
            virtual bool open(const std::filesystem::path& path) = 0;
            virtual void close() = 0;
            virtual std::size_t readAt(std::uint64_t offset, void* buffer, std::size_t size) = 0;
            virtual std::size_t writeAt(std::uint64_t offset, const void* buffer, std::size_t size) = 0;
            virtual bool sync() = 0;
            virtual std::uint64_t size() const = 0;
    };
}

#endif // STORAGE_I_FILE_MANAGER_H