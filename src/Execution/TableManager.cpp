#include "Execution/TableManager.h"

namespace Xale::Execution
{
	/*
	 * @brief Constructor for TableManager
	 * @param storage Reference to the storage engine
	 * @param fileManager Reference to the file manager
	 */
	TableManager::TableManager(Xale::Storage::IStorageEngine& storage, Xale::Storage::IFileManager& fileManager)
		: _storage(storage), _fileManager(fileManager)
	{
		loadAllTables();
	}

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
		
		saveAllTables();
		
		return tablePtr;
	}

	/*
	 * @brief Drops the table with the given name
	 * @param name Name of the table to drop
	 * @return True if the table was dropped, false if it did not exist
	 */
	bool TableManager::dropTable(const std::string& name)
	{
		bool result = _tables.erase(name) > 0;
		
		// Auto-save after dropping table
		if (result)
			saveAllTables();
		
		return result;
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

	/*
	 * @brief Save all tables to disk
	 * File format:
	 * [4 bytes: table_count]
	 * For each table:
	 *   [4 bytes: table_name_length]
	 *   [N bytes: table_name]
	 *   [4 bytes: table_data_length]
	 *   [M bytes: table_data (serialized)]
	 */
	void TableManager::saveAllTables()
	{
		// Calculate total size needed
		size_t totalSize = sizeof(uint32_t); // table count
		
		std::vector<std::pair<std::string, std::vector<char>>> serializedTables;
		
		for (const auto& pair : _tables)
		{
			std::vector<char> data = pair.second->serialize();
			serializedTables.push_back({pair.first, std::move(data)});
			
			totalSize += sizeof(uint32_t); // name length
			totalSize += pair.first.size(); // name
			totalSize += sizeof(uint32_t); // data length
			totalSize += serializedTables.back().second.size(); // data
		}
		
		// Build file data
		std::vector<char> fileData;
		fileData.reserve(totalSize);
		
		// Write table count
		uint32_t tableCount = static_cast<uint32_t>(_tables.size());
		fileData.insert(fileData.end(), 
			reinterpret_cast<char*>(&tableCount), 
			reinterpret_cast<char*>(&tableCount) + sizeof(uint32_t));
		
		// Write each table
		for (const auto& [name, data] : serializedTables)
		{
			// Write name length
			uint32_t nameLen = static_cast<uint32_t>(name.size());
			fileData.insert(fileData.end(),
				reinterpret_cast<char*>(&nameLen),
				reinterpret_cast<char*>(&nameLen) + sizeof(uint32_t));
			
			// Write name
			fileData.insert(fileData.end(), name.begin(), name.end());
			
			// Write data length
			uint32_t dataLen = static_cast<uint32_t>(data.size());
			fileData.insert(fileData.end(),
				reinterpret_cast<char*>(&dataLen),
				reinterpret_cast<char*>(&dataLen) + sizeof(uint32_t));
			
			// Write data
			fileData.insert(fileData.end(), data.begin(), data.end());
		}
		
		// Write to file at offset 0
		_fileManager.writeAt(0, fileData.data(), fileData.size());
		
		// Flush to ensure data is written
		_fileManager.sync();
	}

	/*
	 * @brief Load all tables from disk
	 * File format:
	 * [4 bytes: table_count]
	 * For each table:
	 *   [4 bytes: table_name_length]
	 *   [N bytes: table_name]
	 *   [4 bytes: table_data_length]
	 *   [M bytes: table_data (serialized)]
	 */
	void TableManager::loadAllTables()
	{
		// Check if file has data
		if (_fileManager.size() < sizeof(uint32_t))
			return; // Empty file or doesn't exist
		
		// Read table count
		uint32_t tableCount = 0;
		_fileManager.readAt(0, &tableCount, sizeof(uint32_t));
		
		if (tableCount == 0)
			return;
		
		// Read each table
		size_t offset = sizeof(uint32_t);
		
		for (uint32_t i = 0; i < tableCount; ++i)
		{
			// Read name length
			uint32_t nameLen = 0;
			_fileManager.readAt(offset, &nameLen, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			
			// Read name
			std::vector<char> nameBuffer(nameLen);
			_fileManager.readAt(offset, nameBuffer.data(), nameLen);
			std::string tableName(nameBuffer.begin(), nameBuffer.end());
			offset += nameLen;
			
			// Read data length
			uint32_t dataLen = 0;
			_fileManager.readAt(offset, &dataLen, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			
			// Read data
			std::vector<char> dataBuffer(dataLen);
			_fileManager.readAt(offset, dataBuffer.data(), dataLen);
			offset += dataLen;
			
			// Deserialize and load table
			loadTable(tableName, dataBuffer);
		}
	}

	/*
	 * @brief Save a single table to disk
	 * @param table The table to save
	 */
	void TableManager::saveTable(const Xale::DataStructure::Table& table)
	{
		// Serialize the table
		std::vector<char> data = table.serialize();
		
		// TODO: Write to file manager at appropriate offset
		// This would need a proper file format with table directory/metadata
	}

	/*
	 * @brief Load a single table from disk
	 * @param tableName Name of the table
	 * @param data Serialized table data
	 */
	void TableManager::loadTable(const std::string& tableName, const std::vector<char>& data)
	{
		// Deserialize the table
		auto table = std::make_unique<Xale::DataStructure::Table>(
			Xale::DataStructure::Table::deserialize(data)
		);
		
		_tables[tableName] = std::move(table);
	}
}
