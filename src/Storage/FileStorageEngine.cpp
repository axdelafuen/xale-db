#include "Storage/FileStorageEngine.h"

namespace Xale::Storage
{
    FileStorageEngine::FileStorageEngine(IFileManager& fm, const std::string path):
        _logger(Xale::Logger::Logger<FileStorageEngine>::getInstance()),
        _fileManager(fm)
    {
		_path = path;
    }

    FileStorageEngine::~FileStorageEngine()
    {
        if (_started) shutdown();
    }

    bool FileStorageEngine::startup()
    {
        if (_started) return true;

        try {
            _started = _fileManager.open(_path);
        }
        catch (const Xale::Core::DbException& e)
        {
            _logger.error("Startup failed: " + std::string(e.what())); 
            return false;
        }

        return _started;
    }

    void FileStorageEngine::shutdown()
    {
        if (!_started) return;
        
        _logger.debug("Shutdown engine...");
        
        try {
            _fileManager.close();
        }
        catch (const Xale::Core::DbException& e)
        {
            _logger.error("Shutdown failed: " + std::string(e.what())); 
            return;
        }

        _started = false;
    }
}
