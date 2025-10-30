
## 🔹 Ce este Docker?

#### 🧩 **Docker** = **un sistem care rulează aplicații în containere izolate**.

##### Un container:
-  are propriul mediu(sistem de fișiere, procese);
-  se bazează pe o imagine(un șablon read-only);
-  pornește și consumă puține resurse.

### Tips:  

💡 Gândește-te la imagine ca la o „rețetă”  și la container ca la „mâncarea gătită” din acea rețetă

---
---
## 🔹Primul container

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker run hello-world
```

#### 🔍 Ce face?
 -  descarcă o imagine oficială de test
 -  o rulează
 -  îți afișează un mesaj de confirmare.

Dacă vezi „Hello from Docker!”, înseamnă că sistemul funcționează.

---
---
## 🔹Vizualizează ce s-a întâmplat

##### Listează imaginile descărcate:

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker images
```

##### Listează toate containerele (inclusiv cele oprite):

```bash
flavius@flavius-Katana-GF66-12UEO:~$ docker ps -a
```

##### Observă:

- coloana `IMAGE` arată din ce imagine s-a creat containerul;
- `STATUS` îți spune dacă rulează sau s-a oprit.

---
---
### 🔹 4. Rulează un container interactiv

Acum pornește un **Ubuntu real**:

```bash
docker run -it ubuntu bash
```

🟢 Ești acum **în interiorul** unui container Ubuntu.  

Testează câteva comenzi:
```bash
ls cat /etc/os-release
```

Apoi ieși:
```bash
ls cat /etc/os-release
```

---

### 🔹 5. Analizează rezultatul

După ce ieși, rulează:

```bash
docker ps -a
```

Observă că acel container există, dar e „Exited”.

Dacă vrei să-l pornești din nou:

```bash
docker start -ai <container_id>
```

Dacă vrei să-l ștergi:

```bash
docker rm <container_id>
```

---

### 🔹 6. Curățare și informații suplimentare

Pentru informații generale:

```bash
docker info
```

Pentru a șterge toate containerele oprite și imaginile nefolosite:

```bash
docker system prune -a
```

---

✅ **După capitolul 1 trebuie să știi:**

- diferența dintre _imagine_ și _container_;
- cum pornești, oprești și verifici un container;
- că totul e izolat de sistemul tău real.