#include "Execution/TableManager.h"

namespace Xale::Execution
{
	/*
	 * @brief Constructor for TableManager
	 * @param storage Reference to the storage engine
	 */
	TableManager::TableManager(Xale::Storage::IStorageEngine& storage)
		: _storage(storage)
	{}

	/*
	 * @brief Creates a new table with the given name
	 * @param name Name of the table to create
	 * @return Pointer to the created Xale::DataStructure::Table, or nullptr if it already exists
	 */
	Xale::DataStructure::Table* TableManager::createTable(const std::string& name)
	{
		if (tableExists(name))
			return nullptr;
		
		auto table = std::make_unique<Xale::DataStructure::Table>(name);
		Xale::DataStructure::Table* tablePtr = table.get();
		_tables[name] = std::move(table);
		
		return tablePtr;
	}

	/*
	 * @brief Drops the table with the given name
	 * @param name Name of the table to drop
	 * @return True if the table was dropped, false if it did not exist
	 */
	bool TableManager::dropTable(const std::string& name)
	{
		return _tables.erase(name) > 0;
	}

	/*
	 * @brief Retrieves the table with the given name
	 * @param name Name of the table to retrieve
	 * @return Pointer to the Xale::DataStructure::Table, or nullptr if it does not exist
	 */
	Xale::DataStructure::Table* TableManager::getTable(const std::string& name)
	{
		auto it = _tables.find(name);

		if (it != _tables.end())
			return it->second.get();

		return nullptr;
	}

	/*
	 * @brief Checks if a table with the given name exists
	 * @param name Name of the table to check
	 * @return True if the table exists, false otherwise
	 */
	bool TableManager::tableExists(const std::string& name) const
	{
		return _tables.find(name) != _tables.end();
	}

	/*
	 * @brief Retrieves the names of all existing tables
	 * @return Vector of table names
	 */
	std::vector<std::string> TableManager::getTableNames() const
	{
		std::vector<std::string> names;

		for (const auto& pair : _tables)
			names.push_back(pair.first);

		return names;
	}
}
