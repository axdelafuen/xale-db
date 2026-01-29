#ifndef EXECUTION_I_EXECUTOR_H
#define EXECUTION_I_EXECUTOR_H

#include "Query/Statement.h"
#include "DataStructure/ResultSet.h"

#include <memory>

namespace Xale::Execution
{
    class IExecutor
    {
        public:
            virtual ~IExecutor() = default;
            virtual std::unique_ptr<Xale::DataStructure::ResultSet> execute(Xale::Query::Statement* statement) = 0;
    };
}

#endif // EXECUTION_I_EXECUTOR_H