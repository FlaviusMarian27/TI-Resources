#!/bin/bash
# ============================================================
# PRIMARY SERVER INITIALIZATION SCRIPT
# ============================================================
# This script runs once when the primary database is first created.
# It creates the replication user that replicas will use to connect.
# 
# Docker runs scripts in /docker-entrypoint-initdb.d/ automatically
# after the database is initialized.
# ============================================================

set -e

echo "============================================================"
echo "Initializing Primary Server for Replication"
echo "============================================================"

# Create the replication user
# REPLICATION privilege allows streaming replication
# LOGIN privilege allows the user to connect
psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    -- Create replication user with replication privileges
    CREATE USER ${REPLICATION_USER:-replicator} 
        WITH REPLICATION 
        LOGIN 
        PASSWORD '${REPLICATION_PASSWORD:-replpassword}';
    
    -- Grant necessary privileges
    GRANT CONNECT ON DATABASE ${POSTGRES_DB} TO ${REPLICATION_USER:-replicator};
    
    -- Log success
    SELECT 'Replication user created successfully!' AS status;
EOSQL

# Create a replication slot for each replica (optional but recommended)
# Slots ensure WAL segments are retained until replicas receive them
psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    -- Create replication slots (optional)
    -- Replicas can use these to ensure they don't miss WAL data
    SELECT pg_create_physical_replication_slot('replica1_slot', true);
    SELECT pg_create_physical_replication_slot('replica2_slot', true);
    SELECT pg_create_physical_replication_slot('replica3_slot', true);
    
    SELECT 'Replication slots created!' AS status;
EOSQL

echo "============================================================"
echo "Primary initialization complete!"
echo "Replication user: ${REPLICATION_USER:-replicator}"
echo "============================================================"
