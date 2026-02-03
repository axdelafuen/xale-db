#include "Engine/QueryEngine.h"

namespace Xale::Engine
{
    /*
     * @brief Constructor of the XaleDB SQL QueryEngine
     * @param parser a SQL string parser
     * @param executor a SQL statement executor
     */
    QueryEngine::QueryEngine(
            Xale::Query::IParser* parser, 
            Xale::Execution::IExecutor* executor) :
        _parser(parser),
        _executor(executor),
        _results(nullptr),
        _lastStatementType(Xale::Query::StatementType::Unknown)
    {}
   
    /*
     * @brief Run the given string query
     * @param sqlQuery The SQL string query
     * @return True if the execution run well, False otherwise
     */
    bool QueryEngine::run(std::string sqlQuery)
    {
        auto parsedStmt = _parser->parse(sqlQuery);
        _lastStatementType = parsedStmt->type;
        _results = _executor->execute(parsedStmt.get());

        return true;
    }

    /*
     * @brief Get the last runned query results
     * @return a ResultSet of the last runned query
     */
    std::unique_ptr<Xale::DataStructure::ResultSet> QueryEngine::getResults()
    {
        if (_results != nullptr)
            return std::move(_results);
        else
            return nullptr;
    }

    /*
     * @brief Get the last runned query results as a formatted string
     * @return a string representing the last runned query results
     */
    std::string QueryEngine::getResultsToString()
    {
        if (_results == nullptr)
            return "No query runned!";

        switch (_lastStatementType)
        {
            case Xale::Query::StatementType::Select:
                return formatSelectResult();
            case Xale::Query::StatementType::Insert:
                return formatInsertResult();
            case Xale::Query::StatementType::Update:
                return formatUpdateResult();
            case Xale::Query::StatementType::Delete:
                return formatDeleteResult();
            case Xale::Query::StatementType::Create:
                return formatCreateResult();
            case Xale::Query::StatementType::Drop:
                return formatDropResult();
            case Xale::Query::StatementType::List:
                return formatSelectResult();
            default:
                return "Query executed";
        }
    }

    std::string QueryEngine::formatSelectResult()
    {
        const auto& schema = _results->getSchema();
        const auto& rows = _results->getRows();

        if (schema.empty() || rows.empty())
            return "Empty set";

        auto valueToString = [](const Xale::DataStructure::Field& field) -> std::string {
            if (field.type == Xale::DataStructure::FieldType::Integer)
            {
                return std::visit([](auto&& arg) -> std::string {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, int>)
                        return std::to_string(arg);
                    else if constexpr (std::is_same_v<T, double>)
                        return std::to_string(static_cast<int>(arg));
                    else if constexpr (std::is_same_v<T, std::monostate>)
                        return "NULL";
                    else
                        return "0";
                }, field.value);
            }
            
            return std::visit([](auto&& arg) -> std::string {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::monostate>)
                    return "NULL";
                else if constexpr (std::is_same_v<T, int>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, double>)
                {
                    std::string str = std::to_string(arg);
                    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                    if (str.back() == '.')
                        str.pop_back();
                    return str;
                }
                else if constexpr (std::is_same_v<T, std::string>)
                    return arg;
                else
                    return "UNKNOWN";
            }, field.value);
        };

        std::vector<size_t> columnWidths;
        for (const auto& col : schema)
        {
            size_t maxWidth = col.name.length();
            
            for (const auto& row : rows)
            {
                for (const auto& field : row.fields)
                {
                    if (field.name == col.name)
                    {
                        std::string valueStr = valueToString(field);
                        maxWidth = std::max(maxWidth, valueStr.length());
                        break;
                    }
                }
            }
            
            columnWidths.push_back(maxWidth);
        }

        auto buildSeparator = [&]() {
            std::string separator = "+";
            for (size_t width : columnWidths)
                separator += std::string(width + 2, '-') + "+";
            return separator + "\n";
        };

        std::string result;
        std::string separator = buildSeparator();

        result += separator;
        result += "|";
        for (size_t i = 0; i < schema.size(); ++i)
        {
            std::string header = schema[i].name;
            size_t padding = columnWidths[i] - header.length();
            result += " " + header + std::string(padding, ' ') + " |";
        }
        result += "\n";
        result += separator;

        for (const auto& row : rows)
        {
            result += "|";
            for (size_t i = 0; i < schema.size(); ++i)
            {
                std::string valueStr;
                
                for (const auto& field : row.fields)
                {
                    if (field.name == schema[i].name)
                    {
                        valueStr = valueToString(field);
                        break;
                    }
                }
                
                size_t padding = columnWidths[i] - valueStr.length();
                result += " " + valueStr + std::string(padding, ' ') + " |";
            }
            result += "\n";
        }

        result += separator;
        result += std::to_string(rows.size()) + " row";
        if (rows.size() != 1)
            result += "s";
        result += " in set";

        return result;
    }

    std::string QueryEngine::formatInsertResult()
    {
        return "Query OK, 1 row inserted";
    }

    std::string QueryEngine::formatUpdateResult()
    {
        size_t rowCount = _results->getRowCount();
        std::string result = "Query OK";
        if (rowCount > 0)
            result += ", " + std::to_string(rowCount) + " row" + (rowCount != 1 ? "s" : "") + " updated";
        else
            result += ", 0 rows updated";
        return result;
    }

    std::string QueryEngine::formatDeleteResult()
    {
        size_t rowCount = _results->getRowCount();
        std::string result = "Query OK";
        if (rowCount > 0)
            result += ", " + std::to_string(rowCount) + " row" + (rowCount != 1 ? "s" : "") + " deleted";
        else
            result += ", 0 rows deleted";
        return result;
    }

    std::string QueryEngine::formatCreateResult()
    {
        return "Query OK, table created";
    }

    std::string QueryEngine::formatDropResult()
    {
        return "Query OK, table dropped";
    }
}
