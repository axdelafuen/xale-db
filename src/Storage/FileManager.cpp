#include "Storage/BinaryFileManager.h"

namespace Xale::Storage
{
    /**
	 * @brief Constructor for BinaryFileManager
     */
    BinaryFileManager::BinaryFileManager() : 
        _logger(Xale::Logger::Logger<BinaryFileManager>::getInstance())
    {}

	/**
     * @brief Destructor for BinaryFileManager
	 */
    BinaryFileManager::~BinaryFileManager()
    {
        if (_file.is_open()) {
            _file.flush();
            _file.close();
	    }
    }

    /**
	 * @brief Opens a binary file at the specified path
	 * @param path The file path to open
	 * @return true if the file is opened successfully, false otherwise
     */
    bool BinaryFileManager::open(const std::filesystem::path& path)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _path = path;

        _file.open(_path, std::ios::in | std::ios::out | std::ios::binary);

        if (!_file.is_open()) {
            std::ofstream create(_path, std::ios::binary);
        
            if (!create)
            {
                THROW_DB_EXCEPTION(
                    Xale::Core::ExceptionCode::StorageOpen, 
                    "Storage data file failed to created.");
            }
        
            create.close();
            _file.open(_path, std::ios::in | std::ios::out | std::ios::binary);
     
            if (!_file.is_open())
            {
                THROW_DB_EXCEPTION(
                    Xale::Core::ExceptionCode::StorageOpen, 
                    "Storage data file failed to open.");
            }
        }

        _logger.debug("Storage file opened successfully.");

        return true;
    }

    /**
	 * @brief Closes the currently opened binary file
     */
    void BinaryFileManager::close()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_file.is_open()) {
            _file.flush();
            _file.close();
        }
    }

    /**
	 * @brief Reads data from the binary file at the specified offset into the provided buffer
	 * @param offset The offset in the file to start reading from
	 * @param buffer The buffer to read data into
	 * @param size The number of bytes to read
	 * @return The number of bytes actually read
     */
    std::size_t BinaryFileManager::readAt(std::uint64_t offset, void* buffer, std::size_t size)
    {
        if (!buffer || size == 0)
        {
            THROW_DB_EXCEPTION(
                Xale::Core::ExceptionCode::ReadFile,
                "No buffer initialized.");
        }

        std::lock_guard<std::mutex> lock(_mutex);
        if (!_file.is_open())
        {
            THROW_DB_EXCEPTION(
                Xale::Core::ExceptionCode::ReadFile,
                "File not open.");
        }

        _file.clear();
        _file.seekg(static_cast<std::streamoff>(offset), std::ios::beg);
        if (!_file) 
        {
            THROW_DB_EXCEPTION(
                Xale::Core::ExceptionCode::ReadFile,
                "File failed to clear.");
        }

        _file.read(reinterpret_cast<char*>(buffer), static_cast<std::streamsize>(size));

        _logger.debug("File read successfully (" + std::to_string(size) + " bytes)");

        return static_cast<std::size_t>(_file.gcount());
    }

    /**
	 * @brief Writes data to the binary file at the specified offset from the provided buffer
	 * @param offset The offset in the file to start writing to
	 * @param buffer The buffer containing data to write
	 * @param size The number of bytes write
     */
    std::size_t BinaryFileManager::writeAt(std::uint64_t offset, const void* buffer, std::size_t size)
    {
        if (!buffer || size == 0)
        {
            THROW_DB_EXCEPTION(
                Xale::Core::ExceptionCode::WriteFile,
                "No buffer initialized.");
        }

        std::lock_guard<std::mutex> lock(_mutex);

        if (!_file.is_open()) 
        {
            THROW_DB_EXCEPTION(
                Xale::Core::ExceptionCode::WriteFile,
                "File not open.");
        }
        
        _file.clear();
        _file.seekp(static_cast<std::streamoff>(offset), std::ios::beg);

        if (!_file)
        {
            THROW_DB_EXCEPTION(
                Xale::Core::ExceptionCode::WriteFile,
                "File failed to clear.");
        }

        _file.write(reinterpret_cast<const char*>(buffer), static_cast<std::streamsize>(size));

        if (!_file) 
        {
            THROW_DB_EXCEPTION(
                    Xale::Core::ExceptionCode::WriteFile,
                    "File failed to write buffer.");
        }

        _logger.debug("File write sccessfully (" + std::to_string(size) + " bytes)"); 
        return size;
    }

    /**
	 * @brief Synchronizes the binary file to ensure all data is written to disk
	 * @return true if the synchronization is successful, false otherwise
     */
    bool BinaryFileManager::sync()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!_file.is_open())
        {
            THROW_DB_EXCEPTION(
                Xale::Core::ExceptionCode::SyncFile,
                "File not open.");
        }
        _file.flush();
        // Platform-specific fsync could be added here for stronger durability
        return static_cast<bool>(_file);
    }

    /**
	 * @brief Gets the size of the binary file
     */
    std::uint64_t BinaryFileManager::size() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        try {
            if (_path.empty() || !std::filesystem::exists(_path)) return 0;
            return static_cast<std::uint64_t>(std::filesystem::file_size(_path));
        } catch (...) {
            return 0;
        }
    }
}
