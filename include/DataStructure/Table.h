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
     * @brief Represents a mutable and persistent dataset (table)
     *
     * Provides methods for schema definition, row manipulation, and serialization.
     */
	class Table : public IDataTemplate
    {

        public:
            /**
             * @brief Construct a new Table with the given name
             * @param name Name of the table
             */
            explicit Table(const std::string& name);

            /** @copydoc IDataTemplate::getName */
            const std::string& getName() const override;

            /** @copydoc IDataTemplate::getSchema */
            const std::vector<ColumnDefinition>& getSchema() const override;

            /** @copydoc IDataTemplate::getRows */
            const std::vector<Row>& getRows() const override;

            /** @copydoc IDataTemplate::getRowCount */
            size_t getRowCount() const override;

            /** @copydoc IDataTemplate::getColumnCount */
            size_t getColumnCount() const override;

            /** @copydoc IDataTemplate::isEmpty */
            bool isEmpty() const override;

            /** @copydoc IDataTemplate::isMutable */
            bool isMutable() const override;

            /**
             * @brief Add a new column to the table schema
             * @param column Column definition to add
             */
            void addColumn(const ColumnDefinition& column);

            /**
             * @brief Insert a new row into the table
             * @param row Row to insert
             * @return True if insertion is successful, false otherwise
             */
            bool insertRow(const Row& row);

            /**
             * @brief Update rows matching a condition
             * @param columnName Name of the column to match
             * @param value Value to match in the column
             * @param updates Map of column names to new values
             * @return Number of rows updated
             */
            size_t updateRows(
                const std::string& columnName,
                const FieldValue& value,
                const std::unordered_map<std::string, FieldValue>& updates);

            /**
             * @brief Delete rows matching a condition
             * @param columnName Name of the column to match
             * @param value Value to match in the column
             * @return Number of rows deleted
             */
            size_t deleteRows(const std::string& columnName, const FieldValue& value);

            /**
             * @brief Find rows matching a condition
             * @param columnName Name of the column to match
             * @param value Value to match in the column
             * @return Vector of matching rows
             */
            std::vector<Row> findRows(const std::string& columnName, const FieldValue& value) const;

            /**
             * @brief Serialize the table to a byte vector
             * @return Serialized data
             */
            std::vector<char> serialize() const;

            /**
             * @brief Deserialize a table from a byte vector
             * @param data Serialized data
             * @return Deserialized Table object
             */
            static Table deserialize(const std::vector<char>& data);

        private:
            /** @brief Table name */
            std::string _name;

            /** @brief Table schema (column definitions) */
            std::vector<ColumnDefinition> _schema;

            /** @brief Table rows (data) */
            std::vector<Row> _rows;

            /** @brief Primary index for fast row lookup */
            std::unique_ptr<Xale::DataStructure::BPlusTree<int, Row>> _primaryIndex;
        };
}

#endif // DATA_STRUCTURE_TABLE_H