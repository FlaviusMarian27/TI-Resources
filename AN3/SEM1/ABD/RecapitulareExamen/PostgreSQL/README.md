# PostgreSQL Streaming Replication with Docker

This project sets up a PostgreSQL streaming replication cluster with **1 primary (master)** and **3 replicas (slaves)** using Docker Compose.

## Table of Contents

1. [Quick Start](#quick-start)
2. [Architecture Overview](#architecture-overview)
3. [How Replication Works](#how-replication-works)
4. [Configuration Explained](#configuration-explained)
5. [Testing the Setup](#testing-the-setup)
6. [Common Operations](#common-operations)
7. [Troubleshooting](#troubleshooting)

---

## Quick Start

```bash
# 1. Make scripts executable
chmod +x manage.sh scripts/*.sh

# 2. Start the cluster
./manage.sh start

# 3. Wait ~30 seconds for initialization, then check status
./manage.sh status

# 4. Run tests to verify replication
./manage.sh test
```

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                     Docker Network (pg_network)                  │
│                                                                  │
│  ┌──────────────────┐                                           │
│  │   pg_primary     │◄────── Writes go here                     │
│  │   (Master)       │                                           │
│  │   Port: 5432     │                                           │
│  └────────┬─────────┘                                           │
│           │                                                      │
│           │ WAL Streaming (continuous)                          │
│           │                                                      │
│     ┌─────┴─────┬─────────────┐                                 │
│     ▼           ▼             ▼                                 │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐                        │
│  │ replica1 │ │ replica2 │ │ replica3 │◄── Read queries here   │
│  │ Port:5433│ │ Port:5434│ │ Port:5435│                        │
│  └──────────┘ └──────────┘ └──────────┘                        │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

**Port Mapping:**
| Server | Container Port | Host Port |
|--------|---------------|-----------|
| Primary | 5432 | 5432 |
| Replica 1 | 5432 | 5433 |
| Replica 2 | 5432 | 5434 |
| Replica 3 | 5432 | 5435 |

---

## How Replication Works

### Write-Ahead Log (WAL)

PostgreSQL uses WAL to ensure data durability. Every change to the database is first written to WAL files before being applied to the actual data files.

```
Transaction → WAL Buffer → WAL Files → Data Files
                              ↓
                         Streamed to Replicas
```

### Streaming Replication Flow

1. **Client writes to Primary** → Data is written to WAL
2. **WAL Sender** process on Primary streams WAL to replicas
3. **WAL Receiver** on each replica receives the stream
4. **Startup** process on replica applies WAL to local data

### Synchronous vs Asynchronous

- **Asynchronous (this setup):** Primary doesn't wait for replicas. Faster but small risk of data loss if primary crashes before replicas receive data.
- **Synchronous:** Primary waits for at least one replica to confirm receipt. Slower but no data loss.

---

## Configuration Explained

### Primary Server (postgresql.conf)

```ini
# Enable streaming replication
wal_level = replica        # Log enough info for replication

# Allow replica connections
max_wal_senders = 10       # Max concurrent WAL sender processes
max_replication_slots = 10 # Slots prevent WAL deletion

# Keep WAL for slow replicas
wal_keep_size = 1GB        # Retain this much WAL

# Allow read queries on replicas
hot_standby = on           # Inherited by replicas
```

### Primary Server (pg_hba.conf)

```
# Allow replication connections from Docker network
host  replication  replicator  172.0.0.0/8  scram-sha-256
```

This line says:
- Connection type: `host` (TCP/IP)
- Database: `replication` (special keyword for replication)
- User: `replicator` (our replication user)
- Address: `172.0.0.0/8` (Docker networks)
- Auth: `scram-sha-256` (password authentication)

### Replica Initialization (pg_basebackup)

Replicas are initialized using `pg_basebackup`:

```bash
pg_basebackup \
    -h pg_primary \        # Primary hostname
    -U replicator \        # Replication user
    -D /var/lib/postgresql/data \  # Destination
    -Fp \                  # Plain format (directory)
    -Xs \                  # Stream WAL during backup
    -R                     # Create standby.signal & configure recovery
```

The `-R` flag automatically creates:
- `standby.signal` - Tells PostgreSQL this is a standby server
- `postgresql.auto.conf` - Contains `primary_conninfo` connection string

---

## Testing the Setup

### Manual Testing

```bash
# Connect to primary
docker exec -it pg_primary psql -U postgres -d testdb

# Create a table
CREATE TABLE test (id serial, name text);

# Insert data
INSERT INTO test (name) VALUES ('Hello Replication!');

# Check data
SELECT * FROM test;
```

```bash
# Connect to replica (different terminal)
docker exec -it pg_replica1 psql -U postgres -d testdb

# Verify data replicated
SELECT * FROM test;

# Try to write (should fail!)
INSERT INTO test (name) VALUES ('This will fail');
-- ERROR: cannot execute INSERT in a read-only transaction
```

### Check Replication Status

On the primary:

```sql
-- See connected replicas
SELECT 
    client_addr,
    state,
    sent_lsn,
    replay_lsn,
    sync_state
FROM pg_stat_replication;
```

On a replica:

```sql
-- Check if in recovery mode (should be true)
SELECT pg_is_in_recovery();

-- See replication lag
SELECT 
    now() - pg_last_xact_replay_timestamp() AS replication_lag;
```

### Automated Testing

```bash
./manage.sh test
```

---

## Common Operations

### Connect to Servers

```bash
# Primary (read/write)
./manage.sh primary
# or
psql -h localhost -p 5432 -U postgres -d testdb

# Replica 1 (read-only)
./manage.sh replica1
# or
psql -h localhost -p 5433 -U postgres -d testdb
```

### View Logs

```bash
# All containers
./manage.sh logs

# Specific container
docker logs pg_primary -f
docker logs pg_replica1 -f
```

### Restart a Single Replica

```bash
docker restart pg_replica1
```

### Fresh Start

```bash
./manage.sh clean   # Removes all data!
./manage.sh start   # Start fresh
```

---

## Troubleshooting

### Replica Not Connecting

**Check if primary is accepting connections:**
```bash
docker exec pg_primary pg_isready
```

**Check primary logs:**
```bash
docker logs pg_primary | grep -i replication
```

**Verify replication user exists:**
```bash
docker exec pg_primary psql -U postgres -c "\du replicator"
```

### Replication Lag

**Check lag on primary:**
```sql
SELECT 
    client_addr,
    pg_size_pretty(pg_wal_lsn_diff(sent_lsn, replay_lsn)) AS lag
FROM pg_stat_replication;
```

High lag could mean:
- Network issues
- Replica under heavy read load
- Slow disk on replica

### Replica Falls Behind (WAL Removed)

If a replica disconnects too long, it may miss WAL:

```
FATAL: requested WAL segment has already been removed
```

**Solution:** Re-initialize the replica:
```bash
docker stop pg_replica1
docker volume rm pg-replication_pg_replica1_data
docker start pg_replica1
```

### Cannot Write to Replica

This is **expected behavior**! Replicas are read-only:
```
ERROR: cannot execute INSERT in a read-only transaction
```

All writes must go to the primary.

---

## Connection Strings

**Primary (read/write):**
```
postgresql://postgres:secretpassword@localhost:5432/testdb
```

**Replicas (read-only):**
```
postgresql://postgres:secretpassword@localhost:5433/testdb
postgresql://postgres:secretpassword@localhost:5434/testdb
postgresql://postgres:secretpassword@localhost:5435/testdb
```

---

## File Structure

```
pg-replication/
├── docker-compose.yml          # Main compose file
├── manage.sh                   # Cluster management script
├── README.md                   # This file
├── config/
│   ├── primary/
│   │   ├── postgresql.conf     # Primary config
│   │   └── pg_hba.conf         # Primary access control
│   └── replica/
│       ├── postgresql.conf     # Replica config
│       └── pg_hba.conf         # Replica access control
└── scripts/
    ├── init-primary.sh         # Creates replication user
    ├── init-replica.sh         # Runs pg_basebackup
    └── test-replication.sh     # Automated tests
```

---

## Next Steps

Once comfortable with this setup, you could explore:

1. **Synchronous Replication** - Uncomment the synchronous settings in `postgresql.conf`
2. **Automatic Failover** - Tools like Patroni, repmgr, or pg_auto_failover
3. **Load Balancing** - PgBouncer or HAProxy to distribute read queries
4. **Monitoring** - pg_stat_replication, pgBadger, Prometheus/Grafana
