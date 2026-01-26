#include "Storage/StorageEngine.h"

namespace Xale::Storage
{
    StorageEngine::StorageEngine(const std::string path)
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
        _started = _fileManager.open(_path);
        return _started;
    }

    void StorageEngine::shutdown()
    {
        if (!_started) return;
        _fileManager.close();
        _started = false;
    }

    IFileManager& StorageEngine::fileManager()
    {
        return _fileManager;
    }
}