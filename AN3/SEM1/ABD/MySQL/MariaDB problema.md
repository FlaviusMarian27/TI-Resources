
**1.** Descărcați un dataset de pe  
[https://developer.imdb.com/non-commercial-datasets/](https://developer.imdb.com/non-commercial-datasets/) : **name.basics.tsv.gz**

**2.** Creați tabela corespunzătoare într-o bază de date **MySQL** și încărcați datele de mai sus.

**3.** Faceți un studiu de caz asupra căutării în această tabelă:
- căutare după nume care se termină cu „onnor”
- căutare după nume care conțin „alley”
    

- **Scopul exercițiului** este să reduceți timpul de căutare sub 1 secundă pentru exemplele de mai sus.
- Baza de date trebuie să fie replicată în **2 instanțe slave**.
- Se va nota ordinea în care ați făcut operațiile (de exemplu, dacă ați creat mai întâi slave-urile sau ați încărcat informațiile în baza de date master mai întâi).

---

**Trebuie să încărcați:**
- fișierul `docker-compose.yml` / fișierul `Dockerfile` (dacă nu ați folosit docker compose);
- un fișier TXT cu toate comenzile pe care le-ați rulat în shell de MySQL (gen `SELECT * FROM ...`) → `mysql-history.txt`;
- un fișier TXT cu toate comenzile pe care le-ați rulat în linie de comandă (inclusiv `docker compose up -d`) → `shell-history.txt`;
- un fișier TXT cu o scurtă explicație a ceea ce ați făcut (și de ce, cum ați redus timpul de căutare etc.) → `explicatii.txt`.

# Rezolvare

```bash
flavius@flavius-Katana-GF66-12UEO:~/Documents/ABD/test1$ code docker-compose.yaml
```

```yaml
# Use root/example as user/password credentials
services:

  db: #serverul de baze de date (MariaDB)
    image: mariadb:latest #pentru ultima versiune
    container_name: test_abd_imdb
    restart: always
    environment:
      MARIADB_ROOT_PASSWORD: toor
      MARIADB_DATABASE: imdb
      MARIADB_USER: user_imdb
      MARIADB_PASSWORD: user_pass
    ports:
      - 3306:3306
    volumes:
      - db_data:/var/lib/mysql #folderul din container unde MariaDB își ține toate datele (tabele, indexuri, etc.)

  adminer: #interfata web (client) pentru DB
    image: adminer
    restart: always
    ports:
      - 8080:8080
    environment:
      ADMINER_DEFAULT_SERVER: db
    depends_on:
      - db
  
volumes:
  db_data:
```

```bash
docker compose down -v //in caz ca avem si alte containere pornite
```

```bash
docker compose up -d
docker compose ps
```

![Img](images/img1.png)

