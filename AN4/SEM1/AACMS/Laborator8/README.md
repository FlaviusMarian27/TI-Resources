# Drupal High Availability (lab 8)

Setup cu **3 instanțe Drupal** în spatele unui **load balancer nginx**, cu o bază de date PostgreSQL comună. Tema (Bootstrap5) e instalată într-un folder comun, deci o văd toate nodurile. Totul rulează local, cu Docker Compose.

## Arhitectura

```
                      http://localhost:8080
                               |
                        +-------------+
                        |  nginx (lb) |   round-robin
                        +------+------+
              +----------------+----------------+
              |                |                |
         +----+----+      +----+----+      +----+----+
         | drupal1 |      | drupal2 |      | drupal3 |
         | (:8081) |      |         |      |         |
         +----+----+      +----+----+      +----+----+
              |                |                |
              +-------+--------+--------+-------+
                      |                 |
              +-------+-------+   +-----+------------------+
              | PostgreSQL    |   | volume comune:         |
              | (db)          |   |  - sites/default       |
              +---------------+   |  - ./themes            |
                                  +------------------------+
```

| Serviciu | Rol | Port pe host |
|----------|-----|--------------|
| `db` | PostgreSQL 16, baza de date comună | - |
| `drupal1` | Instanță Drupal 10 (Apache). Port expus doar pentru instalare | 8081 |
| `drupal2` | Instanță Drupal 10 (Apache) | - |
| `drupal3` | Instanță Drupal 10 (Apache) | - |
| `lb` | nginx, împarte cererile între cele 3 noduri | 8080 |

## Structura folderului

```
drupal-ha/
├── docker-compose.yml
├── nginx.conf
├── themes/            # folder comun montat în toate nodurile
│   └── contrib/bootstrap5/
└── README.md
```

## Cum asigură high availability

1. **3 instanțe identice.** Toate sunt construite din aceeași imagine (`drupal:10-apache`), deci oricare poate servi orice cerere.
2. **Load balancer.** nginx are un bloc `upstream` cu cele 3 noduri și împarte cererile între ele (round-robin). Dacă un nod nu răspunde, `proxy_next_upstream` retrimite cererea la alt nod.
3. **Stare comună**, ca nodurile să fie interschimbabile:
   - **Baza de date** e una singură. Conținutul, configurația, sesiunile și cache-ul sunt în ea. Un utilizator logat rămâne logat chiar dacă cererea următoare ajunge pe alt nod, deci nu e nevoie de sticky sessions.
   - **`sites/default`** e un volum Docker comun (`settings.php` și fișierele încărcate), deci toate nodurile folosesc aceeași conexiune la DB.
   - **`./themes`** e un folder de pe host montat în toate nodurile. O temă instalată sau modificată apare imediat peste tot.
4. **Header de debug.** nginx adaugă `X-Served-By` cu IP-ul nodului care a răspuns, ca să vedem balansarea.

### Limite

E HA doar la nivel de aplicație. `lb`, `db` și volumele rămân single point of failure. Un HA complet ar cere încă un load balancer cu IP virtual (keepalived), replicare la Postgres și storage partajat real (NFS sau similar).

## Fișiere de configurare

### docker-compose.yml

```yaml
services:
  db:
    image: postgres:16
    environment:
      POSTGRES_DB: drupal
      POSTGRES_USER: drupal
      POSTGRES_PASSWORD: drupal
    volumes:
      - dbdata:/var/lib/postgresql/data

  drupal1:
    image: drupal:10-apache
    depends_on:
      - db
    ports:
      - "8081:80"
    volumes:
      - sitesdefault:/var/www/html/sites/default
      - ./themes:/var/www/html/themes

  drupal2:
    image: drupal:10-apache
    depends_on:
      - db
    volumes:
      - sitesdefault:/var/www/html/sites/default
      - ./themes:/var/www/html/themes

  drupal3:
    image: drupal:10-apache
    depends_on:
      - db
    volumes:
      - sitesdefault:/var/www/html/sites/default
      - ./themes:/var/www/html/themes

  lb:
    image: nginx:alpine
    depends_on:
      - drupal1
      - drupal2
      - drupal3
    ports:
      - "8080:80"
    volumes:
      - ./nginx.conf:/etc/nginx/conf.d/default.conf:ro

volumes:
  dbdata:
  sitesdefault:
```

### nginx.conf

```nginx
upstream drupal {
  zone drupal 64k;
  server drupal1:80;
  server drupal2:80;
  server drupal3:80;
}

server {
  listen 80;

  location / {
    proxy_pass http://drupal;
    proxy_set_header Host $http_host;
    proxy_set_header X-Forwarded-For $remote_addr;
    proxy_next_upstream error timeout http_502 http_503;
    add_header X-Served-By $upstream_addr always;
  }
}
```

Explicații:

- `zone drupal 64k;` face ca workerii nginx să împartă același contor round-robin. Fără el, fiecare worker are contorul lui și distribuția iese strâmbă.
- `Host $http_host` păstrează portul din cerere (`localhost:8080`). Cu `$host`, portul se pierdea, iar redirecturile Drupal duceau la `http://localhost/` (portul 80) și apărea "connection refused".
- `proxy_next_upstream` retrimite cererea la alt nod dacă cel ales dă eroare sau nu răspunde.

## Pași de instalare

### 1. Pornire

```bash
mkdir -p drupal-ha/themes && cd drupal-ha
# pune aici docker-compose.yml și nginx.conf
docker compose up -d
docker compose ps        # trebuie să vezi 5 containere Up
```

### 2. Instalare Drupal (o singură dată, pe drupal1)

Se face pe **http://localhost:8081**, direct pe drupal1, ca wizardul să nu sară între noduri.

- limba, apoi profilul **Standard**
- tip bază de date: **PostgreSQL**
- nume bază de date, utilizator, parolă: `drupal`
- *Opțiuni avansate* → host: `db`, portul 5432, prefixul de tabelă gol
- completezi datele site-ului și contul de administrator

### 3. Verificare balansare

```bash
for i in $(seq 1 30); do curl -s -o /dev/null -D - localhost:8080 | grep -i x-served-by; done | sort | uniq -c
```

Rezultat așteptat: câte 10 cereri pe fiecare dintre cele 3 IP-uri.

IP-urile containerelor:

```bash
docker inspect -f '{{.Name}} {{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' $(docker compose ps -q)
```

### 4. Instalare temă (Bootstrap5)

```bash
docker compose exec drupal3 composer require drupal/bootstrap5
ls themes/contrib
```

Composer rulează într-un singur container, dar scrie în `/var/www/html/themes`, care e folderul comun `./themes`. Toate nodurile văd tema.

Apoi, în browser: **http://localhost:8080/admin/appearance** → Bootstrap5 → **Instalează și setează ca implicită**.

### 5. Test cu noduri oprite

```bash
docker compose stop drupal1 drupal2
for i in $(seq 1 10); do curl -s -o /dev/null -w "%{http_code} " localhost:8080; done; echo
```

Rezultat așteptat: `200` de 10 ori. Site-ul merge, servit de drupal3.

Pornire la loc:

```bash
docker compose start drupal1 drupal2
```

Nu da `restart` la `lb` cât timp unele noduri sunt oprite, pentru că nginx nu pornește dacă nu poate rezolva un upstream.

### 6. Modificarea temei

Adaug o regulă CSS (fundal roz deschis). Fișierele create de composer aparțin lui root, deci e nevoie de `sudo`:

```bash
echo 'body { background-color: #ffe4e1 !important; }' | sudo tee -a themes/contrib/bootstrap5/css/style.css
```

Apoi se golește cache-ul: **http://localhost:8080/admin/config/development/performance** → **Clear all caches**. Cache-ul e în baza de date comună, deci o singură golire ajunge pentru toate nodurile. Dai un refresh pe **http://localhost:8080** și fundalul e schimbat.

Verificare că modificarea e pe fiecare nod:

```bash
docker compose exec drupal2 grep -c ffe4e1 /var/www/html/themes/contrib/bootstrap5/css/style.css
docker compose exec drupal3 grep -c ffe4e1 /var/www/html/themes/contrib/bootstrap5/css/style.css
```

Rezultat așteptat: `1` de la fiecare. Dacă fundalul nu se schimbă, adaugă aceeași linie și în `css/style-52.css`, apoi golește din nou cache-ul și fă hard refresh (`Ctrl+Shift+R`).

## Probleme întâlnite

| Problemă | Cauză | Soluție |
|----------|-------|---------|
| Toate cererile ajung pe același nod | Fiecare worker nginx are propriul contor round-robin | `zone drupal 64k;` în `upstream` |
| "localhost refused to connect" după setarea temei | `Host $host` taie portul, redirectul merge la portul 80 | `proxy_set_header Host $http_host;` |
| nginx nu pornește după restart | Un nod oprit nu poate fi rezolvat de DNS-ul Docker | Pornești întâi toate nodurile, apoi `lb` |
| `grep -c bootstrap5` dă un număr (ex. 16) | Numără liniile din HTML care conțin cuvântul | Contează să fie mai mare ca 0; mai clar e codul HTTP `200` |

## Comenzi utile

```bash
docker compose ps                       # starea containerelor
docker compose logs -f lb               # log-urile load balancer-ului
docker compose stop drupal1 drupal2     # oprește noduri
docker compose start drupal1 drupal2    # le pornește la loc
docker compose down                     # oprește și șterge containerele (datele rămân în volume)
docker compose down -v                  # șterge și volumele (reset complet)
```