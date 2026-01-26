# xale-db

XaleDB is a minimal _SQL_ database engine.

## Dependencies

- xale-logger (a minimal c++ logger: [axdelafuen/xale-logger](https://github.com/axdelafuen/xale-logger/))

## Build

```bash
mkdir -p build && cd build
cmake ..
make
```

## Run

```bash
./build/xale-db
```

## Usage

_This is the first implementation goal:_

```
CREATE TABLE xaleUser (
	id INT,
	name STRING,
	age INT
);
```

> Table `xaleUser` created successfully.

---


```
INSERT INTO xaleUser (id, name, age) VALUES (1, "Alice", 30);
```

> 1 row inserted into `xaleUser`.

---

```
SELECT * FROM xaleUser;
```

> | id | name  | age |
> |----|-------|-----|
> | 1  | Alice | 30  |

---

```
UPDATE xaleUser SET age = 31 WHERE id = 1;
```

> 1 row updated in `xaleUser`.

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

> 1 row deleted from `xaleUser`.

## Author

[axdelafuen](https://github.com/axdelafuen)
