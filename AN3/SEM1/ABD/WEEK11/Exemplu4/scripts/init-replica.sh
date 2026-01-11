#!/bin/bash
# ============================================================
# REPLICA SERVER INITIALIZATION SCRIPT
# ============================================================
# This script initializes a replica by:
# 1. Waiting for the primary to be ready
# 2. Using pg_basebackup to copy data from primary
# 3. Configuring the replica to stream from primary
# 4. Starting PostgreSQL in standby mode
#
# pg_basebackup creates a complete copy of the primary's data
# directory, which is the foundation for streaming replication.
#
# IMPORTANT: PostgreSQL cannot run as root, so we use gosu/su
# to run commands as the 'postgres' user.
# ============================================================

set -e

# Data directory
PGDATA="/var/lib/postgresql/data"

echo "============================================================"
echo "Initializing Replica Server"
echo "Primary: ${PRIMARY_HOST}:${PRIMARY_PORT}"
echo "============================================================"

# ------------------------------------------------------------
# STEP 1: Check if already initialized
# ------------------------------------------------------------
if [ -f "$PGDATA/PG_VERSION" ]; then
    echo "Data directory already exists, starting PostgreSQL..."
    # Switch to postgres user and start
    exec gosu postgres postgres -c config_file=/etc/postgresql/postgresql.conf -c hba_file=/etc/postgresql/pg_hba.conf
fi

# ------------------------------------------------------------
# STEP 2: Wait for primary to be ready
# ------------------------------------------------------------
echo "Waiting for primary server at ${PRIMARY_HOST}:${PRIMARY_PORT}..."

# Create .pgpass file for postgres user
mkdir -p /var/lib/postgresql
echo "${PRIMARY_HOST}:${PRIMARY_PORT}:*:${REPLICATION_USER}:${REPLICATION_PASSWORD}" > /var/lib/postgresql/.pgpass
chmod 600 /var/lib/postgresql/.pgpass
chown postgres:postgres /var/lib/postgresql/.pgpass

# Wait until primary accepts connections
until PGPASSWORD="${REPLICATION_PASSWORD}" pg_isready -h "${PRIMARY_HOST}" -p "${PRIMARY_PORT}" -U "${REPLICATION_USER}"
do
    echo "Primary not ready yet, waiting 2 seconds..."
    sleep 2
done

echo "Primary is ready!"

# Extra wait to ensure primary is fully initialized
sleep 5

# ------------------------------------------------------------
# STEP 3: Prepare data directory
# ------------------------------------------------------------
# Ensure data directory exists and has correct ownership
mkdir -p "$PGDATA"
chown postgres:postgres "$PGDATA"
chmod 700 "$PGDATA"

# ------------------------------------------------------------
# STEP 4: Run pg_basebackup as postgres user
# ------------------------------------------------------------
# pg_basebackup creates a consistent snapshot of the primary
# Options:
#   -h: Primary hostname
#   -p: Primary port
#   -U: Replication user
#   -D: Destination directory
#   -Fp: Format = plain (directory structure)
#   -Xs: WAL method = stream (include WAL during backup)
#   -P: Show progress
#   -R: Create standby.signal and configure recovery
#   -v: Verbose output
# ------------------------------------------------------------

echo "Running pg_basebackup to clone primary..."
echo "This may take a while depending on database size..."

# Run pg_basebackup as postgres user
gosu postgres bash -c "
    export PGPASSWORD='${REPLICATION_PASSWORD}'
    pg_basebackup \
        -h '${PRIMARY_HOST}' \
        -p '${PRIMARY_PORT}' \
        -U '${REPLICATION_USER}' \
        -D '${PGDATA}' \
        -Fp \
        -Xs \
        -P \
        -R \
        -v
"

echo "pg_basebackup completed!"

# ------------------------------------------------------------
# STEP 5: Verify standby configuration
# ------------------------------------------------------------
# pg_basebackup with -R flag creates:
# - standby.signal: Empty file that tells PostgreSQL this is a standby
# - postgresql.auto.conf: Contains primary_conninfo

echo "Checking standby configuration..."

if [ -f "$PGDATA/standby.signal" ]; then
    echo "✓ standby.signal exists"
else
    echo "Creating standby.signal..."
    gosu postgres touch "$PGDATA/standby.signal"
fi

# Show the auto-generated connection info
echo "Primary connection info (from postgresql.auto.conf):"
cat "$PGDATA/postgresql.auto.conf"

# ------------------------------------------------------------
# STEP 6: Ensure correct permissions
# ------------------------------------------------------------
chown -R postgres:postgres "$PGDATA"
chmod 700 "$PGDATA"

# ------------------------------------------------------------
# STEP 7: Start PostgreSQL as postgres user
# ------------------------------------------------------------
echo "============================================================"
echo "Starting PostgreSQL in standby mode..."
echo "============================================================"

exec gosu postgres postgres -c config_file=/etc/postgresql/postgresql.conf -c hba_file=/etc/postgresql/pg_hba.conf
