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
    /**
     * @brief Basic implementation of the IExecutor interface, responsible for executing SQL statements and returning results.
     */
	class BasicExecutor : public IExecutor
    {
        public:
            /**
             * @brief Constructs a BasicExecutor with a reference to the TableManager.
             * @param tableManager Reference to the TableManager for managing database tables.
             */
            BasicExecutor(TableManager& tableManager);

            /**
             * @copydoc IExecutor::execute
             * @param statement Pointer to the SQL statement to be executed.
             * @return A unique pointer to the ResultSet containing the results of the execution.
             */
            std::unique_ptr<Xale::DataStructure::ResultSet> execute(Xale::Query::Statement* statement) override;
        
        private:
            TableManager& _tableManager;

            /**
             * @brief Executes a SELECT statement and returns the result set.
             * @param stmt Pointer to the SELECT statement to be executed.
             * @return A unique pointer to the ResultSet containing the results of the SELECT execution.
             */
            std::unique_ptr<Xale::DataStructure::ResultSet> executeSelect(Xale::Query::SelectStatement* stmt);
            
            /**
             * @brief Executes an INSERT statement and returns the result set.
             * @param stmt Pointer to the INSERT statement to be executed.
             * @return A unique pointer to the ResultSet containing the results of the INSERT execution.
             */
            std::unique_ptr<Xale::DataStructure::ResultSet> executeInsert(Xale::Query::InsertStatement* stmt);
            
            /**
             * @brief Executes an UPDATE statement and returns the result set.
             * @param stmt Pointer to the UPDATE statement to be executed.
             * @return A unique pointer to the ResultSet containing the results of the UPDATE execution.
             */
            std::unique_ptr<Xale::DataStructure::ResultSet> executeUpdate(Xale::Query::UpdateStatement* stmt);
            
            /**
             * @brief Executes a DELETE statement and returns the result set.
             * @param stmt Pointer to the DELETE statement to be executed.
             * @return A unique pointer to the ResultSet containing the results of the DELETE execution.
             */
            std::unique_ptr<Xale::DataStructure::ResultSet> executeDelete(Xale::Query::DeleteStatement* stmt);
            
            /**
            * @brief Executes a CREATE statement and returns the result set.
            * @param stmt Pointer to the CREATE statement to be executed.
            * @return A unique pointer to the ResultSet containing the results of the CREATE execution.
            */
            std::unique_ptr<Xale::DataStructure::ResultSet> executeCreate(Xale::Query::CreateStatement* stmt);
            
            /**
             * @brief Executes a DROP statement and returns the result set.
             * @param stmt Pointer to the DROP statement to be executed.
             * @return A unique pointer to the ResultSet containing the results of the DROP execution.
             */
            std::unique_ptr<Xale::DataStructure::ResultSet> executeDrop(Xale::Query::DropStatement* stmt);
            
            /**
             * @brief Executes a LIST statement and returns the result set.
             * @param stmt Pointer to the LIST statement to be executed.
             * @return A unique pointer to the ResultSet containing the results of the LIST execution.
             */
            std::unique_ptr<Xale::DataStructure::ResultSet> executeList(Xale::Query::ListStatement* stmt);

            /**
             * @brief Evaluates an expression and returns its value.
             * @param expr The expression to be evaluated.
             * @return The value of the evaluated expression as a FieldValue.
             */
            Xale::DataStructure::FieldValue evaluateExpression(const Xale::Query::Expression& expr);
            
            /**
             * @brief Evaluates a condition on a given row.
             * @param row The row to be evaluated.
             * @param where The WHERE clause containing the condition.
             * @return True if the condition is met, false otherwise.
             */
            bool evaluateCondition(const Xale::DataStructure::Row& row, const Xale::Query::WhereClause* where);
    };
}

#endif // EXECUTION_BASIC_EXECUTOR_H
