#ifndef TABLE_MANAGER_TESTS_H
#define TABLE_MANAGER_TESTS_H

#include "TestsHelper.h"
#include "Execution/TableManager.h"
#include "Storage/BinaryFileManager.h"
#include "Storage/FileStorageEngine.h"
#include "DataStructure/DataTypes.h"
#include "Core/ExceptionHandler.h"

#define DECLARE_TABLE_MANAGER_TEST(name) DECLARE_TEST(EXECUTION, table_manager_##name)

namespace Xale::Tests
{
    DECLARE_TABLE_MANAGER_TEST(create_table)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-table-manager.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage);
            
            auto* table = manager.createTable("users");
            
            bool result = (table != nullptr) &&
                         (table->getName() == "users") &&
                         (manager.tableExists("users"));
            
            storage.shutdown();
            return result;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_TABLE_MANAGER_TEST(create_duplicate_table)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-table-manager.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage);
            
            auto* table1 = manager.createTable("users");
            auto* table2 = manager.createTable("users");
            
            bool result = (table1 != nullptr) && (table2 == nullptr);
            
            storage.shutdown();
            return result;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_TABLE_MANAGER_TEST(get_table)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-table-manager.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage);
            
            manager.createTable("users");
            auto* table = manager.getTable("users");
            
            bool result = (table != nullptr) && (table->getName() == "users");
            
            storage.shutdown();
            return result;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_TABLE_MANAGER_TEST(get_nonexistent_table)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-table-manager.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage);
            
            auto* table = manager.getTable("nonexistent");
            
            bool result = (table == nullptr);
            
            storage.shutdown();
            return result;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_TABLE_MANAGER_TEST(drop_table)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-table-manager.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage);
            
            manager.createTable("users");
            bool dropped = manager.dropTable("users");
            bool exists = manager.tableExists("users");
            
            bool result = dropped && !exists;
            
            storage.shutdown();
            return result;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_TABLE_MANAGER_TEST(drop_nonexistent_table)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-table-manager.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage);
            
            bool dropped = manager.dropTable("nonexistent");
            
            bool result = !dropped;
            
            storage.shutdown();
            return result;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_TABLE_MANAGER_TEST(table_exists)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-table-manager.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage);
            
            bool beforeCreate = manager.tableExists("users");
            manager.createTable("users");
            bool afterCreate = manager.tableExists("users");
            manager.dropTable("users");
            bool afterDrop = manager.tableExists("users");
            
            bool result = !beforeCreate && afterCreate && !afterDrop;
            
            storage.shutdown();
            return result;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }

    DECLARE_TABLE_MANAGER_TEST(get_table_names)
    {
        try
        {
            Xale::Storage::BinaryFileManager fm;
            Xale::Storage::FileStorageEngine storage(fm, "test-table-manager.bin");
            storage.startup();
            
            Xale::Execution::TableManager manager(storage);
            
            auto namesEmpty = manager.getTableNames();
            
            manager.createTable("users");
            manager.createTable("products");
            manager.createTable("orders");
            
            auto names = manager.getTableNames();
            
            bool result = (namesEmpty.size() == 0) &&
                         (names.size() == 3) &&
                         (std::find(names.begin(), names.end(), "users") != names.end()) &&
                         (std::find(names.begin(), names.end(), "products") != names.end()) &&
                         (std::find(names.begin(), names.end(), "orders") != names.end());
            
            storage.shutdown();
            return result;
        }
        catch (const Xale::Core::DbException&)
        {
            return false;
        }
    }
}

#endif // TABLE_MANAGER_TESTS_H
