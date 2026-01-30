#ifndef DATA_STRUCTURE_RESULT_SET_H
#define DATA_STRUCTURE_RESULT_SET_H

#include "DataStructure/IDataTemplate.h"
#include "Core/ExceptionHandler.h"

namespace Xale::DataStructure
{
    /*
     * @brief Read-only temporary dataset
     */
    class ResultSet : public IDataTemplate
    {
        public:
            explicit ResultSet(const std::string& name = "result");
            const std::string& getName() const override;
            const std::vector<ColumnDefinition>& getSchema() const override;
            const std::vector<Row>& getRows() const override;
            size_t getRowCount() const override;
            size_t getColumnCount() const override;
            bool isEmpty() const override;
            bool isMutable() const override;
            void addColumn(const ColumnDefinition& column);
            void addRow(const Row& row);
            const Row& getRow(size_t index) const;

        private:
            std::string _name;
            std::vector<ColumnDefinition> _schema;
            std::vector<Row> _rows;
    };
}

#endif // EXECUTION_RESULT_SET_H