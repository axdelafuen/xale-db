#ifndef ENGINE_QUERY_ENGINE_H
#define ENGINE_QUERY_ENGINE_H

#include "Query/IParser.h"
#include "Query/Statement.h"
#include "Execution/IExecutor.h"
#include "DataStructure/ResultSet.h"

#include <string>
#include <vector>
#include <memory>

namespace Xale::Engine
{
    class QueryEngine
    {
        public:
            QueryEngine() = default;

            /**
             * @brief Constructor of the XaleDB SQL QueryEngine
             * @param parser a SQL string parser
             * @param executor a SQL statement executor
             */
            QueryEngine(
                Xale::Query::IParser* parser, 
                Xale::Execution::IExecutor* executor);
                
            /**
             * @brief Run one or more semicolon-separated SQL queries
             * @param sqlQuery The SQL string query (may contain multiple statements separated by ';')
             * @return True if all executions ran successfully
             */
            bool run(std::string sqlQuery);

            /**
             * @brief Get the last runned query results
             * @return a ResultSet of the last runned query
             */
            std::unique_ptr<Xale::DataStructure::ResultSet> getResults();

            /**
             * @brief Get all query results as a formatted string (multi-query aware)
             * @return a string representing all executed query results
             */
            std::string getResultsToString();
            
        private:
            Xale::Query::IParser* _parser;
            Xale::Execution::IExecutor* _executor;
            std::unique_ptr<Xale::DataStructure::ResultSet> _results;
            Xale::Query::StatementType _lastStatementType;
            std::vector<std::string> _multiResponses; ///< Accumulated responses for multi-query

            /**
             * @brief Split input by semicolons, respecting string literals
             * @param input Raw SQL input string
             * @return Vector of individual query strings
             */
            std::vector<std::string> splitQueries(const std::string& input) const;

            /**
             * @brief Format the current _results based on _lastStatementType
             * @return Formatted result string
             */
            std::string formatCurrentResult();

            /**
             * @brief Format the results of a SELECT statement as a string
             * @return a string representing the SELECT statement results
             */
            std::string formatSelectResult();

            /**
             * @brief Format the results of an INSERT statement as a string
             * @return a string representing the INSERT statement results
             */
            std::string formatInsertResult();
            
            /**
             * @brief Format the results of an UPDATE statement as a string
             * @return a string representing the UPDATE statement results
             */
            std::string formatUpdateResult();

            /**
             * @brief Format the results of a DELETE statement as a string
             * @return a string representing the DELETE statement results
             */
            std::string formatDeleteResult();

            /**
             * @brief Format the results of a CREATE statement as a string
             * @return a string representing the CREATE statement results
             */
            std::string formatCreateResult();

            /**
             * @brief Format the results of a DROP statement as a string
             * @return a string representing the DROP statement results
             */
            std::string formatDropResult();
    };
}

#endif // ENGINE_QUERY_ENGINE_H
