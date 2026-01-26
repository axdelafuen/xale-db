#include "Storage/FileManager.h"

namespace Xale::Storage
{

    FileManager::~FileManager()
    {
        if (_file.is_open()) {
            _file.flush();
            _file.close();
	    }
    }

    bool FileManager::open(const std::filesystem::path& path)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _path = path;

        _file.open(_path, std::ios::in | std::ios::out | std::ios::binary);
    
        if (!_file.is_open()) {
            std::ofstream create(_path, std::ios::binary);
        
            if (!create)
                return false;
        
            create.close();
            _file.open(_path, std::ios::in | std::ios::out | std::ios::binary);
     
            if (!_file.is_open())
                return false;
        }

        return true;
    }

    void FileManager::close()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_file.is_open()) {
            _file.flush();
            _file.close();
        }
    }

    std::size_t FileManager::readAt(std::uint64_t offset, void* buffer, std::size_t size)
    {
        if (!buffer || size == 0)
            return 0;

        std::lock_guard<std::mutex> lock(_mutex);
        if (!_file.is_open()) return 0;

        _file.clear();
        _file.seekg(static_cast<std::streamoff>(offset), std::ios::beg);
        if (!_file) return 0;

        _file.read(reinterpret_cast<char*>(buffer), static_cast<std::streamsize>(size));
        return static_cast<std::size_t>(_file.gcount());
    }

    std::size_t FileManager::writeAt(std::uint64_t offset, const void* buffer, std::size_t size)
    {
        if (!buffer || size == 0) 
            return 0;

        std::lock_guard<std::mutex> lock(_mutex);

        if (!_file.is_open()) 
            return 0;

        _file.clear();
        _file.seekp(static_cast<std::streamoff>(offset), std::ios::beg);

        if (!_file) 
            return 0;

        _file.write(reinterpret_cast<const char*>(buffer), static_cast<std::streamsize>(size));

        if (!_file) 
            return 0;

        return size;
    }

    bool FileManager::sync()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!_file.is_open()) return false;
        _file.flush();
        // Platform-specific fsync could be added here for stronger durability
        return static_cast<bool>(_file);
    }

    std::uint64_t FileManager::size() const
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