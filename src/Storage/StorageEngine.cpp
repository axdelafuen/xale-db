#include "Storage/StorageEngine.h"

namespace Xale::Storage
{
    StorageEngine::StorageEngine(const std::string path) :
        _logger(Xale::Logger::Logger<StorageEngine>::getInstance())
    {
		_path = Xale::Core::Helper::getExecutableFolderPath() + "/" + path;
    }

    StorageEngine::~StorageEngine()
    {
        if (_started) shutdown();
    }

    bool StorageEngine::startup()
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

    void StorageEngine::shutdown()
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

    IFileManager& StorageEngine::fileManager()
    {
        return _fileManager;
    }
}
