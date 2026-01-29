#include "DataStructure/Table.h"

namespace Xale::DataStructure
{
	/*
	 * @brief Constructor
	 * @param name Table name
	 */
	Table::Table(const std::string& name)
		:_name(name)
	{}

	/*
	 * @brief Get table name
	 * @return Table name
	 */
	const std::string& Table::getName() const
	{
		return _name;
	}

	/*
	 * @brief Get schema
	 * @return Vector of column definitions
	 */
	const std::vector<ColumnDefinition>& Table::getSchema() const
	{
		return _schema;
	}

	/*
	 * @brief Get all rows
	 * @return Vector of rows
	 */
	const std::vector<Row>& Table::getRows() const
	{
		return _rows;
	}

	/*
	 * @brief Get row count
	 * @return Number of rows
	 */
	size_t Table::getRowCount() const
	{
		return _rows.size();
	}

	/*
	 * @brief Get column count
	 * @return Number of columns
	 */
	size_t Table::getColumnCount() const
	{
		return _schema.size();
	}

	/*
	 * @brief Check if empty
	 * @return True if empty
	 */
	bool Table::isEmpty() const
	{
		return _rows.empty();
	}

	/*
	 * @brief Check if mutable
	 * @return Always true for Table
	 */
	bool Table::isMutable() const
	{
		return true;
	}

	/*
	 * @brief Add column definition
	 * @param column Column definition
	 */
	bool Table::addColumn(const ColumnDefinition& column)
	{
		_schema.push_back(column);

		return true;
	}

	/*
	 * @brief Insert a row
	 * @param row Row to insert
	 * @return True if successful
	 * @throws DbException if validation fails
	 */
	bool Table::insertRow(const Row& row)
	{
		if (row.size() != _schema.size())
			return false;
		
		_rows.push_back(row);

		return true;
	}

	/*
	 * @brief Update rows matching condition
	 * @param columnName Column to filter
	 * @param value Value to match
	 * @param updates Map of column names to new values
	 * @return Number of rows updated
	 */
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
			if (row[columnIndex] == value)
			{
				for (const auto& [updateColumn, newValue] : updates)
				{
					for (size_t j = 0; j < _schema.size(); ++j)
					{
						if (_schema[j].name == updateColumn)
						{
							row[j] = newValue;
							break;
						}
					}
				}
				++updatedCount;
			}
		}

		return updatedCount;
	}

	/*
	 * @brief Delete rows matching condition
	 * @param columnName Column to filter
	 * @param value Value to match
	 * @return Number of rows deleted
	 */
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
			if ((*it)[columnIndex] == value)
			{
				it = _rows.erase(it);
				++deletedCount;
			}
			else
				++it;
		}

		return deletedCount;
	}

	/*
	 * @brief Find rows by condition
	 * @param columnName Column name to filter
	 * @param value Value to match
	 * @return Vector of matching rows
	 */
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
			if (row[columnIndex] == value)
				result.push_back(row);
		}

		return result;
	}
}
