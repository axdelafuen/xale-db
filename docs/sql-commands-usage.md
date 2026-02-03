# SQL commands usage

## Types

- `INT`
- `FLOAT`
- `STRING`
- `NULL`

## Operators

- `=`, `!=`, `<`, `>`, `<=`, `>=`

## Table definitions

**Create a table:**

```sql
CREATE TABLE `table_name` (`col_1_name` [TYPE], `col_2_name` [TYPE], ...)
```

**Update a table:**

_Not implemented yet!_

**Delete a table:**

```sql
DROP TABLE `table_name`
```

**List all tables stored:**

```sql
LIST TABLE
```

## Table content manipulation

**Select data from a table:**

```sql
SELECT `col_1_name`, `col_2_name`, ... 
FROM `table_name` 
WHERE `col_x_name` [OPERATOR] `value`
```

**Insert data into a table:**

```sql
INSERT INTO `table_name` VALUES `value1`, `value2`
```

`value1` _correspond to:_ `col_1_name`

**Update data of a table:**

_Not implemented yet!_

**Delete data of a table:**

```sql
DELETE * 
FROM `table_name` 
WHERE `col_x_name` [OPERATOR] `value`
```

### Joins

_Not implemented yet!_
