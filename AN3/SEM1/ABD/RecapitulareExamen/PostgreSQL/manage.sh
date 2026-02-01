#!/bin/bash
# ============================================================
# PostgreSQL Replication Cluster Management Script
# ============================================================
# Usage: ./manage.sh [command]
#
# Commands:
#   start     - Start all containers
#   stop      - Stop all containers
#   restart   - Restart all containers
#   status    - Show cluster status
#   logs      - Show logs from all containers
#   clean     - Remove all containers and volumes (fresh start)
#   primary   - Connect to primary with psql
#   replica1  - Connect to replica 1 with psql
#   replica2  - Connect to replica 2 with psql
#   replica3  - Connect to replica 3 with psql
#   test      - Run replication tests
# ============================================================

COMPOSE_FILE="docker-compose.yml"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

case "$1" in
    start)
        echo -e "${GREEN}Starting PostgreSQL Replication Cluster...${NC}"
        docker compose -f $COMPOSE_FILE up -d
        echo ""
        echo "Waiting for services to initialize..."
        sleep 10
        echo ""
        echo -e "${GREEN}Cluster started!${NC}"
        echo "Run './manage.sh status' to check cluster health"
        ;;
    
    stop)
        echo -e "${YELLOW}Stopping PostgreSQL Replication Cluster...${NC}"
        docker compose -f $COMPOSE_FILE down
        echo -e "${GREEN}Cluster stopped.${NC}"
        ;;
    
    restart)
        echo -e "${YELLOW}Restarting PostgreSQL Replication Cluster...${NC}"
        docker compose -f $COMPOSE_FILE down
        docker compose -f $COMPOSE_FILE up -d
        echo ""
        echo "Waiting for services to initialize..."
        sleep 10
        echo -e "${GREEN}Cluster restarted!${NC}"
        ;;
    
    status)
        echo -e "${BLUE}============================================================${NC}"
        echo -e "${BLUE}    PostgreSQL Replication Cluster Status${NC}"
        echo -e "${BLUE}============================================================${NC}"
        echo ""
        
        echo -e "${YELLOW}Container Status:${NC}"
        docker compose -f $COMPOSE_FILE ps
        echo ""
        
        echo -e "${YELLOW}Primary Server (pg_primary):${NC}"
        if docker exec pg_primary pg_isready -U postgres > /dev/null 2>&1; then
            echo -e "${GREEN}✓ Primary is ready${NC}"
            
            echo ""
            echo "Connected replicas:"
            docker exec pg_primary psql -U postgres -d testdb -c "
                SELECT 
                    client_addr,
                    state,
                    sync_state,
                    pg_size_pretty(pg_wal_lsn_diff(sent_lsn, replay_lsn)) AS lag
                FROM pg_stat_replication;
            " 2>/dev/null
        else
            echo -e "${RED}✗ Primary is not ready${NC}"
        fi
        
        echo ""
        echo -e "${YELLOW}Replica Status:${NC}"
        for replica in pg_replica1 pg_replica2 pg_replica3; do
            if docker exec $replica pg_isready -U postgres > /dev/null 2>&1; then
                IS_STANDBY=$(docker exec $replica psql -U postgres -d testdb -t -c "SELECT pg_is_in_recovery();" 2>/dev/null | tr -d ' ')
                if [ "$IS_STANDBY" = "t" ]; then
                    echo -e "${GREEN}✓ $replica: Running (standby mode)${NC}"
                else
                    echo -e "${YELLOW}! $replica: Running (not in standby mode)${NC}"
                fi
            else
                echo -e "${RED}✗ $replica: Not ready${NC}"
            fi
        done
        ;;
    
    logs)
        echo -e "${BLUE}Showing logs (Ctrl+C to exit)...${NC}"
        docker compose -f $COMPOSE_FILE logs -f
        ;;
    
    clean)
        echo -e "${RED}WARNING: This will delete all data!${NC}"
        read -p "Are you sure? (y/N) " confirm
        if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
            echo "Stopping and removing containers..."
            docker compose -f $COMPOSE_FILE down -v
            echo -e "${GREEN}Clean complete. Run './manage.sh start' to create fresh cluster.${NC}"
        else
            echo "Cancelled."
        fi
        ;;
    
    primary)
        echo -e "${BLUE}Connecting to Primary (pg_primary)...${NC}"
        docker exec -it pg_primary psql -U postgres -d testdb
        ;;
    
    replica1)
        echo -e "${BLUE}Connecting to Replica 1 (pg_replica1)...${NC}"
        echo -e "${YELLOW}Note: This is read-only!${NC}"
        docker exec -it pg_replica1 psql -U postgres -d testdb
        ;;
    
    replica2)
        echo -e "${BLUE}Connecting to Replica 2 (pg_replica2)...${NC}"
        echo -e "${YELLOW}Note: This is read-only!${NC}"
        docker exec -it pg_replica2 psql -U postgres -d testdb
        ;;
    
    replica3)
        echo -e "${BLUE}Connecting to Replica 3 (pg_replica3)...${NC}"
        echo -e "${YELLOW}Note: This is read-only!${NC}"
        docker exec -it pg_replica3 psql -U postgres -d testdb
        ;;
    
    test)
        echo -e "${BLUE}Running Replication Tests...${NC}"
        ./scripts/test-replication.sh
        ;;
    
    *)
        echo "PostgreSQL Replication Cluster Manager"
        echo ""
        echo "Usage: $0 {start|stop|restart|status|logs|clean|primary|replica1|replica2|replica3|test}"
        echo ""
        echo "Commands:"
        echo "  start     - Start all containers"
        echo "  stop      - Stop all containers"  
        echo "  restart   - Restart all containers"
        echo "  status    - Show cluster status"
        echo "  logs      - Show logs from all containers"
        echo "  clean     - Remove all containers and volumes"
        echo "  primary   - Connect to primary with psql"
        echo "  replica1  - Connect to replica 1 with psql"
        echo "  replica2  - Connect to replica 2 with psql"
        echo "  replica3  - Connect to replica 3 with psql"
        echo "  test      - Run replication tests"
        ;;
esac
