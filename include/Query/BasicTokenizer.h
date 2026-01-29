#ifndef QUERY_BASIC_TOKENIZER_H
#define QUERY_BASIC_TOKENIZER_H

#include "Query/ITokenizer.h"

namespace Xale::Query
{
    // data definition keywords
    const std::string create_token = "CREATE";
    const std::string alter_token = "ALTER";
    const std::string drop_token = "DROP";

    // data manipulation keywords
    const std::string select_token = "SELECT";
    const std::string inser_token = "INSERT";
    const std::string update_token = "UPDATE";
    const std::string delete_token = "DELETE";

    // query keywords
    const std::string from_token = "FROM";
    const std::string where_token = "WHERE";

    // join keywords
    const std::string join_token = "JOIN";
    const std::string left_token = "LEFT";
    const std::string right_token = "RIGHT";

    // logical keywords
    const std::string and_token = "AND";
    const std::string or_token = "OR";
    const std::string not_token = "NOT";
    const std::string equal_token = "=";
    const std::string diff_token = "!=";

    class BasicTokenizer : ITokenizer
    {
        public:
            std::vector<std::string> tokenize(
                    const std::string& input) override;
    }
}

#endif // BASIC_QUERY_TOKENIZER_H
