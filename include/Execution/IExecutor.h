#ifndef EXECUTION_I_EXECUTOR_H
#define EXECUTION_I_EXECUTOR_H

#include "Query/Statement.h"
#include "DataStructure/ResultSet.h"

#include <memory>

namespace Xale::Execution
{
    /**
     * @brief Interface for executing SQL statements and returning results.
     */
    class IExecutor
    {
        public:
            virtual ~IExecutor() = default;
            /**
             * @brief Executes a given SQL statement and returns the result set.
             * @param statement Pointer to the SQL statement to be executed.
             * @return A unique pointer to the ResultSet containing the results of the execution.
             */
            virtual std::unique_ptr<Xale::DataStructure::ResultSet> execute(Xale::Query::Statement* statement) = 0;
    };
}

#endif // EXECUTION_I_EXECUTOR_H