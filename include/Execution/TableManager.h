#ifndef TABLE_MANAGER_H
#define TABLE_MANAGER_H

#include "DataStructure/Table.h"
#include "Storage/IStorageEngine.h"

#include <memory>

namespace Xale::Execution
{
    class TableManager
    {
        public:
            TableManager(Xale::Storage::IStorageEngine& storage);
            Xale::DataStructure::Table* createTable(const std::string& name);
            bool dropTable(const std::string& name);
            Xale::DataStructure::Table* getTable(const std::string& name);
            bool tableExists(const std::string& name) const;
            std::vector<std::string> getTableNames() const;

        private:
            Xale::Storage::IStorageEngine& _storage;
            std::unordered_map<std::string, std::unique_ptr<Xale::DataStructure::Table>> _tables;
    };
}

#endif // TABLE_MANAGER_H