#!/bin/bash
# ============================================================
# POSTGRESQL REPLICATION TEST SCRIPT
# ============================================================
# This script tests that replication is working correctly by:
# 1. Checking replication status on primary
# 2. Creating test data on primary
# 3. Verifying data appears on all replicas
# 4. Testing that replicas are read-only
#
# Uses docker exec to run psql inside containers (no local psql needed)
# ============================================================

# Don't exit on first error - we want to show all results
set +e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Container names
PRIMARY="pg_primary"
REPLICA1="pg_replica1"
REPLICA2="pg_replica2"
REPLICA3="pg_replica3"

# Database connection
PGUSER="postgres"
PGDATABASE="testdb"

# Helper function to run psql on primary
psql_primary() {
    docker exec $PRIMARY psql -U $PGUSER -d $PGDATABASE "$@"
}

# Helper function to run psql on a replica
psql_replica() {
    local container=$1
    shift
    docker exec $container psql -U $PGUSER -d $PGDATABASE "$@"
}

echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}    PostgreSQL Replication Test Suite${NC}"
echo -e "${BLUE}============================================================${NC}"
echo ""

# ------------------------------------------------------------
# STEP 0: Wait for cluster to be ready
# ------------------------------------------------------------
echo -e "${YELLOW}Waiting for cluster to be fully ready...${NC}"
echo "----------------------------------------"

# Check if containers are running
echo "Checking containers..."
for container in $PRIMARY $REPLICA1 $REPLICA2 $REPLICA3; do
    if docker ps --format '{{.Names}}' | grep -q "^${container}$"; then
        echo -e "  ${GREEN}✓ $container is running${NC}"
    else
        echo -e "  ${RED}✗ $container is NOT running${NC}"
        echo "  Start the cluster first: ./manage.sh start"
        exit 1
    fi
done
echo ""

# Wait for primary to be ready and NOT in recovery
echo "Waiting for primary server to be ready..."
MAX_ATTEMPTS=30
ATTEMPT=0
while [ $ATTEMPT -lt $MAX_ATTEMPTS ]; do
    IS_READY=$(docker exec $PRIMARY psql -U $PGUSER -d $PGDATABASE -t -c "SELECT pg_is_in_recovery();" 2>/dev/null | tr -d ' \n\r')
    
    if [ "$IS_READY" = "f" ]; then
        echo -e "${GREEN}✓ Primary is ready and accepting writes${NC}"
        break
    fi
    
    ATTEMPT=$((ATTEMPT + 1))
    echo "  Attempt $ATTEMPT/$MAX_ATTEMPTS - waiting... (status: '$IS_READY')"
    sleep 2
done

if [ $ATTEMPT -eq $MAX_ATTEMPTS ]; then
    echo -e "${RED}✗ Primary did not become ready in time${NC}"
    echo "  Check logs: docker logs $PRIMARY"
    exit 1
fi

# Wait for replicas to connect
echo ""
echo "Waiting for replicas to connect to primary..."
ATTEMPT=0
while [ $ATTEMPT -lt $MAX_ATTEMPTS ]; do
    REPLICA_COUNT=$(docker exec $PRIMARY psql -U $PGUSER -d $PGDATABASE -t -c "SELECT count(*) FROM pg_stat_replication;" 2>/dev/null | tr -d ' \n\r')
    
    if [ "$REPLICA_COUNT" -ge 3 ] 2>/dev/null; then
        echo -e "${GREEN}✓ All 3 replicas connected${NC}"
        break
    fi
    
    ATTEMPT=$((ATTEMPT + 1))
    echo "  Attempt $ATTEMPT/$MAX_ATTEMPTS - $REPLICA_COUNT/3 replicas connected..."
    sleep 3
done

if [ $ATTEMPT -eq $MAX_ATTEMPTS ]; then
    echo -e "${YELLOW}! Warning: Not all replicas connected (got $REPLICA_COUNT), continuing anyway...${NC}"
fi

echo ""

# ------------------------------------------------------------
# TEST 1: Check Primary Status
# ------------------------------------------------------------
echo -e "${YELLOW}TEST 1: Checking Primary Server Status${NC}"
echo "----------------------------------------"

echo "Primary server info:"
psql_primary -c "SELECT version();" 2>/dev/null | head -3

echo ""
echo "Verifying primary is NOT in recovery mode:"
IS_REPLICA=$(docker exec $PRIMARY psql -U $PGUSER -d $PGDATABASE -t -c "SELECT pg_is_in_recovery();" 2>/dev/null | tr -d ' \n\r')

if [ "$IS_REPLICA" = "f" ]; then
    echo -e "${GREEN}✓ Primary is NOT in recovery mode (correct!)${NC}"
else
    echo -e "${RED}✗ Primary reports it IS in recovery (got: '$IS_REPLICA')${NC}"
    exit 1
fi

echo ""

# ------------------------------------------------------------
# TEST 2: Check Replication Connections
# ------------------------------------------------------------
echo -e "${YELLOW}TEST 2: Checking Replication Connections${NC}"
echo "----------------------------------------"

echo "Querying pg_stat_replication on primary:"
echo ""
psql_primary -c "
SELECT 
    client_addr,
    state,
    sent_lsn,
    replay_lsn,
    sync_state
FROM pg_stat_replication;
" 2>/dev/null

REPLICA_COUNT=$(docker exec $PRIMARY psql -U $PGUSER -d $PGDATABASE -t -c "SELECT count(*) FROM pg_stat_replication;" 2>/dev/null | tr -d ' \n\r')

if [ "$REPLICA_COUNT" -ge 3 ] 2>/dev/null; then
    echo -e "${GREEN}✓ Found $REPLICA_COUNT connected replicas${NC}"
else
    echo -e "${YELLOW}! Expected 3 replicas, found $REPLICA_COUNT${NC}"
fi

echo ""

# ------------------------------------------------------------
# TEST 3: Check Replica Status
# ------------------------------------------------------------
echo -e "${YELLOW}TEST 3: Checking Replica Status${NC}"
echo "----------------------------------------"

for container in $REPLICA1 $REPLICA2 $REPLICA3; do
    IS_STANDBY=$(docker exec $container psql -U $PGUSER -d $PGDATABASE -t -c "SELECT pg_is_in_recovery();" 2>/dev/null | tr -d ' \n\r')
    
    if [ "$IS_STANDBY" = "t" ]; then
        echo -e "${GREEN}✓ $container is in recovery mode (standby) - correct!${NC}"
    else
        echo -e "${RED}✗ $container is NOT in recovery mode (got: '$IS_STANDBY')${NC}"
    fi
done

echo ""

# ------------------------------------------------------------
# TEST 4: Create Test Table and Data on Primary
# ------------------------------------------------------------
echo -e "${YELLOW}TEST 4: Creating Test Data on Primary${NC}"
echo "----------------------------------------"

# Create test table
psql_primary -c "
DROP TABLE IF EXISTS replication_test;
CREATE TABLE replication_test (
    id SERIAL PRIMARY KEY,
    message TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
" 2>/dev/null

echo "Created table 'replication_test'"

# Insert test data
psql_primary -c "
INSERT INTO replication_test (message) VALUES 
    ('Hello from primary!'),
    ('Replication test message 1'),
    ('Replication test message 2'),
    ('Test timestamp: $(date)');
" 2>/dev/null

echo "Inserted 4 test rows"
echo -e "${GREEN}✓ Test data created on primary${NC}"

echo ""

# Wait for replication to catch up
echo "Waiting 2 seconds for replication..."
sleep 2

# ------------------------------------------------------------
# TEST 5: Verify Data on Replicas
# ------------------------------------------------------------
echo -e "${YELLOW}TEST 5: Verifying Data on Replicas${NC}"
echo "----------------------------------------"

PRIMARY_COUNT=$(docker exec $PRIMARY psql -U $PGUSER -d $PGDATABASE -t -c "SELECT count(*) FROM replication_test;" 2>/dev/null | tr -d ' \n\r')
echo "Primary has $PRIMARY_COUNT rows"

for container in $REPLICA1 $REPLICA2 $REPLICA3; do
    REPLICA_ROW_COUNT=$(docker exec $container psql -U $PGUSER -d $PGDATABASE -t -c "SELECT count(*) FROM replication_test;" 2>/dev/null | tr -d ' \n\r')
    
    if [ "$REPLICA_ROW_COUNT" = "$PRIMARY_COUNT" ]; then
        echo -e "${GREEN}✓ $container has $REPLICA_ROW_COUNT rows (matches primary)${NC}"
    else
        echo -e "${RED}✗ $container has $REPLICA_ROW_COUNT rows (expected $PRIMARY_COUNT)${NC}"
    fi
done

echo ""

# Show data from one replica
echo "Data from $REPLICA1:"
psql_replica $REPLICA1 -c "SELECT * FROM replication_test;" 2>/dev/null

echo ""

# ------------------------------------------------------------
# TEST 6: Test Read-Only on Replicas
# ------------------------------------------------------------
echo -e "${YELLOW}TEST 6: Testing Read-Only Mode on Replicas${NC}"
echo "----------------------------------------"

for container in $REPLICA1 $REPLICA2 $REPLICA3; do
    # Try to insert (should fail)
    RESULT=$(docker exec $container psql -U $PGUSER -d $PGDATABASE -c "INSERT INTO replication_test (message) VALUES ('This should fail');" 2>&1)
    
    if echo "$RESULT" | grep -qi "read-only"; then
        echo -e "${GREEN}✓ $container correctly rejected write operation${NC}"
    else
        echo -e "${RED}✗ $container may have accepted write (unexpected!)${NC}"
        echo "  Result: $RESULT"
    fi
done

echo ""

# ------------------------------------------------------------
# TEST 7: Test Real-Time Replication
# ------------------------------------------------------------
echo -e "${YELLOW}TEST 7: Testing Real-Time Replication${NC}"
echo "----------------------------------------"

TIMESTAMP=$(date +%H:%M:%S)
echo "Inserting new row on primary with timestamp: $TIMESTAMP"

psql_primary -c "INSERT INTO replication_test (message) VALUES ('Real-time test at $TIMESTAMP');" 2>/dev/null

sleep 1

echo "Checking replicas immediately..."
for container in $REPLICA1 $REPLICA2 $REPLICA3; do
    LATEST=$(docker exec $container psql -U $PGUSER -d $PGDATABASE -t -c "SELECT message FROM replication_test ORDER BY id DESC LIMIT 1;" 2>/dev/null | tr -d '\n\r' | xargs)
    
    if echo "$LATEST" | grep -q "$TIMESTAMP"; then
        echo -e "${GREEN}✓ $container received new data${NC}"
    else
        echo -e "${YELLOW}! $container may have slight delay (got: '$LATEST')${NC}"
    fi
done

echo ""

# ------------------------------------------------------------
# TEST 8: Replication Lag Check
# ------------------------------------------------------------
echo -e "${YELLOW}TEST 8: Checking Replication Lag${NC}"
echo "----------------------------------------"

psql_primary -c "
SELECT 
    client_addr,
    state,
    pg_wal_lsn_diff(sent_lsn, replay_lsn) AS lag_bytes,
    pg_size_pretty(pg_wal_lsn_diff(sent_lsn, replay_lsn)) AS lag_pretty
FROM pg_stat_replication;
" 2>/dev/null

echo ""

# ------------------------------------------------------------
# SUMMARY
# ------------------------------------------------------------
echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}    Test Summary${NC}"
echo -e "${BLUE}============================================================${NC}"
echo ""
echo "Containers:"
echo "  Primary:   $PRIMARY (port 5432 on host)"
echo "  Replica 1: $REPLICA1 (port 5433 on host)"
echo "  Replica 2: $REPLICA2 (port 5434 on host)"
echo "  Replica 3: $REPLICA3 (port 5435 on host)"
echo ""
echo "Connect from host (if psql installed):"
echo "  Primary:  psql -h localhost -p 5432 -U postgres -d testdb"
echo "  Replica:  psql -h localhost -p 5433 -U postgres -d testdb"
echo ""
echo "Connect via docker:"
echo "  Primary:  docker exec -it $PRIMARY psql -U postgres -d testdb"
echo "  Replica:  docker exec -it $REPLICA1 psql -U postgres -d testdb"
echo ""
echo -e "${GREEN}All tests completed!${NC}"
