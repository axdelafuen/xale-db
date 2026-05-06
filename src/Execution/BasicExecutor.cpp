#include "Execution/BasicExecutor.h"

namespace Xale::Execution
{
	BasicExecutor::BasicExecutor(TableManager& tableManager)
		: _tableManager(tableManager)
	{}

	std::unique_ptr<Xale::DataStructure::ResultSet> BasicExecutor::execute(Xale::Query::Statement* statement)
	{
		switch (statement->type)
		{
			case Xale::Query::StatementType::Select: return executeSelect(static_cast<Xale::Query::SelectStatement*>(statement));
			case Xale::Query::StatementType::Insert: return executeInsert(static_cast<Xale::Query::InsertStatement*>(statement));
			case Xale::Query::StatementType::Update: return executeUpdate(static_cast<Xale::Query::UpdateStatement*>(statement));
			case Xale::Query::StatementType::Delete: return executeDelete(static_cast<Xale::Query::DeleteStatement*>(statement));
			case Xale::Query::StatementType::Create: return executeCreate(static_cast<Xale::Query::CreateStatement*>(statement));
			case Xale::Query::StatementType::Drop: return executeDrop(static_cast<Xale::Query::DropStatement*>(statement));
            case Xale::Query::StatementType::List: return executeList(static_cast<Xale::Query::ListStatement*>(statement));
            default: THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ExecutionError, "Unsupported statement type");
		}
	}

	std::unique_ptr<Xale::DataStructure::ResultSet> BasicExecutor::executeSelect(Xale::Query::SelectStatement* stmt)
	{
		auto table = _tableManager.getTable(stmt->tableName);
		if (!table) 
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::Unknown, "Table does not exist");

		auto resultSet = std::make_unique<Xale::DataStructure::ResultSet>();

		// Helper: extract column name from "table.column" or plain "column"
		auto colNamePart = [](const std::string& s) -> std::string {
			auto dot = s.rfind('.');
			return dot != std::string::npos ? s.substr(dot + 1) : s;
		};

		bool isWildcard = stmt->columns.size() == 1 &&
		                  stmt->columns[0].type == Xale::Query::ExpressionType::Wildcard;

		if (stmt->joins.empty())
		{
			// Original single-table path
			if (isWildcard)
				for (const auto& col : table->getSchema()) resultSet->addColumn(col);
			else
				for (const auto& col : stmt->columns)
					resultSet->addColumn(Xale::DataStructure::ColumnDefinition(colNamePart(col.value), Xale::DataStructure::FieldType::String));
			
			for (const auto& row : table->getRows())
			{
				if (stmt->where && !evaluateCondition(row, stmt->where.get())) continue;
				if (isWildcard)
				{
					resultSet->addRow(row);
				}
				else
				{
					Xale::DataStructure::Row projected;
					for (const auto& col : stmt->columns)
					{
						std::string name = colNamePart(col.value);
						for (const auto& field : row.fields)
							if (field.name == name) { projected.fields.push_back(field); break; }
					}
					resultSet->addRow(projected);
				}
			}
		}
		else
		{
			// JOIN path: build merged rows via nested-loop join
			std::vector<Xale::DataStructure::Row> mergedRows = table->getRows();

			for (const auto& join : stmt->joins)
			{
				auto joinTable = _tableManager.getTable(join.tableName);
				if (!joinTable)
					THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ExecutionError, "JOIN table does not exist: " + join.tableName);

				std::string leftCol  = colNamePart(join.leftTableCol);
				std::string rightCol = colNamePart(join.rightTableCol);

				std::vector<Xale::DataStructure::Row> newMerged;
				for (const auto& leftRow : mergedRows)
				{
					Xale::DataStructure::FieldValue leftVal;
					bool foundLeft = false;
					for (const auto& f : leftRow.fields)
						if (f.name == leftCol) { leftVal = f.value; foundLeft = true; break; }

					if (!foundLeft) continue;

					for (const auto& rightRow : joinTable->getRows())
					{
						Xale::DataStructure::FieldValue rightVal;
						bool foundRight = false;
						for (const auto& f : rightRow.fields)
							if (f.name == rightCol) { rightVal = f.value; foundRight = true; break; }

						// Compare values with type coercion (int vs double)
						auto valuesEqual = [](const Xale::DataStructure::FieldValue& a, const Xale::DataStructure::FieldValue& b) -> bool {
							if (a == b) return true;
							if (std::holds_alternative<int>(a) && std::holds_alternative<double>(b))
								return static_cast<double>(std::get<int>(a)) == std::get<double>(b);
							if (std::holds_alternative<double>(a) && std::holds_alternative<int>(b))
								return std::get<double>(a) == static_cast<double>(std::get<int>(b));
							return false;
						};

						if (foundRight && valuesEqual(leftVal, rightVal))
						{
							Xale::DataStructure::Row merged;
							merged.fields = leftRow.fields;
							for (const auto& f : rightRow.fields)
								merged.fields.push_back(f);
							newMerged.push_back(merged);
						}
					}
				}
				mergedRows = std::move(newMerged);
			}

			// Build result schema
			if (isWildcard)
			{
				for (const auto& col : table->getSchema()) resultSet->addColumn(col);
				for (const auto& join : stmt->joins)
				{
					auto jt = _tableManager.getTable(join.tableName);
					if (jt)
						for (const auto& col : jt->getSchema()) resultSet->addColumn(col);
				}
			}
			else
			{
				for (const auto& col : stmt->columns)
					resultSet->addColumn(Xale::DataStructure::ColumnDefinition(colNamePart(col.value), Xale::DataStructure::FieldType::String));
			}

			// Apply WHERE and project
			for (const auto& row : mergedRows)
			{
				if (stmt->where && !evaluateCondition(row, stmt->where.get())) continue;

				if (isWildcard)
				{
					resultSet->addRow(row);
				}
				else
				{
					Xale::DataStructure::Row projected;
					for (const auto& col : stmt->columns)
					{
						std::string name = colNamePart(col.value);
						for (const auto& field : row.fields)
							if (field.name == name) { projected.fields.push_back(field); break; }
					}
					resultSet->addRow(projected);
				}
			}
		}

		return resultSet;
	}

	std::unique_ptr<Xale::DataStructure::ResultSet> BasicExecutor::executeInsert(Xale::Query::InsertStatement* stmt)
	{
		auto table = _tableManager.getTable(stmt->tableName);
		if (!table) 
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ExecutionError, "Table does not exist");

		Xale::DataStructure::Row newRow;

		for (size_t i = 0; i < stmt->values.size(); ++i)
        {
			const auto& expr = stmt->values[i];
			Xale::DataStructure::FieldValue value = evaluateExpression(expr);
			const auto& schema = table->getSchema();
			if (i >= schema.size()) THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ExecutionError, "Too many values");
			newRow.fields.push_back(Xale::DataStructure::Field(schema[i].name, schema[i].type, value));
		}

		table->insertRow(newRow);
		
		// Auto-save after inserting row
		_tableManager.saveAllTables();
		
		return std::make_unique<Xale::DataStructure::ResultSet>();
	}

	std::unique_ptr<Xale::DataStructure::ResultSet> BasicExecutor::executeUpdate(Xale::Query::UpdateStatement* stmt)
	{
		auto table = _tableManager.getTable(stmt->tableName);

		if (!table) 
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ExecutionError, "Table does not exist");

		std::unordered_map<std::string, Xale::DataStructure::FieldValue> updates;
		for (const auto& assignment : stmt->assignments) 
            updates[assignment.first] = evaluateExpression(assignment.second);

		table->updateRows("", Xale::DataStructure::FieldValue(), updates);

		// Auto-save after updating rows
		_tableManager.saveAllTables();

		return std::make_unique<Xale::DataStructure::ResultSet>();
	}

	std::unique_ptr<Xale::DataStructure::ResultSet> BasicExecutor::executeDelete(Xale::Query::DeleteStatement* stmt)
	{
		auto table = _tableManager.getTable(stmt->tableName);

		if (!table) 
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ExecutionError, "Table does not exist");

		table->deleteRows("", Xale::DataStructure::FieldValue());

		// Auto-save after deleting rows
		_tableManager.saveAllTables();

		return std::make_unique<Xale::DataStructure::ResultSet>();
	}

	std::unique_ptr<Xale::DataStructure::ResultSet> BasicExecutor::executeCreate(Xale::Query::CreateStatement* stmt)
	{
		auto table = _tableManager.createTable(stmt->tableName);
		
		if (!table)
			THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ExecutionError, "Table already exists");
		
		if (!stmt->columns.empty())
		{
			for (const auto& colDef : stmt->columns)
			{
				Xale::DataStructure::FieldType fieldType;
				if (colDef.type == "INT" || colDef.type == "INTEGER")
					fieldType = Xale::DataStructure::FieldType::Integer;
				else if (colDef.type == "FLOAT" || colDef.type == "DOUBLE")
					fieldType = Xale::DataStructure::FieldType::Float;
				else if (colDef.type == "STRING" || colDef.type == "TEXT" || colDef.type == "VARCHAR")
					fieldType = Xale::DataStructure::FieldType::String;
				else
					THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ExecutionError, "Unknown column type: " + colDef.type);
				
				table->addColumn(Xale::DataStructure::ColumnDefinition(
					colDef.name,
					fieldType,
					colDef.isPrimaryKey,
					true,
					colDef.references.refTable,
					colDef.references.refColumn
				));
			}
		}
		
		return std::make_unique<Xale::DataStructure::ResultSet>();
	}

	std::unique_ptr<Xale::DataStructure::ResultSet> BasicExecutor::executeDrop(Xale::Query::DropStatement* stmt)
	{
		if (!_tableManager.dropTable(stmt->tableName))
            THROW_DB_EXCEPTION(Xale::Core::ExceptionCode::ExecutionError, "Table does not exist");
		return std::make_unique<Xale::DataStructure::ResultSet>();
	}

    std::unique_ptr<Xale::DataStructure::ResultSet> BasicExecutor::executeList(Xale::Query::ListStatement* stmt)
    {
        auto tableNames = _tableManager.getTableNames();
		const std::string colIdentifier = "table_name";
		const Xale::DataStructure::FieldType colType = Xale::DataStructure::FieldType::String;


        auto result = std::make_unique<Xale::DataStructure::ResultSet>();

        if (tableNames.empty())
            return result;
        
        result->addColumn(Xale::DataStructure::ColumnDefinition(colIdentifier, colType));

        for (const auto& tableName : tableNames)
        {
            Xale::DataStructure::Row row;
            auto field = Xale::DataStructure::Field(colIdentifier, colType, tableName);
            row.fields.push_back(field);
            result->addRow(row);
        }

        return result;
    }

	Xale::DataStructure::FieldValue BasicExecutor::evaluateExpression(const Xale::Query::Expression& expr)
	{
		switch (expr.type) {
			case Xale::Query::ExpressionType::NumericLiteral:
				try { return expr.value.find('.') != std::string::npos ? std::stod(expr.value) : std::stoi(expr.value); }
				catch (...) { return 0; }
			case Xale::Query::ExpressionType::StringLiteral: {
				std::string str = expr.value;
				if (str.length() >= 2 &&
				    ((str.front() == '\'' && str.back() == '\'') ||
				     (str.front() == '"'  && str.back() == '"')))
					str = str.substr(1, str.length() - 2);
				return str;
			}
			case Xale::Query::ExpressionType::Identifier: return expr.value;
			default: return std::monostate{};
		}
	}

	bool BasicExecutor::evaluateCondition(const Xale::DataStructure::Row& row, const Xale::Query::WhereClause* where)
	{
		if (!where || !where->condition)
			return true;
		
		const auto& condition = where->condition;
		
		if (condition->type == Xale::Query::ExpressionType::BinaryOp && condition->binary)
		{
			const auto& binary = condition->binary;
			
			if (binary->left->type != Xale::Query::ExpressionType::Identifier)
				return true;

			std::string columnName = binary->left->value;
			// Strip optional table prefix (e.g. "users.id" -> "id")
			{
				auto dot = columnName.rfind('.');
				if (dot != std::string::npos) columnName = columnName.substr(dot + 1);
			}

			Xale::DataStructure::FieldValue leftValue;
			bool found = false;
			for (const auto& field : row.fields)
			{
				if (field.name == columnName)
				{
					leftValue = field.value;
					found = true;
					break;
				}
			}
			
			if (!found)
				return false;
			
			Xale::DataStructure::FieldValue rightValue = evaluateExpression(*binary->right);
			
			const std::string& op = binary->op;
			
			try
			{
				if (op == "=")
					return leftValue == rightValue;
				else if (op == "!=")
					return leftValue != rightValue;
				else if (op == "<")
				{
					if (std::holds_alternative<int>(leftValue) && std::holds_alternative<int>(rightValue))
						return std::get<int>(leftValue) < std::get<int>(rightValue);
					if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue))
						return std::get<double>(leftValue) < std::get<double>(rightValue);
					return false;
				}
				else if (op == ">")
				{
					if (std::holds_alternative<int>(leftValue) && std::holds_alternative<int>(rightValue))
						return std::get<int>(leftValue) > std::get<int>(rightValue);
					if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue))
						return std::get<double>(leftValue) > std::get<double>(rightValue);
					return false;
				}
				else if (op == "<=")
				{
					if (std::holds_alternative<int>(leftValue) && std::holds_alternative<int>(rightValue))
						return std::get<int>(leftValue) <= std::get<int>(rightValue);
					if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue))
						return std::get<double>(leftValue) <= std::get<double>(rightValue);
					return false;
				}
				else if (op == ">=")
				{
					if (std::holds_alternative<int>(leftValue) && std::holds_alternative<int>(rightValue))
						return std::get<int>(leftValue) >= std::get<int>(rightValue);
					if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue))
						return std::get<double>(leftValue) >= std::get<double>(rightValue);
					return false;
				}
			}
			catch (...)
			{
				return false;
			}
		}
		
		return true;
	}
}
