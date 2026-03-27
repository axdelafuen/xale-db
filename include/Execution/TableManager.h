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
    /**
     * @brief Manages the lifecycle of tables, including creation, retrieval, deletion, and persistence.
     */
    class TableManager
    {
        public:
            /**
             * @brief Constructor for TableManager
             * @param storage Reference to the storage engine
             * @param fileManager Reference to the file manager
             */
            TableManager(Xale::Storage::IStorageEngine& storage, Xale::Storage::IFileManager& fileManager);

            /**
             * @brief Creates a new table with the given name
             * @param name Name of the table to create
             * @return Pointer to the created Xale::DataStructure::Table, or nullptr if it already exists
             */
            Xale::DataStructure::Table* createTable(const std::string& name);

            /**
             * @brief Drops the table with the given name
             * @param name Name of the table to drop
             * @return True if the table was dropped, false if it did not exist
             */
            bool dropTable(const std::string& name);

            /**
             * @brief Retrieves the table with the given name
             * @param name Name of the table to retrieve
             * @return Pointer to the Xale::DataStructure::Table, or nullptr if it does not exist
             */
            Xale::DataStructure::Table* getTable(const std::string& name);

            /**
             * @brief Checks if a table with the given name exists
             * @param name Name of the table to check
             * @return True if the table exists, false otherwise
             */
            bool tableExists(const std::string& name) const;

            /**
             * @brief Retrieves the names of all existing tables
             * @return Vector of table names
             */
            std::vector<std::string> getTableNames() const;

            /**
             * @brief Save all tables to disk
             * File format:
             * [4 bytes: table_count]
             * For each table:
             *   [4 bytes: table_name_length]
             *   [N bytes: table_name]
             *   [4 bytes: table_data_length]
             *   [M bytes: table_data (serialized)]
             */
            void saveAllTables();

            /**
             * @brief Load all tables from disk
             * File format:
             * [4 bytes: table_count]
             * For each table:
             *   [4 bytes: table_name_length]
             *   [N bytes: table_name]
             *   [4 bytes: table_data_length]
             *   [M bytes: table_data (serialized)]
             */
            void loadAllTables();

        private:
            Xale::Storage::IStorageEngine& _storage;
            Xale::Storage::IFileManager& _fileManager;
            std::unordered_map<std::string, std::unique_ptr<Xale::DataStructure::Table>> _tables;
            
            /**
             * @brief Save a single table to disk
             * @param table The table to save
             */
            void saveTable(const Xale::DataStructure::Table& table);

            /**
             * @brief Load a single table from disk
             * @param tableName Name of the table
             * @param data Serialized table data
             */
            void loadTable(const std::string& tableName, const std::vector<char>& data);
    };
}

#endif // TABLE_MANAGER_H
