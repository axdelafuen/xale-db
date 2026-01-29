#include "DataStructure/ResultSet.h"

namespace Xale::DataStructure
{
    /*
     * @brief Constructor
     * @param name Result set name (optional)
     */
    ResultSet::ResultSet(const std::string& name)
        :_name(name)
    {}

    /*
     * @brief Get result set name
     * @return Name
     */
	const std::string& ResultSet::getName() const
    {
        return _name;
	}

    /*
     * @brief Get schema
     * @return Vector of column definitions
     */
    const std::vector<ColumnDefinition>& ResultSet::getSchema() const
    {
        return _schema;
	}

    /*
     * @brief Get all rows
     * @return Vector of rows
     */
    const std::vector<Row>& ResultSet::getRows() const
    {
        return _rows;
    }

    /*
     * @brief Get row count
     * @return Number of rows
     */
    size_t ResultSet::getRowCount() const
    {
        return _rows.size();
    }

    /*
     * @brief Get column count
     * @return Number of columns
     */
    size_t ResultSet::getColumnCount() const
    {
        return _columns.size();
    }

    /*
     * @brief Check if empty
     * @return True if empty
     */
    bool ResultSet::isEmpty() const
    {
        return _rows.empty();
	}
    
    /*
     * @brief Check if mutable
     * @return Always false for ResultSet
     */
	bool ResultSet::isMutable() const
    {
		return false;
	}

    /*
     * @brief Add column definition (used by executor)
     * @param column Column definition
     */
    void ResultSet::addColumn(const std::string& name)
    {
        _columns.push_back(name);
    }

    /*
     * @brief Add a row (used by executor)
     * @param row Row to add
     */
    void ResultSet::addRow(const Row& row)
    {
        _rows.push_back(row);
    }

    /*
     * @brief Get a specific row
     * @param index Row index
     * @return Reference to the row
     * @throws DbException if index out of bounds
     */
    const Row& ResultSet::getRow(size_t index) const
    {
        if (index >= _rows.size())
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::Unknown, "Row index out of bounds");
        return _rows[index];
    }
}
