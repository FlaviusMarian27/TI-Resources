# AACMS Lab 6: Drupal High Availability (Docker Compose)

Drupal 10 rulat în Docker Compose pe Linux Mint, cu high availability pe două straturi:

- **Web**: 2 instanțe Drupal în spatele unui load balancer Nginx
- **Baza de date**: PostgreSQL primary + replică (streaming replication), cu HAProxy în față

## Structura

```
Browser / curl  (localhost:8080)
        │
        ▼
┌──────────────────┐
│  lb (Nginx)      │  round-robin, retry pe celălalt server
└──────┬───────────┘
   ┌───┴────┐
   ▼        ▼
┌────────┐ ┌────────┐
│drupal1 │ │drupal2 │   volum partajat: drupal_sites
└───┬────┘ └───┬────┘
    └────┬─────┘
         ▼
┌──────────────────┐
│  db (HAProxy)    │  endpoint unic pentru Drupal: db:5432
└──────┬───────────┘
   ┌───┴──────────────┐
   ▼                  ▼
┌────────────┐   ┌────────────┐
│ db-primary │──▶│ db-replica │
│ read/write │   │ standby    │
└────────────┘   │ ("backup") │
 streaming repl. └────────────┘
```

| Serviciu | Rol | Dacă cade |
|---|---|---|
| `lb` | Nginx, distribuie cererile | Site inaccesibil (punct unic de cădere) |
| `drupal1`, `drupal2` | Aplicația | Nginx trimite totul la celălalt |
| `db` | HAProxy, host unic pentru Drupal | DB inaccesibil (punct unic de cădere) |
| `db-primary` | Postgres principal | HAProxy trece pe replică, promovare manuală |
| `db-replica` | Postgres standby | Nu afectează site-ul |

**Ce e HA și ce nu:** web = HA real; DB = replicare reală, dar failover **manual** (`pg_ctl promote`); `lb` și `db` rămân puncte unice de cădere.

---

## Pasul 1: Pregătire

Folder de lucru:

```bash
mkdir -p ~/Documents/AACMS/lab6 && cd ~/Documents/AACMS/lab6
```

Verifici Docker și Compose:

```bash
docker --version
docker compose version
docker ps
```

Dacă `docker ps` dă `permission denied`:

```bash
sudo usermod -aG docker $USER
```

apoi logout/login.

(Instalare Docker pe Linux Mint, dacă lipsește: repo-ul oficial Docker pentru Ubuntu, folosind `UBUNTU_CODENAME` din `/etc/os-release`, apoi pachetele `docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin`.)

## Pasul 2: Varianta simplă (Drupal + PostgreSQL)

Primul test, un singur Drupal cu un singur Postgres. Imagini: `drupal:10-apache` și `postgres:16`. Installer-ul Drupal pe `http://localhost:8080` cu:

- Profil: Standard
- Bază de date: PostgreSQL
- Nume `drupal`, utilizator `drupal`, parolă `drupalpass`
- Opțiuni avansate: **Host = `db`**, Port `5432`

Date fictive pentru site: nume `AACMS Lab6`, admin `admin` / `admin123`, email `admin@aacms.local`.

Această variantă a fost ștearsă ca să facem varianta HA: `docker compose down -v`.

## Pasul 3: Fișierele pentru varianta HA

Creezi în `lab6` cinci fișiere.

### `docker-compose.yml`

```yaml
x-drupal: &drupal
  image: drupal:10-apache
  restart: unless-stopped
  depends_on:
    - db
  volumes:
    - drupal_sites:/var/www/html/sites

services:
  db-primary:
    image: postgres:16
    restart: unless-stopped
    environment:
      POSTGRES_DB: drupal
      POSTGRES_USER: drupal
      POSTGRES_PASSWORD: drupalpass
    volumes:
      - primary_data:/var/lib/postgresql/data
      - ./init-primary.sh:/docker-entrypoint-initdb.d/init-primary.sh:ro
    healthcheck:
      test: ["CMD-SHELL", "pg_isready -h 127.0.0.1 -U drupal -d drupal"]
      interval: 5s
      timeout: 3s
      retries: 10

  db-replica:
    image: postgres:16
    restart: unless-stopped
    user: postgres
    entrypoint: ["bash", "/replica.sh"]
    environment:
      POSTGRES_DB: drupal
      POSTGRES_USER: drupal
      POSTGRES_PASSWORD: drupalpass
    volumes:
      - replica_data:/var/lib/postgresql/data
      - ./replica.sh:/replica.sh:ro
    depends_on:
      db-primary:
        condition: service_healthy

  db:
    image: haproxy:3.0
    restart: unless-stopped
    volumes:
      - ./haproxy.cfg:/usr/local/etc/haproxy/haproxy.cfg:ro
    depends_on:
      - db-primary
      - db-replica

  drupal1:
    <<: *drupal
  drupal2:
    <<: *drupal

  lb:
    image: nginx:stable
    restart: unless-stopped
    ports:
      - "8080:80"
    volumes:
      - ./nginx.conf:/etc/nginx/conf.d/default.conf:ro
    depends_on:
      - drupal1
      - drupal2

volumes:
  primary_data:
  replica_data:
  drupal_sites:
```

### `init-primary.sh`

Creează userul de replicare și extensia `pg_trgm`.

```bash
#!/bin/bash
set -e
psql -v ON_ERROR_STOP=1 -U "$POSTGRES_USER" -d "$POSTGRES_DB" <<-EOSQL
  CREATE ROLE replicator WITH REPLICATION LOGIN PASSWORD 'replpass';
  CREATE EXTENSION IF NOT EXISTS pg_trgm;
EOSQL
echo "host replication replicator all scram-sha-256" >> "$PGDATA/pg_hba.conf"
```

### `replica.sh`

Copiază baza de la primary (`pg_basebackup`) și pornește ca standby.

```bash
#!/bin/bash
set -e
if [ -z "$(ls -A "$PGDATA" 2>/dev/null)" ]; then
  until pg_isready -h db-primary -U replicator; do sleep 2; done
  PGPASSWORD=replpass pg_basebackup -h db-primary -U replicator -D "$PGDATA" -Fp -Xs -R -P
  chmod 700 "$PGDATA"
fi
exec docker-entrypoint.sh postgres
```

### `haproxy.cfg`

Primary activ, replica doar `backup`. **Fișierul trebuie să se termine cu newline**, altfel HAProxy dă `Missing LF on last line` și intră în restart loop.

```
global
    maxconn 100

defaults
    mode tcp
    timeout connect 5s
    timeout client 30m
    timeout server 30m

frontend pg
    bind *:5432
    default_backend pg_nodes

backend pg_nodes
    option pgsql-check user drupal
    server primary db-primary:5432 check inter 2s fall 2 rise 2
    server replica db-replica:5432 check inter 2s fall 2 rise 2 backup
```

### `nginx.conf`

Round-robin, retry pe celălalt server, header `X-Backend` care arată cine a răspuns.

```nginx
upstream drupal_pool {
    server drupal1:80 max_fails=1 fail_timeout=5s;
    server drupal2:80 max_fails=1 fail_timeout=5s;
}

server {
    listen 80;

    location / {
        proxy_pass http://drupal_pool;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_next_upstream error timeout http_502 http_503;
        proxy_connect_timeout 2s;
        add_header X-Backend $upstream_addr always;
    }
}
```

`proxy_connect_timeout 2s` a fost adăugat ca Nginx să ocolească rapid un backend oprit (fără el, un curl a stat blocat la oprirea lui `drupal1`).

## Pasul 4: Pornire

```bash
docker compose up -d
docker compose ps
```

Folosește `-d`. Fără el terminalul rămâne atașat la log-uri, iar `Ctrl+C` oprește containerele.

Verifici că replica e conectată:

```bash
docker compose exec db-primary psql -U drupal -c "select client_addr, state from pg_stat_replication;"
```

Trebuie să apară o linie cu `streaming`.

## Pasul 5: Instalarea Drupal

Cât timp rulează installer-ul, oprești o instanță (altfel Nginx alternează între ele și sesiunea se pierde):

```bash
docker compose stop drupal2
```

Deschizi `http://localhost:8080`:

- Profil: Standard
- Bază de date: PostgreSQL
- Nume `drupal`, utilizator `drupal`, parolă `drupalpass`
- Opțiuni avansate: **Host = `db`** (HAProxy, nu Postgres direct), Port `5432`

La final, repornești:

```bash
docker compose start drupal2
```

## Pasul 6: Teste

Ordinea contează: failover-ul pe DB se face **ultimul**.

### 6.1 Load balancing

```bash
for i in 1 2 3 4 5 6; do curl -sI localhost:8080 | grep X-Backend; done
```

Trebuie să alterneze două IP-uri. Maparea IP la container:

```bash
docker inspect -f '{{.Name}} {{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' lab6-drupal1-1 lab6-drupal2-1
```

### 6.2 Failover web

```bash
docker compose stop drupal1
for i in 1 2 3 4; do curl -sI localhost:8080 | grep X-Backend; done
docker compose start drupal1
```

Trebuie să răspundă doar `drupal2`, fără întrerupere.

### 6.3 Replicarea funcționează

```bash
docker compose exec db-primary psql -U drupal -c "create table test_repl(x int); insert into test_repl values (1);"
docker compose exec db-replica psql -U drupal -c "select * from test_repl;"
docker compose exec db-primary psql -U drupal -c "drop table test_repl;"
```

### 6.4 Failover DB (manual)

```bash
docker compose stop db-primary
docker compose exec -u postgres db-replica pg_ctl promote -D /var/lib/postgresql/data
docker compose exec db-replica psql -U drupal -c "select pg_is_in_recovery();"
```

`f` înseamnă că replica a fost promovată și acum e primary. HAProxy trimite deja traficul spre ea (era `backup`). Reîncarcă site-ul și verifică că merge.

După promote, replica nu mai e replică. Ca să revii la structura inițială: `docker compose down -v` și `docker compose up -d`, apoi reinstalezi Drupal.

## Interogare din terminal

Cod HTTP și timp de răspuns:

```bash
curl -s -o /dev/null -w "%{http_code} %{time_total}s\n" localhost:8080
```

Monitorizare continuă (utilă când oprești containere):

```bash
watch -n1 'curl -s -o /dev/null -m 3 -w "%{http_code} " localhost:8080; curl -sI -m 3 localhost:8080 | grep X-Backend'
```

Log-uri Nginx live: `docker compose logs -f lb`

La ce server Postgres ajunge Drupal (prin HAProxy):

```bash
docker compose exec -e PGPASSWORD=drupalpass db-primary psql -h db -U drupal -d drupal -c "select inet_server_addr(), pg_is_in_recovery();"
```

## Vizualizare în browser

- **DevTools**: F12, tab Network, prima cerere, Response Headers, `X-Backend`. La refresh se schimbă IP-ul.
- **Pagină stats HAProxy** (opțional). Adaugă la finalul `haproxy.cfg` (cu newline la final):

```
listen stats
    mode http
    bind *:8404
    stats enable
    stats uri /
    stats refresh 2s
```

și la serviciul `db` din compose: `ports: ["8404:8404"]`. Apoi `docker compose up -d --force-recreate db` și deschizi `http://localhost:8404`.

## Probleme întâlnite și rezolvări

| Problemă | Cauză | Rezolvare |
|---|---|---|
| Credențiale din installer nu merg | Postgres a fost creat cu alte date (`drupal`/`drupalpass`) | Folosești exact valorile din compose, sau le schimbi în compose și faci `down -v` |
| `lab6-db-1` în `Restarting` | `haproxy.cfg` fără newline la final (`Missing LF on last line`) | `echo "" >> haproxy.cfg`, apoi `docker compose up -d --force-recreate db` |
| `could not resolve address 'db-primary'` la validare cu `docker run` | Rulează în afara rețelei compose | Nu e o problemă reală, testezi direct în stack |
| `docker compose up` blochează terminalul | Lipsește `-d` | `Ctrl+C`, apoi `docker compose up -d` |
| Installer-ul pierde sesiunea | Nginx alternează între drupal1 și drupal2 | `docker compose stop drupal2` pe durata instalării |
| Curl blocat când `drupal1` e oprit | Fără timeout de conectare | `proxy_connect_timeout 2s;` în `nginx.conf`, `docker compose restart lb` |
| `302` la `/core/install.php` | Drupal nu e instalat încă | Faci installer-ul. Dacă primary-ul a fost oprit între timp, `down -v` și o iei de la zero |

## Oprire și ștergere

```bash
docker compose stop            # oprește, păstrează datele
docker compose start           # repornește
docker compose down            # șterge containerele, păstrează volumele
docker compose down -v         # șterge și datele (reinstalezi Drupal)
docker compose down -v --rmi all   # șterge și imaginile
```

Un singur container: `docker compose stop db-replica && docker compose rm -f db-replica`.