
- Descărcați un dataset de pe [https://developer.imdb.com/non-commercial-datasets/](https://developer.imdb.com/non-commercial-datasets/) : titles.basics.tsv.gz

- Creați tabela corespunzătoare într-o bază de date Postgres și încărcați datele de mai sus.

- Faceți un studiu de caz asupra căutării în această tabelă:
	- căutare după nume care se termină cu „phecy”
	- căutare după nume care conțin „une”

- Scopul exercițiului este să reduceți timpul de căutare sub 1 secundă pentru exemplele de mai sus. Baza de date trebuie să fie replicată în 3 instanțe slave.

- Se va nota ordinea în care ați făcut operațiile (de exemplu, dacă ați creat mai întâi slave-urile sau ați încărcat informațiile în baza de date master mai întâi).

- Trebuie să încărcați:
    - fișierul docker-compose.yml / fișierul Dockerfile (dacă nu ați folosit docker compose);
    - un fișier TXT cu toate comenzile pe care le-ați rulat în shell de MySQL (gen SELECT * FROM ...) → mysql-history.txt;
    - un fișier TXT cu toate comenzile pe care le-ați rulat în linie de comandă (inclusiv docker compose up -d) → shell-history.txt;
    - un fișier TXT cu o scurtă explicație a ceea ce ați făcut (și de ce, cum ați redus timpul de căutare etc.) → explicatii.txt.


---
# Rezolvare:


## Docker-compose:

```yml
# PostgreSQL Streaming Replication Setup
# 1 Primary (Master) + 3 Replicas (Slaves)

services:
  # ===========================================
  # PRIMARY SERVER (MASTER)
  # ===========================================
  pg_primary:
    image: postgres:16
    container_name: pg_primary
    hostname: pg_primary
    environment:
      POSTGRES_USER: postgres
      POSTGRES_PASSWORD: secretpassword
      POSTGRES_DB: imdb
      # Replication user credentials
      REPLICATION_USER: replicator
      REPLICATION_PASSWORD: replpassword
    volumes:
      # Custom configuration
      - ./config/primary/postgresql.conf:/etc/postgresql/postgresql.conf:ro
      - ./config/primary/pg_hba.conf:/etc/postgresql/pg_hba.conf:ro
      # Initialization script (creates replication user)
      - ./scripts/init-primary.sh:/docker-entrypoint-initdb.d/init-primary.sh:ro
      - ./data:/import:ro
      # Persistent data
      - pg_primary_data:/var/lib/postgresql/data
    command: postgres -c config_file=/etc/postgresql/postgresql.conf -c hba_file=/etc/postgresql/pg_hba.conf
    ports:
      - "5433:5432"
    networks:
      - pg_network
    healthcheck:
      test: ["CMD-SHELL", "pg_isready -U postgres"]
      interval: 5s
      timeout: 5s
      retries: 5

  # ===========================================
  # REPLICA 1 (SLAVE 1)
  # ===========================================
  pg_replica1:
    image: postgres:16
    container_name: pg_replica1
    hostname: pg_replica1
    environment:
      POSTGRES_USER: postgres
      POSTGRES_PASSWORD: secretpassword
      PGUSER: postgres
      PGPASSWORD: secretpassword
      # Primary connection info for pg_basebackup
      PRIMARY_HOST: pg_primary
      PRIMARY_PORT: 5432
      REPLICATION_USER: replicator
      REPLICATION_PASSWORD: replpassword
    volumes:
      - ./config/replica/postgresql.conf:/etc/postgresql/postgresql.conf:ro
      - ./config/replica/pg_hba.conf:/etc/postgresql/pg_hba.conf:ro
      - ./scripts/init-replica.sh:/init-replica.sh:ro
      - pg_replica1_data:/var/lib/postgresql/data
    ports:
      - "5434:5432"
    networks:
      - pg_network
    depends_on:
      pg_primary:
        condition: service_healthy
    # Override entrypoint to run our initialization
    entrypoint: ["/bin/bash", "/init-replica.sh"]

  # ===========================================
  # REPLICA 2 (SLAVE 2)
  # ===========================================
  pg_replica2:
    image: postgres:16
    container_name: pg_replica2
    hostname: pg_replica2
    environment:
      POSTGRES_USER: postgres
      POSTGRES_PASSWORD: secretpassword
      PGUSER: postgres
      PGPASSWORD: secretpassword
      PRIMARY_HOST: pg_primary
      PRIMARY_PORT: 5432
      REPLICATION_USER: replicator
      REPLICATION_PASSWORD: replpassword
    volumes:
      - ./config/replica/postgresql.conf:/etc/postgresql/postgresql.conf:ro
      - ./config/replica/pg_hba.conf:/etc/postgresql/pg_hba.conf:ro
      - ./scripts/init-replica.sh:/init-replica.sh:ro
      - pg_replica2_data:/var/lib/postgresql/data
    ports:
      - "5435:5432"
    networks:
      - pg_network
    depends_on:
      pg_primary:
        condition: service_healthy
    entrypoint: ["/bin/bash", "/init-replica.sh"]

  # ===========================================
  # REPLICA 3 (SLAVE 3)
  # ===========================================
  pg_replica3:
    image: postgres:16
    container_name: pg_replica3
    hostname: pg_replica3
    environment:
      POSTGRES_USER: postgres
      POSTGRES_PASSWORD: secretpassword
      PGUSER: postgres
      PGPASSWORD: secretpassword
      PRIMARY_HOST: pg_primary
      PRIMARY_PORT: 5432
      REPLICATION_USER: replicator
      REPLICATION_PASSWORD: replpassword
    volumes:
      - ./config/replica/postgresql.conf:/etc/postgresql/postgresql.conf:ro
      - ./config/replica/pg_hba.conf:/etc/postgresql/pg_hba.conf:ro
      - ./scripts/init-replica.sh:/init-replica.sh:ro
      - pg_replica3_data:/var/lib/postgresql/data
    ports:
      - "5436:5432"
    networks:
      - pg_network
    depends_on:
      pg_primary:
        condition: service_healthy
    entrypoint: ["/bin/bash", "/init-replica.sh"]
    
  # =========================
  # ADMINER (UI)
  # =========================
  adminer:
    image: adminer:latest
    container_name: pg_adminer
    restart: always
    depends_on:
      - pg_primary
    ports:
      - "8080:8080"
    environment:
      ADMINER_DEFAULT_SERVER: pg_primary
    networks:
      - pg_network

# ===========================================
# NETWORK
# ===========================================
networks:
  pg_network:
    driver: bridge

# ===========================================
# VOLUMES (Persistent Storage)
# ===========================================
volumes:
  pg_primary_data:
  pg_replica1_data:
  pg_replica2_data:
  pg_replica3_data:
```

## Create the folder for the data

```bash
mkdir -p data
cd data
wget https://datasets.imdbws.com/title.basics.tsv.gz
gunzip -f title.basics.tsv.gz
head -n 1000001 title.basics.tsv > title.basics.1M.tsv
wc -l title.basics.1M.tsv
cd ..
```

```bash
ls -R config scripts
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

```bash
docker exec -it pg_primary psql -U postgres -d imdb
```

---

Now, we are in MASTER:
```SQL
SELECT client_addr, state, sync_state FROM pg_stat_replication;
```

For exit:
```SQL
quit
```

---
Test for a SLAVE:

```SQL
SELECT client_addr, state, sync_state FROM pg_stat_replication;
```

```SQL
quit
```

---

-> To see if the MASTER sees the 2 files
```bash
docker exec -it pg_primary ls -lh /import
```

Output:

```bash
total 1.1G
-rw-rw-r-- 1 1000 1000   80M Feb  1 08:58 title.basics.1M.tsv
-rw-rw-r-- 1 1000 1000 1013M Jan 31 12:32 title.basics.tsv
```

---

```bash
docker exec -it pg_primary psql -U postgres -d imdb
```

Again in MASTER:
```SQL
\timing on
```

```SQL
\pset pager off
```

```SQL
DROP TABLE IF EXISTS title_basics;
```

```SQL
CREATE TABLE title_basics (
  tconst         VARCHAR(12) PRIMARY KEY,
  titleType      TEXT,
  primaryTitle   TEXT,
  originalTitle  TEXT,
  isAdult        INT,
  startYear      INT,
  endYear        INT,
  runtimeMinutes INT,
  genres         TEXT
);
```

```SQL
COPY title_basics 
FROM '/import/title.basics.1M.tsv' 
WITH (FORMAT text, DELIMITER E'\t', NULL '\N', HEADER true);
```

```SQL
EXPLAIN ANALYZE SELECT COUNT(*) FROM title_basics;
```

---

### 1) Now we'll search for all the names that end "phecy":

#### Primitive/banal version:
```SQL
EXPLAIN ANALYZE SELECT COUNT(*) 
FROM title_basics WHERE lower(primaryTitle) LIKE '%phecy';
```

The result: 0.101 sec

### The solve:
#### Index-able prefix with reverse() + index B-tree.
```SQL
DROP INDEX IF EXISTS idx_rev_phecy;
```

```SQL
CREATE INDEX idx_rev_phecy 
ON title_basics ((reverse(lower(primaryTitle)) COLLATE "C") text_pattern_ops);
```

```SQL
ANALYZE title_basics;
```

```SQL
EXPLAIN ANALYZE SELECT COUNT(*) FROM title_basics WHERE (reverse(lower(primaryTitle)) COLLATE "C") LIKE 'ycehp%';
```

The result: 0.004 sec

---

## Now we will look for all the words that contain "une":

#### Primitive/banal version:
```SQL
EXPLAIN ANALYZE SELECT COUNT(*) 
FROM title_basics WHERE lower(primaryTitle) LIKE '%une%';
```

The result: 0.102 sec

## The solve:

#### Bitmap Index Scan by GIN + pg_trgm:
```SQL
CREATE EXTENSION IF NOT EXISTS pg_trgm;
```

```SQL
DROP INDEX IF EXISTS idx_trgm_une;
```

```SQL
CREATE INDEX idx_trgm_une 
ON title_basics USING GIN (lower(primaryTitle) gin_trgm_ops);
```

```SQL
ANALYZE title_basics;
```

```SQL
EXPLAIN ANALYZE SELECT COUNT(*) 
FROM title_basics WHERE lower(primaryTitle) LIKE '%une%';
```

The result: 0.029 sec

---
```SQL
quit
```

---

```bash
docker exec -it pg_replica1 psql -U postgres -d imdb
```

### SLAVE 1:

```SQL
EXPLAIN ANALYZE SELECT COUNT(*) 
FROM title_basics WHERE lower(primaryTitle) LIKE '%une%';
```

---

```bash
docker compose down -v
```

## Explanations:

- Pe eșantionul de 1M înregistrări, interogările sunt deja sub 1 secundă datorită scanării paralele și faptului că  tabela nu este enormă; totuși, EXPLAIN ANALYZE arată schimbarea planului de execuție (de la scan la index), iar pe dataset complet (zeci de milioane) diferența devine mult mai mare.”

- Am urmărit nu doar timpul, ci și planul de execuție: pentru substring am trecut la Bitmap Index Scan prin GIN + pg_trgm, iar pentru sufix am transformat căutarea într-un prefix indexabil cu reverse(...) + index B-tree.

- Prin urmare, chiar dacă pe 1M diferența de timp e mică, metoda e scalabilă și garantează performanță mult mai bună pe volume mari.



