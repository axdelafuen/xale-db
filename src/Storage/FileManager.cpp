#include "Storage/FileManager.h"

bool FileManager::open(const std::filesystem::path& path)
{
    std::lock_guard<std::mutex> lock(mtx_);
    path_ = path;

    file_.open(path_, std::ios::in | std::ios::out | std::ios::binary);
    
    if (!file_.is_open()) {
        std::ofstream create(path_, std::ios::binary);
        
        if (!create)
            return false;
        
        create.close();
        file_.open(path_, std::ios::in | std::ios::out | std::ios::binary);
     
        if (!file_.is_open())
            return false;
    }

    return true;
}

void FileManager::close()
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (file_.is_open()) {
        file_.flush();
        file_.close();
    }
}

std::size_t FileManager::readAt(std::uint64_t offset, void* buffer, std::size_t size)
{
    if (!buffer || size == 0)
        return 0;

    std::lock_guard<std::mutex> lock(mtx_);
    if (!file_.is_open()) return 0;

    file_.clear();
    file_.seekg(static_cast<std::streamoff>(offset), std::ios::beg);
    if (!file_) return 0;

    file_.read(reinterpret_cast<char*>(buffer), static_cast<std::streamsize>(size));
    return static_cast<std::size_t>(file_.gcount());
}

std::size_t FileManager::writeAt(std::uint64_t offset, const void* buffer, std::size_t size)
{
    if (!buffer || size == 0) 
        return 0;

    std::lock_guard<std::mutex> lock(mtx_);

    if (!file_.is_open()) 
        return 0;

    file_.clear();
    file_.seekp(static_cast<std::streamoff>(offset), std::ios::beg);

    if (!file_) 
        return 0;

    file_.write(reinterpret_cast<const char*>(buffer), static_cast<std::streamsize>(size));

    if (!file_) 
        return 0;

    return size;
}

bool FileManager::sync()
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (!file_.is_open()) return false;
    file_.flush();
    // Platform-specific fsync could be added here for stronger durability
    return static_cast<bool>(file_);
}

std::uint64_t FileManager::size() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    try {
        if (path_.empty() || !std::filesystem::exists(path_)) return 0;
        return static_cast<std::uint64_t>(std::filesystem::file_size(path_));
    } catch (...) {
        return 0;
    }
}
