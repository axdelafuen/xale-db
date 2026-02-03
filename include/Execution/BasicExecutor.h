#ifndef EXECUTION_BASIC_EXECUTOR_H
#define EXECUTION_BASIC_EXECUTOR_H

#include "Execution/IExecutor.h"
#include "Core/ExceptionHandler.h"
#include "Execution/TableManager.h"
#include "Query/Statement.h"
#include "DataStructure/DataTypes.h"

#include <unordered_map>

namespace Xale::Execution
{
	class BasicExecutor : public IExecutor
    {
        public:
            BasicExecutor(TableManager& tableManager);
            std::unique_ptr<Xale::DataStructure::ResultSet> execute(Xale::Query::Statement* statement) override;
        private:
            TableManager& _tableManager;

            std::unique_ptr<Xale::DataStructure::ResultSet> executeSelect(Xale::Query::SelectStatement* stmt);
            std::unique_ptr<Xale::DataStructure::ResultSet> executeInsert(Xale::Query::InsertStatement* stmt);
            std::unique_ptr<Xale::DataStructure::ResultSet> executeUpdate(Xale::Query::UpdateStatement* stmt);
            std::unique_ptr<Xale::DataStructure::ResultSet> executeDelete(Xale::Query::DeleteStatement* stmt);
            std::unique_ptr<Xale::DataStructure::ResultSet> executeCreate(Xale::Query::CreateStatement* stmt);
            std::unique_ptr<Xale::DataStructure::ResultSet> executeDrop(Xale::Query::DropStatement* stmt);
            std::unique_ptr<Xale::DataStructure::ResultSet> executeList(Xale::Query::ListStatement* stmt);

            Xale::DataStructure::FieldValue evaluateExpression(const Xale::Query::Expression& expr);
            bool evaluateCondition(const Xale::DataStructure::Row& row, const Xale::Query::WhereClause* where);
    };
}

#endif // EXECUTION_BASIC_EXECUTOR_H
