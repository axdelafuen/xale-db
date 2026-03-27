#include "DataStructure/ResultSet.h"

namespace Xale::DataStructure
{
    ResultSet::ResultSet(const std::string& name)
        :_name(name)
    {}

	const std::string& ResultSet::getName() const
    {
        return _name;
	}

    const std::vector<ColumnDefinition>& ResultSet::getSchema() const
    {
        return _schema;
	}

    const std::vector<Row>& ResultSet::getRows() const
    {
        return _rows;
    }

    size_t ResultSet::getRowCount() const
    {
        return _rows.size();
    }

    size_t ResultSet::getColumnCount() const
    {
        return _schema.size();
    }

    bool ResultSet::isEmpty() const
    {
        return _rows.empty();
	}
    
	bool ResultSet::isMutable() const
    {
		return false;
	}

    void ResultSet::addColumn(const ColumnDefinition& column)
    {
        _schema.push_back(column);
    }

    void ResultSet::addRow(const Row& row)
    {
        _rows.push_back(row);
    }

    const Row& ResultSet::getRow(size_t index) const
    {
        if (index >= _rows.size())
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::Unknown, "Row index out of bounds");
        return _rows[index];
    }
}
