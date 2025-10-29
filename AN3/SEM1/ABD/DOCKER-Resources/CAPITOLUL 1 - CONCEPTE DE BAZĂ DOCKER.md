
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
