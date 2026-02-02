#ifndef ENGINE_QUERY_ENGINE_H
#define ENGINE_QUERY_ENGINE_H

#include "Query/IParser.h"
#include "Query/Statement.h"
#include "Execution/IExecutor.h"
#include "DataStructure/ResultSet.h"

#include <string>
#include <memory>

namespace Xale::Engine
{
    class QueryEngine
    {
        public:
            QueryEngine() = default;
            QueryEngine(
                Xale::Query::IParser* parser, 
                Xale::Execution::IExecutor* executor);
            bool run(std::string sqlQuery);
            std::unique_ptr<Xale::DataStructure::ResultSet> getResults();
            std::string getResultsToString();
        private:
            Xale::Query::IParser* _parser;
            Xale::Execution::IExecutor* _executor;
            std::unique_ptr<Xale::DataStructure::ResultSet> _results;
            Xale::Query::StatementType _lastStatementType;

            std::string formatSelectResult();
            std::string formatInsertResult();
            std::string formatUpdateResult();
            std::string formatDeleteResult();
            std::string formatCreateResult();
            std::string formatDropResult();
    };
}

#endif // ENGINE_QUERY_ENGINE_H
