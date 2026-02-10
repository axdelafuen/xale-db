#ifndef DATA_STRUCTURE_TABLE_H
#define DATA_STRUCTURE_TABLE_H

#include "DataStructure/IDataTemplate.h"
#include "DataStructure/BPlusTree.h"

#include <unordered_map>
#include <cstdint>
#include <cstring>

namespace Xale::DataStructure
{
    /**
     * @brief Mutable and persistent dataset
     */
	class Table : public IDataTemplate
    {
        public:
            explicit Table(const std::string& name);
            const std::string& getName() const override;
            const std::vector<ColumnDefinition>& getSchema() const override;
            const std::vector<Row>& getRows() const override;
            size_t getRowCount() const override;
            size_t getColumnCount() const override;
            bool isEmpty() const override;
            bool isMutable() const override;
            void addColumn(const ColumnDefinition& column);
            bool insertRow(const Row& row);
            size_t updateRows(
                const std::string& columnName,
                const FieldValue& value,
                const std::unordered_map<std::string, FieldValue>& updates);
            size_t deleteRows(const std::string& columnName, const FieldValue& value);
            std::vector<Row> findRows(const std::string& columnName, const FieldValue& value) const;

            std::vector<char> serialize() const;
            static Table deserialize(const std::vector<char>& data);

        private:
            std::string _name;
            std::vector<ColumnDefinition> _schema;
            std::vector<Row> _rows;
            std::unique_ptr<Xale::DataStructure::BPlusTree<int, Row>> _primaryIndex;
        };
}

#endif // DATA_STRUCTURE_TABLE_H