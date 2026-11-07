#!/bin/bash
set -e
if [ -z "$(ls -A "$PGDATA" 2>/dev/null)" ]; then
  until pg_isready -h db-primary -U replicator; do sleep 2; done
  PGPASSWORD=replpass pg_basebackup -h db-primary -U replicator -D "$PGDATA" -Fp -Xs -R -P
  chmod 700 "$PGDATA"
fi
exec docker-entrypoint.sh postgres