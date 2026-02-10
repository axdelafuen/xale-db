#ifndef STORAGE_I_STORAGE_ENGINE_H
#define STORAGE_I_STORAGE_ENGINE_H

#include "Storage/IFileManager.h"

namespace Xale::Storage 
{
    /**
	 * @brief Interface for storage engine operations
     */
    class IStorageEngine
    {
        public:
            virtual bool startup() = 0;
            virtual void shutdown() = 0;
    };
}

#endif // STORAGE_I_STORAGE_ENGINE_H
