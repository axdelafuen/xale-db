#ifndef DATA_STRUCTURE_RESULT_SET_H
#define DATA_STRUCTURE_RESULT_SET_H

#include "DataStructure/IDataTemplate.h"
#include "Core/ExceptionHandler.h"

namespace Xale::DataStructure
{
    /**
     * @brief Read-only temporary dataset
     */
    class ResultSet : public IDataTemplate
    {
        public:
            /**
             * @brief Constructs a ResultSet with an optional name
             * @param name Name of the ResultSet
             */
            explicit ResultSet(const std::string& name = "result");

            /**
             * @brief Implements IDataTemplate interface
             * @return The name of the ResultSet
             */
            const std::string& getName() const override;

            /**
             * @brief Implements IDataTemplate interface
             * @return The schema of the ResultSet
             */
            const std::vector<ColumnDefinition>& getSchema() const override;

            /**
             * @brief Implements IDataTemplate interface
             * @return The rows of the ResultSet
             */
            const std::vector<Row>& getRows() const override;
            
            /**
             * @brief Implements IDataTemplate interface
             * @return The number of rows in the ResultSet
             */
            size_t getRowCount() const override;
            
            /**
             * @brief Implements IDataTemplate interface
             * @return The number of columns in the ResultSet
             */
            size_t getColumnCount() const override;
            
            /**
             * @brief Implements IDataTemplate interface
             * @return True if the ResultSet is empty, false otherwise
             */
            bool isEmpty() const override;
            
            /**
             * @brief Implements IDataTemplate interface
             * @return True if the ResultSet is mutable, false otherwise
             */
            bool isMutable() const override;
            
            /**
             * @brief Adds a column to the ResultSet
             * @param column Column definition to add
             */
            void addColumn(const ColumnDefinition& column);
            
            /**
             * @brief Adds a row to the ResultSet
             * @param row Row to add
             */
            void addRow(const Row& row);
            
            /**
             * @brief Gets a row by index
             * @param index Index of the row
             * @return The row at the specified index
             */ 
            const Row& getRow(size_t index) const;

        private:
            std::string _name;
            std::vector<ColumnDefinition> _schema;
            std::vector<Row> _rows;
    };
}

#endif // EXECUTION_RESULT_SET_H
