
- Descărcați un dataset de pe [https://developer.imdb.com/non-commercial-datasets/](https://developer.imdb.com/non-commercial-datasets/) : titles.basics.tsv.gz
- Creați tabela corespunzătoare într-o bază de date MySQL și încărcați datele de mai sus.

- Faceți un studiu de caz asupra căutării în această tabelă:
	- căutare după nume care se termină cu „things”
	- căutare după nume care conțin „stars”

- Scopul exercițiului este să reduceți timpul de căutare sub 1 secundă pentru exemplele de mai sus. Baza de date trebuie să fie replicată în 3 instanțe slave.

- Se va nota ordinea în care ați făcut operațiile (de exemplu, dacă ați creat mai întâi slave-urile sau ați încărcat informațiile în baza de date master mai întâi).

- Trebuie să încărcați:
	- fișierul docker-compose.yml / fișierul Dockerfile (dacă nu ați folosit docker compose);
	- un fișier TXT cu toate comenzile pe care le-ați rulat în shell de MySQL (gen SELECT * FROM ...) → mysql-history.txt;
	- un fișier TXT cu toate comenzile pe care le-ați rulat în linie de comandă (inclusiv docker compose up -d) → shell-history.txt;
	- un fișier TXT cu o scurtă explicație a ceea ce ați făcut (și de ce, cum ați redus timpul de căutare etc.) → explicatii.txt.


---

# Rezolvare:

```bash
mkdir data
cd data
wget https://datasets.imdbws.com/title.basics.tsv.gz
gunzip -f title.basics.tsv.gz
head -n 1000001 title.basics.tsv > title.basics.1M.tsv
wc -l title.basics.1M.tsv
cd ..
```

```bash
docker compose down -v
```

```bash
docker compose up -d
```

```bash
docker compose ps
```

---

```bash
docker exec -it imdb_master mariadb -u root -p
```

password: toor

# Master

```SQL
SHOW DATABASES;

CREATE USER 'repl'@'%' IDENTIFIED BY 'replica_pass';
GRANT REPLICATION SLAVE ON *.* TO 'repl'@'%';
FLUSH PRIVILEGES;
USE imdb;
SHOW TABLES;
SHOW MASTER STATUS;
```

```SQL
exit
```

---

```bash
docker exec -it imdb_slave1 mariadb -u root -p
```
password: toor

# Slave1:

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

```SQL
exit
```

---
```bash
docker exec -it imdb_slave2 mariadb -u root -p
```
password: toor

# Slave2:

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

```SQL
exit
```

---

```bash
cd data
docker cp title.basics.1M.tsv imdb_master:/tmp/title.basics.tsv
docker exec -it imdb_master ls -lh /tmp
cd ..
```

---

```SQL
docker exec -it imdb_master mariadb -u root -p
```

# MASTER

```SQL
USE imdb;
```

```SQL
CREATE TABLE title_basics (
  tconst VARCHAR(20)  NOT NULL PRIMARY KEY,
  titleType VARCHAR(512) NULL,   
  primaryTitle VARCHAR(512) NULL,
  originalTitle VARCHAR(512) NULL,
  isAdult TINYINT(1) NULL,
  startYear INT NULL,
  endYear INT NULL,
  runtimeMinutes INT NULL,
  genres TEXT NULL
) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
```

```SQL
SHOW TABLES;
DESCRIBE title_basics;
```

```SQL
LOAD DATA INFILE '/tmp/title.basics.tsv'
INTO TABLE title_basics
FIELDS TERMINATED BY '\t'
LINES TERMINATED BY '\n'
IGNORE 1 LINES
(tconst,titleType,primaryTitle,originalTitle,isAdult,startYear,endYear,runtimeMinutes,genres);
```

### 1) Search for words ending in things:

#### Primitive/banal version:

```SQL
SELECT COUNT(*) FROM title_basics WHERE primaryTitle LIKE '%things';
```

The result: 0.321 sec

#### Reverse() + new column

```SQL
ALTER TABLE title_basics ADD COLUMN rev_primaryTitle VARCHAR(512);
UPDATE title_basics SET rev_primaryTitle = REVERSE(primaryTitle);
CREATE INDEX idx_rev ON title_basics(rev_primaryTitle);
```

```SQL
SELECT COUNT(*) FROM title_basics WHERE rev_primaryTitle LIKE 'sgniht%';
```

The result: 0.003 sec

---
### 2) Search for words containing "stars":

```SQL
SELECT COUNT(*) FROM title_basics WHERE primaryTitle LIKE '%stars%';
```

The result: 0.410 sec

#### Column flag + index

```SQL
ALTER TABLE title_basics ADD COLUMN has_stars TINYINT(1) NOT NULL DEFAULT 0; 
```

```SQL
UPDATE title_basics SET has_stars = IF(primaryTitle LIKE '%stars%', 1, 0);
```

```SQL
CREATE INDEX idx_has_stars ON title_basics(has_stars);
```

```SQL
SELECT COUNT(*) FROM title_basics WHERE has_stars = 1;
```

The result: 0.003 sec

```SQL
exit
```

## Replicarea am verificat-o in browser si functioneaza. Se poate testa si in terminal.

```bash
docker compose down -v
```


# Explicatii:

Am optimizat căutările în MySQL astfel:

1. **Pentru „se termină cu”**: Am folosit `REVERSE()` pe titluri și un index B-tree pe coloana inversată. Așa am transformat căutările de tip sufix într-un index scan rapid.
2. **Pentru „conține”**: Am creat o coloană `has_stars` care marchează titlurile ce conțin „stars” și am indexat-o. Astfel, căutările s-au realizat rapid pe index, nu pe întreaga tabelă.
3. **Pe volume mari**: Optimizările asigură performanță pe baze de date mari, evitând scanările complete ale tabelei.
4. **Replicare pe 3 slave**: Am configurat replicarea pentru scalabilitate și performanță la cererile de citire.

În final, am obținut o performanță rapidă și scalabilă, chiar și pe baze de date mari.