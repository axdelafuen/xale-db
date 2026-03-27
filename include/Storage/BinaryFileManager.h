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
            /**
             * @brief Constructor for BinaryFileManager
             */
            BinaryFileManager();

            BinaryFileManager(const BinaryFileManager&) = delete;

            /**
             * @brief Destructor for BinaryFileManager
             */
            ~BinaryFileManager();

            /**
             * @brief Opens a binary file at the specified path
             * @param path The file path to open
             * @return true if the file is opened successfully, false otherwise
             */
            bool open(const std::filesystem::path& path) override;

            /**
             * @brief Closes the currently opened binary file
             */
            void close() override;

            /**
             * @brief Reads data from the binary file at the specified offset into the provided buffer
             * @param offset The offset in the file to start reading from
             * @param buffer The buffer to read data into
             * @param size The number of bytes to read
             * @return The number of bytes actually read
             */
            std::size_t readAt(std::uint64_t offset, void* buffer, std::size_t size) override;

            /**
             * @brief Writes data to the binary file at the specified offset from the provided buffer
             * @param offset The offset in the file to start writing to
             * @param buffer The buffer containing data to write
             * @param size The number of bytes write
             */
            std::size_t writeAt(std::uint64_t offset, const void* buffer, std::size_t size) override;

            /**
             * @brief Synchronizes the binary file to ensure all data is written to disk
             * @return true if the synchronization is successful, false otherwise
             */
            bool sync() override;

            /**
             * @brief Gets the size of the binary file
             */
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
