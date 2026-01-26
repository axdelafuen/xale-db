#ifndef I_STORAGE_ENGINE_H
#define I_STORAGE_ENGINE_H

#include "IFileManager.h"

class IStorageEngine
{
    public:
        virtual bool startup() = 0;
        virtual void shutdown() = 0;
        virtual IFileManager& fileManager() = 0;
};

#endif // I_STORAGE_ENGINE_H
