#!/bin/bash
set -e
psql -v ON_ERROR_STOP=1 -U "$POSTGRES_USER" -d "$POSTGRES_DB" <<-EOSQL
  CREATE ROLE replicator WITH REPLICATION LOGIN PASSWORD 'replpass';
  CREATE EXTENSION IF NOT EXISTS pg_trgm;
EOSQL
echo "host replication replicator all scram-sha-256" >> "$PGDATA/pg_hba.conf"