LIST TABLE;

CREATE TABLE table_1 (id INT PRIMARY KEY, name STRING);

CREATE TABLE table_2 (id INT PRIMARY KEY, t1Id INT REFERENCES table_1(id), addr STRING);

LIST TABLE;

INSERT INTO table_1 VALUES (1, "name1");
INSERT INTO table_1 VALUES (2, "name2");

INSERT INTO table_2 VALUES (1, 1, "addr1_of_name1");
INSERT INTO table_2 VALUES (2, 1, "addr2_of_name1");
INSERT INTO table_2 VALUES (3, 2, "addr1_of_name2");

SELECT * FROM table_1;

SELECT * FROM table_2;

SELECT table_1.name, table_2.addr FROM table_1 JOIN table_2 ON table_1.id = table_2.t1Id;
