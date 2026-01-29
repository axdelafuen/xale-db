#ifndef DATA_STRUCTURE_I_DATA_TEMPLATE_H
#define DATA_STRUCTURE_I_DATA_TEMPLATE_H

#include "DataStructure/DataTypes.h"

#include <string>
#include <vector>
#include <memory>
#include <variant>

namespace Xale::DataStructure
{
    /*
     * @brief Abstract base class for data collections
     * Base for Table, ResultSet, View, Index, etc.
     */
    class IDataTemplate
    {
        public:
            virtual const std::string& getName() const = 0;
            virtual const std::vector<ColumnDefinition>& getSchema() const = 0;
            virtual const std::vector<Row>& getRows() const = 0;
            virtual size_t getRowCount() const = 0;
            virtual size_t getColumnCount() const = 0;
            virtual bool isEmpty() const = 0;
            virtual bool isMutable() const = 0;
    };
}

#endif // DATA_STRUCTURE_I_DATA_TEMPLATE_H