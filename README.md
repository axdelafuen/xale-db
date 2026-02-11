## Overview

XaleDB is a minimal _SQL_ database engine.

checkout technical doc: [xale-db: Main Page](https://axdelafuen.github.io/xale-db/)

## Dependencies

- xale-logger (a minimal c++ logger: [axdelafuen/xale-logger](https://github.com/axdelafuen/xale-logger/))

## Build

```bash
mkdir -p build
cd build
cmake ..
make
```

or

```bash
cmake -B ./build
cmake --build ./build
```

or using `autorun.sh`

## Run

**Debug:**

```bash
./build/xale-db-debug
```

**Server:**

```bash
./build/xale-db-server
```

**Client (CLI):**

```bash
./build/xale-db-cli
```

**Tests:**
```bash
./build/xale-db-tests
```

## Usage

**Features:**

- Basic SQL commands: `CREATE TABLE`, `INSERT`, `SELECT`, `UPDATE`, `DELETE`
- File-based storage
- Client-server architecture

**Not Implemented Features:**

- Advanced SQL commands and features
- Multiple commands in one query
- Concurrency control
- Indexing
- Transactions


### Commands examples

```
CREATE TABLE xaleUser (id INT, name STRING, age INT)
```

> Query OK, table created

---


```
INSERT INTO xaleUser (id, name, age) VALUES (1, "Alice", 30)
```

> Query OK, 1 row inserted

---

```
SELECT * FROM xaleUser
```

> | id | name  | age |
> |----|-------|-----|
> | 1  | Alice | 30  |
>
> 2 rows in set

---

```
UPDATE xaleUser SET age = 31 WHERE id = 1
```

> Query OK, 1 row updated

---

```
SELECT * FROM xaleUser;
```

> | id | name  | age |
> |----|-------|-----|
> | 1  | Alice | 31  |

---

```
DELETE FROM xaleUser WHERE id = 1;
```

> Query OK, 1 row deleted

## License

This project is licensed under the GNU GENERAL PUBLIC LICENSE Version 3 - see the [LICENSE](LICENSE) file for details.

## Author

[axdelafuen](https://github.com/axdelafuen)
