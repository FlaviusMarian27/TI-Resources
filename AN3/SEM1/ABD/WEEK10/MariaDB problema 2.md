
Enunț asemănător ca la problema 1, dar mici modificari. Pași:

```bash
code docker-compose.yml
```

```yml
services:
  # =======================
  # MASTER
  # =======================
  db-master:
    image: mariadb:latest
    container_name: imdb_master
    restart: always
    environment:
      MARIADB_ROOT_PASSWORD: toor
      MARIADB_DATABASE: imdb
      MARIADB_USER: user_imdb
      MARIADB_PASSWORD: user_pass
    command: >
      --server-id=1
      --log-bin=mysql-bin
      --binlog-do-db=imdb
    ports:
      - "3307:3306"              # master accesibil pe localhost:3307
    volumes:
      - master_data:/var/lib/mysql
    networks:
      - imdb_net

  # =======================
  # SLAVE 1
  # =======================
  db-slave1:
    image: mariadb:latest
    container_name: imdb_slave1
    restart: always
    environment:
      MARIADB_ROOT_PASSWORD: toor
      MARIADB_DATABASE: imdb
      MARIADB_USER: user_imdb
      MARIADB_PASSWORD: user_pass
    command: >
      --server-id=2
      --log-bin=mysql-bin
      --relay-log=relay-bin
      --read-only=ON
    depends_on:
      - db-master
    ports:
      - "3308:3306"              # slave1 pe localhost:3308 (daca vrei sa te conectezi direct)
    volumes:
      - slave1_data:/var/lib/mysql
    networks:
      - imdb_net

  # =======================
  # SLAVE 2
  # =======================
  db-slave2:
    image: mariadb:latest
    container_name: imdb_slave2
    restart: always
    environment:
      MARIADB_ROOT_PASSWORD: toor
      MARIADB_DATABASE: imdb
      MARIADB_USER: user_imdb
      MARIADB_PASSWORD: user_pass
    command: >
      --server-id=3
      --log-bin=mysql-bin
      --relay-log=relay-bin
      --read-only=ON
    depends_on:
      - db-master
    ports:
      - "3309:3306"              # slave2 pe localhost:3309
    volumes:
      - slave2_data:/var/lib/mysql
    networks:
      - imdb_net

  # =======================
  # ADMINER (client web)
  # =======================
  adminer:
    image: adminer:latest
    container_name: imdb_adminer
    restart: always
    depends_on:
      - db-master
    ports:
      - "8080:8080"              # http://localhost:8080
    environment:
      ADMINER_DEFAULT_SERVER: db-master
    networks:
      - imdb_net

# =======================
# VOLUME PENTRU DATE
# =======================
volumes:
  master_data:
  slave1_data:
  slave2_data:

# =======================
# REȚEA
# =======================
networks:
  imdb_net:
    driver: bridge

```

```bash
docker compose down -v // pt a verificat sa fie oprit preventiv
```

```bash
docker compose up -d
docker compose ps
```

```bash
docker exec -it imdb_master mariadb -u root -p
```
Parola: toor

```SQL
SHOW DATABASES;

CREATE USER 'repl'@'%' IDENTIFIED BY 'replica_pass';
GRANT REPLICATION SLAVE ON *.* TO 'repl'@'%';
FLUSH PRIVILEGES;

USE imdb;
SHOW TABLES;
```

```SQL
SHOW MASTER STATUS
```

```bash
MariaDB [(none)]> SHOW DATABASES;
+--------------------+
| Database           |
+--------------------+
| imdb               |
| information_schema |
| mysql              |
| performance_schema |
| sys                |
+--------------------+
5 rows in set (0.002 sec)

MariaDB [(none)]> CREATE USER 'repl'@'%' IDENTIFIED BY 'replica_pass';
Query OK, 0 rows affected (0.005 sec)

MariaDB [(none)]> GRANT REPLICATION SLAVE ON *.* TO 'repl'@'%';
Query OK, 0 rows affected (0.004 sec)

MariaDB [(none)]> FLUSH PRIVILEGES;
Query OK, 0 rows affected (0.002 sec)

MariaDB [(none)]> USE imdb;
Database changed
MariaDB [imdb]> SHOW TABLES;
Empty set (0.001 sec)

MariaDB [imdb]> SHOW MASTER STATUS;
+------------------+----------+--------------+------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin.000002 |      342 | imdb         |                  |
+------------------+----------+--------------+------------------+
1 row in set (0.001 sec)

MariaDB [imdb]>
```

---

```bash
docker exec -it imdb_slave1 mariadb -u root -p
```

```SQL
STOP SLAVE;         -- ca să fim siguri că nu e ceva vechi
RESET SLAVE ALL;    -- șterge orice config de replicare existent

CHANGE MASTER TO
  MASTER_HOST='db-master',
  MASTER_USER='repl',
  MASTER_PASSWORD='replica_pass',
  MASTER_LOG_FILE='mysql-bin.000002',
  MASTER_LOG_POS=342;

START SLAVE;

SHOW SLAVE STATUS\G
```

```bash
flavius@flavius-Katana-GF66-12UEO:~/Documents/ABD/test1-2$ docker exec -it imdb_slave1 mariadb -u root -p
Enter password: 
Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MariaDB connection id is 4
Server version: 12.1.2-MariaDB-ubu2404-log mariadb.org binary distribution

Copyright (c) 2000, 2018, Oracle, MariaDB Corporation Ab and others.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

MariaDB [(none)]> STOP SLAVE;
Query OK, 0 rows affected, 1 warning (0.001 sec)

MariaDB [(none)]> RESET SLAVE ALL;
Query OK, 0 rows affected (0.001 sec)

MariaDB [(none)]> CHANGE MASTER TO MASTER_HOST='db-master',MASTER_USER='repl',MASTER_PASSWORD='replica_pass',MASTER_LOG_FILE='mysql-bin.000002',MASTER_LOG_POS=342;
Query OK, 0 rows affected, 1 warning (0.102 sec)

MariaDB [(none)]> START SLAVE;
Query OK, 0 rows affected (0.003 sec)

MariaDB [(none)]> SHOW SLAVE STATUS\G
*************************** 1. row ***************************
                Slave_IO_State: Waiting for master to send event
                   Master_Host: db-master
                   Master_User: repl
                   Master_Port: 3306
                 Connect_Retry: 60
               Master_Log_File: mysql-bin.000002
           Read_Master_Log_Pos: 342
                Relay_Log_File: relay-bin.000002
                 Relay_Log_Pos: 555
         Relay_Master_Log_File: mysql-bin.000002
              Slave_IO_Running: Yes
             Slave_SQL_Running: Yes
               Replicate_Do_DB: 
           Replicate_Ignore_DB: 
            Replicate_Do_Table: 
        Replicate_Ignore_Table: 
       Replicate_Wild_Do_Table: 
   Replicate_Wild_Ignore_Table: 
                    Last_Errno: 0
                    Last_Error: 
                  Skip_Counter: 0
           Exec_Master_Log_Pos: 342
               Relay_Log_Space: 858
               Until_Condition: None
                Until_Log_File: 
                 Until_Log_Pos: 0
            Master_SSL_Allowed: Yes
            Master_SSL_CA_File: 
            Master_SSL_CA_Path: 
               Master_SSL_Cert: 
             Master_SSL_Cipher: 
                Master_SSL_Key: 
         Seconds_Behind_Master: 0
 Master_SSL_Verify_Server_Cert: Yes
                 Last_IO_Errno: 0
                 Last_IO_Error: 
                Last_SQL_Errno: 0
                Last_SQL_Error: 
   Replicate_Ignore_Server_Ids: 
              Master_Server_Id: 1
                Master_SSL_Crl: 
            Master_SSL_Crlpath: 
                    Using_Gtid: No
                   Gtid_IO_Pos: 
       Replicate_Do_Domain_Ids: 
   Replicate_Ignore_Domain_Ids: 
                 Parallel_Mode: optimistic
                     SQL_Delay: 0
           SQL_Remaining_Delay: NULL
       Slave_SQL_Running_State: Slave has read all relay log; waiting for more updates
              Slave_DDL_Groups: 0
Slave_Non_Transactional_Groups: 0
    Slave_Transactional_Groups: 0
          Replicate_Rewrite_DB: 
                Connects_Tried: 1
            Master_Retry_Count: 100000
1 row in set (0.006 sec)

MariaDB [(none)]> 

```

```bash
docker exec -it imdb_slave2 mariadb -u root -p
```

toor

```SQL
STOP SLAVE;
RESET SLAVE ALL;

CHANGE MASTER TO
  MASTER_HOST='db-master',
  MASTER_USER='repl',
  MASTER_PASSWORD='replica_pass',
  MASTER_LOG_FILE='mysql-bin.000002',
  MASTER_LOG_POS=342;

START SLAVE;

SHOW SLAVE STATUS\G
```

```bash
MariaDB [(none)]> STOP SLAVE;
Query OK, 0 rows affected, 1 warning (0.001 sec)

MariaDB [(none)]> RESET SLAVE ALL;
Query OK, 0 rows affected (0.001 sec)

MariaDB [(none)]> CHANGE MASTER TO MASTER_HOST='db-master',MASTER_USER='repl',MASTER_PASSWORD='replica_pass',MASTER_LOG_FILE='mysql-bin.000002',MASTER_LOG_POS=342;
Query OK, 0 rows affected, 1 warning (0.105 sec)

MariaDB [(none)]> START SLAVE;
Query OK, 0 rows affected (0.002 sec)

MariaDB [(none)]> SHOW SLAVE STATUS\G
*************************** 1. row ***************************
                Slave_IO_State: Waiting for master to send event
                   Master_Host: db-master
                   Master_User: repl
                   Master_Port: 3306
                 Connect_Retry: 60
               Master_Log_File: mysql-bin.000002
           Read_Master_Log_Pos: 342
                Relay_Log_File: relay-bin.000002
                 Relay_Log_Pos: 555
         Relay_Master_Log_File: mysql-bin.000002
              Slave_IO_Running: Yes
             Slave_SQL_Running: Yes
               Replicate_Do_DB: 
           Replicate_Ignore_DB: 
            Replicate_Do_Table: 
        Replicate_Ignore_Table: 
       Replicate_Wild_Do_Table: 
   Replicate_Wild_Ignore_Table: 
                    Last_Errno: 0
                    Last_Error: 
                  Skip_Counter: 0
           Exec_Master_Log_Pos: 342
               Relay_Log_Space: 858
               Until_Condition: None
                Until_Log_File: 
                 Until_Log_Pos: 0
            Master_SSL_Allowed: Yes
            Master_SSL_CA_File: 
            Master_SSL_CA_Path: 
               Master_SSL_Cert: 
             Master_SSL_Cipher: 
                Master_SSL_Key: 
         Seconds_Behind_Master: 0
 Master_SSL_Verify_Server_Cert: Yes
                 Last_IO_Errno: 0
                 Last_IO_Error: 
                Last_SQL_Errno: 0
                Last_SQL_Error: 
   Replicate_Ignore_Server_Ids: 
              Master_Server_Id: 1
                Master_SSL_Crl: 
            Master_SSL_Crlpath: 
                    Using_Gtid: No
                   Gtid_IO_Pos: 
       Replicate_Do_Domain_Ids: 
   Replicate_Ignore_Domain_Ids: 
                 Parallel_Mode: optimistic
                     SQL_Delay: 0
           SQL_Remaining_Delay: NULL
       Slave_SQL_Running_State: Slave has read all relay log; waiting for more updates
              Slave_DDL_Groups: 0
Slave_Non_Transactional_Groups: 0
    Slave_Transactional_Groups: 0
          Replicate_Rewrite_DB: 
                Connects_Tried: 1
            Master_Retry_Count: 100000
1 row in set (0.007 sec)

MariaDB [(none)]>
```

```bash
mkdir data
cd data
wget https://datasets.imdbws.com/name.basics.tsv.gz
gunzip name.basics.tsv.gz
ls -lh
```

```bash
docker cp name.basics.tsv imdb_master:/tmp/name.basics.tsv
```

```bash
docker exec -it imdb_master ls -lh /tmp
```

------

```bash
docker exec -it imdb_master mariadb -u root -p
```

in caz ca nu exista:
```SQL
CREATE DATABASE IF NOT EXISTS imdb
  CHARACTER SET utf8mb4
  COLLATE utf8mb4_unicode_ci;

USE imdb;
```

---

```SQL
CREATE TABLE name_basics (
  nconst            VARCHAR(12)  NOT NULL PRIMARY KEY,
  primaryName       VARCHAR(255) NULL,
  birthYear         INT NULL,
  deathYear         INT NULL,
  primaryProfession TEXT NULL,
  knownForTitles    TEXT NULL
) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

```SQL
SHOW TABLES;
DESCRIBE name_basics;
```

Importa fisierul in tabela:

```bash
LOAD DATA INFILE '/tmp/name.basics.tsv'
INTO TABLE name_basics
FIELDS TERMINATED BY '\t'
LINES TERMINATED BY '\n'
IGNORE 1 LINES
(nconst, primaryName, birthYear, deathYear, primaryProfession, knownForTitles);
```

Pe maste:

```SQL
SELECT COUNT(*) FROM name_basics;
```

Pe fiecare slave:

```SQL
SELECT COUNT(*) FROM imdb.name_basics;
```


Pt onnor:
```SQL
SELECT COUNT(*) FROM name_basics WHERE primaryName LIKE '%onnor';
//3.116 sec
```

Cream coloana:
```SQL
ALTER TABLE name_basics
ADD COLUMN rev_name VARCHAR(255);
```

```SQL
UPDATE name_basics 
SET rev_name = REVERSE(primaryName);
```

Creaza index pt coloana inversa:
```SQL
ALTER TABLE name_basics
ADD INDEX idx_rev_name (rev_name);
```

```SQL
SELECT COUNT(*) 
FROM name_basics 
WHERE rev_name LIKE 'ronno%';
//0.084 sec
//0.007 sec dupa 
```

Pt alley:

Facem o coloana de tip flag:
```SQL
ALTER TABLE name_basics
ADD COLUMN has_alley TINYINT(1) NOT NULL DEFAULT 0;
```

Pentru ca primaryName poate sa fie si NULL:
```SQL
UPDATE name_basics
SET has_alley = IF(primaryName LIKE '%alley%', 1, 0);
```

```SQL
CREATE INDEX idx_has_alley ON name_basics(has_alley);
```

```SQL
SELECT COUNT(*)
FROM name_basics
WHERE has_alley = 1;
```

```bash
docker compose down -v
```
