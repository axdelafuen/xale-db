#ifndef TABLE_MANAGER_H
#define TABLE_MANAGER_H

#include "DataStructure/Table.h"
#include "Storage/IStorageEngine.h"
#include "Storage/IFileManager.h"
#include "Storage/BinaryFileManager.h"

#include <cstring>
#include <fstream>

#include <memory>

namespace Xale::Execution
{
    class TableManager
    {
        public:
            TableManager(Xale::Storage::IStorageEngine& storage, Xale::Storage::IFileManager& fileManager);
            Xale::DataStructure::Table* createTable(const std::string& name);
            bool dropTable(const std::string& name);
            Xale::DataStructure::Table* getTable(const std::string& name);
            bool tableExists(const std::string& name) const;
            std::vector<std::string> getTableNames() const;

            void saveAllTables();
            void loadAllTables();

        private:
            Xale::Storage::IStorageEngine& _storage;
            Xale::Storage::IFileManager& _fileManager;
            std::unordered_map<std::string, std::unique_ptr<Xale::DataStructure::Table>> _tables;
            
            void saveTable(const Xale::DataStructure::Table& table);
            void loadTable(const std::string& tableName, const std::vector<char>& data);
    };
}

#endif // TABLE_MANAGER_H