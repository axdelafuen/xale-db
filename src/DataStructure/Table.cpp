#include "DataStructure/Table.h"

namespace Xale::DataStructure
{
	Table::Table(const std::string& name)
		:_name(name)
	{}

	const std::string& Table::getName() const
	{
		return _name;
	}

	const std::vector<ColumnDefinition>& Table::getSchema() const
	{
		return _schema;
	}

	const std::vector<Row>& Table::getRows() const
	{
		return _rows;
	}

	size_t Table::getRowCount() const
	{
		return _rows.size();
	}

	size_t Table::getColumnCount() const
	{
		return _schema.size();
	}

	bool Table::isEmpty() const
	{
		return _rows.empty();
	}

	bool Table::isMutable() const
	{
		return true;
	}

	void Table::addColumn(const ColumnDefinition& column)
	{
		_schema.push_back(column);
	}

	bool Table::insertRow(const Row& row)
	{
		if (row.fields.size() != _schema.size())
			return false;
		
		_rows.push_back(row);

		return true;
	}

	size_t Table::updateRows(
		const std::string& columnName,
		const FieldValue& value,
		const std::unordered_map<std::string, FieldValue>& updates)
	{
		size_t updatedCount = 0;
		int columnIndex = -1;

		for (size_t i = 0; i < _schema.size(); ++i)
		{
			if (_schema[i].name == columnName)
			{
				columnIndex = static_cast<int>(i);
				break;
			}
		}

		if (columnIndex == -1)
			return 0;

		for (auto& row : _rows)
		{
			if (row.fields[columnIndex].value == value)
			{
				for (const auto& [updateColumn, newValue] : updates)
				{
					for (size_t j = 0; j < _schema.size(); ++j)
					{
						if (_schema[j].name == updateColumn)
						{
							row.fields[j].value = newValue;
							break;
						}
					}
				}
				++updatedCount;
			}
		}

		return updatedCount;
	}

	size_t Table::deleteRows(const std::string& columnName, const FieldValue& value)
	{
		size_t deletedCount = 0;
		int columnIndex = -1;

		for (size_t i = 0; i < _schema.size(); ++i)
		{
			if (_schema[i].name == columnName)
			{
				columnIndex = static_cast<int>(i);
				break;
			}
		}

		if (columnIndex == -1)
			return 0;
		auto it = _rows.begin();

		while (it != _rows.end())
		{
			if ((*it).fields[columnIndex].value == value)
			{
				it = _rows.erase(it);
				++deletedCount;
			}
			else
				++it;
		}

		return deletedCount;
	}

	std::vector<Row> Table::findRows(const std::string& columnName, const FieldValue& value) const
	{
		std::vector<Row> result;
		int columnIndex = -1;

		for (size_t i = 0; i < _schema.size(); ++i)
		{
			if (_schema[i].name == columnName)
			{
				columnIndex = static_cast<int>(i);
				break;
			}
		}

		if (columnIndex == -1)
			return result;

		for (const auto& row : _rows)
		{
			if (row.fields[columnIndex].value == value)
				result.push_back(row);
		}

		return result;
	}

	std::vector<char> Table::serialize() const
	{
		std::vector<char> buffer;
		
		// Helper to write data to buffer
		auto writeString = [&](const std::string& str) {
			uint32_t len = str.length();
			buffer.insert(buffer.end(), reinterpret_cast<const char*>(&len), reinterpret_cast<const char*>(&len) + sizeof(len));
			buffer.insert(buffer.end(), str.begin(), str.end());
		};
		
		auto writeInt = [&](int32_t val) {
			buffer.insert(buffer.end(), reinterpret_cast<const char*>(&val), reinterpret_cast<const char*>(&val) + sizeof(val));
		};
		
		auto writeDouble = [&](double val) {
			buffer.insert(buffer.end(), reinterpret_cast<const char*>(&val), reinterpret_cast<const char*>(&val) + sizeof(val));
		};
		
		// Write table name
		writeString(_name);
		
		// Write schema
		uint32_t schemaSize = _schema.size();
		buffer.insert(buffer.end(), reinterpret_cast<const char*>(&schemaSize), reinterpret_cast<const char*>(&schemaSize) + sizeof(schemaSize));
		
		for (const auto& col : _schema)
		{
			writeString(col.name);
			int32_t type = static_cast<int32_t>(col.type);
			writeInt(type);
			char isPK = col.isPrimaryKey ? 1 : 0;
			buffer.push_back(isPK);
			char isNull = col.isNullable ? 1 : 0;
			buffer.push_back(isNull);
		}
		
		// Write rows
		uint32_t rowCount = _rows.size();
		buffer.insert(buffer.end(), reinterpret_cast<const char*>(&rowCount), reinterpret_cast<const char*>(&rowCount) + sizeof(rowCount));
		
		for (const auto& row : _rows)
		{
			for (const auto& field : row.fields)
			{
				std::visit([&](auto&& arg) {
					using T = std::decay_t<decltype(arg)>;
					if constexpr (std::is_same_v<T, int>)
					{
						writeInt(static_cast<int32_t>(FieldType::Integer));
						writeInt(arg);
					}
					else if constexpr (std::is_same_v<T, double>)
					{
						writeInt(static_cast<int32_t>(FieldType::Float));
						writeDouble(arg);
					}
					else if constexpr (std::is_same_v<T, std::string>)
					{
						writeInt(static_cast<int32_t>(FieldType::String));
						writeString(arg);
					}
					else if constexpr (std::is_same_v<T, std::monostate>)
					{
						writeInt(static_cast<int32_t>(FieldType::Null));
						// NULL value, no data to write
					}
				}, field.value);
			}
		}
		
		return buffer;
	}

	Table Table::deserialize(const std::vector<char>& data)
	{
		size_t offset = 0;
		
		// Helper to read data from buffer
		auto readString = [&]() -> std::string {
			uint32_t len;
			std::memcpy(&len, &data[offset], sizeof(len));
			offset += sizeof(len);
			std::string str(data.begin() + offset, data.begin() + offset + len);
			offset += len;
			return str;
		};
		
		auto readInt = [&]() -> int32_t {
			int32_t val;
			std::memcpy(&val, &data[offset], sizeof(val));
			offset += sizeof(val);
			return val;
		};
		
		auto readDouble = [&]() -> double {
			double val;
			std::memcpy(&val, &data[offset], sizeof(val));
			offset += sizeof(val);
			return val;
		};
		
		// Read table name
		std::string tableName = readString();
		Table table(tableName);
		
		// Read schema
		uint32_t schemaSize;
		std::memcpy(&schemaSize, &data[offset], sizeof(schemaSize));
		offset += sizeof(schemaSize);
		
		for (uint32_t i = 0; i < schemaSize; ++i)
		{
			std::string colName = readString();
			int32_t typeInt = readInt();
			FieldType type = static_cast<FieldType>(typeInt);
			bool isPK = data[offset++] == 1;
			bool isNull = data[offset++] == 1;
			
			table.addColumn(ColumnDefinition(colName, type, isPK, isNull));
		}
		
		// Read rows
		uint32_t rowCount;
		std::memcpy(&rowCount, &data[offset], sizeof(rowCount));
		offset += sizeof(rowCount);
		
		const auto& schema = table.getSchema();
		
		for (uint32_t i = 0; i < rowCount; ++i)
		{
			Row row;
			for (size_t colIdx = 0; colIdx < schema.size(); ++colIdx)
			{
				const auto& col = schema[colIdx];
				int32_t ftypeInt = readInt();
				FieldType ftype = static_cast<FieldType>(ftypeInt);
				
				FieldValue value;
				if (ftype == FieldType::Integer)
				{
					value = readInt();
				}
				else if (ftype == FieldType::Float)
				{
					value = readDouble();
				}
				else if (ftype == FieldType::String)
				{
					value = readString();
				}
				else // Null
				{
					value = std::monostate{};
				}
				
				row.fields.push_back(Field(col.name, ftype, value));
			}
			table.insertRow(row);
		}
		
		return table;
	}
}
