#ifndef DATA_STRUCTURE_DATA_TYPES_H
#define DATA_STRUCTURE_DATA_TYPES_H

#include <string>
#include <vector>
#include <variant>

namespace Xale::DataStructure
{
    /*
     * @brief Type of data in a field
     */
    enum class FieldType
    {
        Integer,
        Float,
        String,
        Null
    };

    /*
     * @brief Variant type for field values
     */
    using FieldValue = std::variant<int, double, std::string, std::monostate>;

    /*
     * @brief A single field in a row
     */
    struct Field
    {
        std::string name;
        FieldType type;
        FieldValue value;

        Field() : name(""), type(FieldType::Null), value(std::monostate{}) {}
        Field(std::string n, FieldType t, FieldValue v)
            : name(std::move(n)), type(t), value(std::move(v)) {
        }
    };

    /*
     * @brief A row of data
     */
    struct Row
    {
        std::vector<Field> fields;

        Row() = default;
        explicit Row(std::vector<Field> f) : fields(std::move(f)) {}
    };

    /*
     * @brief Column definition
     */
    struct ColumnDefinition
    {
        std::string name;
        FieldType type;
        bool isPrimaryKey;
        bool isNullable;

        ColumnDefinition(std::string n, FieldType t, bool pk = false, bool nullable = true)
            : name(std::move(n)), type(t), isPrimaryKey(pk), isNullable(nullable) {
        }
    };
}

#endif // DATA_STRUCTURE_DATA_TYPES_H