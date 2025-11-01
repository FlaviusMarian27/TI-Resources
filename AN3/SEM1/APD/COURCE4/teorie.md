# 🧩 Recapitulare OpenMP – Programare Paralelă

## 🔹 1️⃣ Ce este OpenMP
OpenMP (Open Multi-Processing) este o bibliotecă de paralelizare pentru procesoare cu **memorie comună (shared memory)** — toate threadurile văd aceeași zonă de memorie.
👉 E orientată pe **paralelizare la nivel de bucle și secțiuni**, folosind directive `#pragma omp ...`.

---

## ⚙️ 2️⃣ Activare

Trebuie compilat cu flagul:

```bash
gcc -fopenmp program.c -o program
```

➡️ Dacă lipsește `-fopenmp`, directivele sunt ignorate complet.

---

## 🔸 3️⃣ Directive principale

### ✅ a) parallel

Creează o regiune paralelă – fiecare thread execută codul din interior.
```c
#pragma omp parallel
{
    printf("Salut de la threadul %d din %d\n", omp_get_thread_num(), omp_get_num_threads());
}
```

➡️ Implicit folosește numărul de core-uri logice.
Se poate seta manual:
```c
#pragma omp parallel num_threads(4)
```

---

### ✅ b) parallel for

Paralelizează o buclă:
```c
#pragma omp parallel for
for (int i = 0; i < n; i++)
    a[i] = b[i] * 2;
```

➡️ Fiecare thread primește un subset de iterații.

---

### ✅ c) critical

Protejează o secțiune critică (acces exclusiv).

```c
#pragma omp parallel for
for (int i = 0; i < n; i++) {
    if (a[i] > 100) {
        #pragma omp critical
        counter++;
    }
}
```

➡️ Evită race conditions, dar scade performanța.

---

### ✅ d) reduction

Înlocuiește secțiunile critice pentru acumulări.

```c
int sum = 0;
#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < n; i++)
    sum += a[i];
```

➡️ Fiecare thread are o copie locală, combinate automat la final.

---

### ✅ e) private / shared / firstprivate

Controlează vizibilitatea variabilelor:
shared(x)       – toți threadurile folosesc aceeași variabilă  
private(x)      – fiecare are propria copie neinițializată  
firstprivate(x) – copie privată inițializată cu valoarea globală

Exemplu:
```c
#pragma omp parallel for private(i) shared(a,b)
```

---

### ✅ f) schedule

Controlează cum se împart iterațiile:

```c
#pragma omp parallel for schedule(static,4)
```

Opțiuni:

static   – blocuri fixe pentru fiecare thread  
dynamic  – threadurile iau dinamic următorul bloc liber  
guided   – blocuri mari la început, mici spre final  
auto     – compilatorul decide

---

### ✅ g) sections

Pentru taskuri diferite, nu bucle.

```c
#pragma omp parallel sections
{
    #pragma omp section
    task1();
    #pragma omp section
    task2();
    #pragma omp section
    task3();
}
```

---

### ✅ h) barrier

Sincronizare: toate threadurile trebuie să ajungă aici înainte de a continua.

---

## ⚙️ 4️⃣ Funcții utile (<omp.h>)
omp_get_thread_num()   – ID-ul threadului curent  
omp_get_num_threads()  – numărul total de threaduri  
omp_get_wtime()        – timp în secunde (pentru măsurători)  
omp_set_num_threads(n) – setează manual numărul de threaduri  

---

## ⏱️ 5️⃣ Măsurarea timpului

```c
double start, end;
start = omp_get_wtime();

#pragma omp parallel for
for (int i = 0; i < n; i++)
    work();

end = omp_get_wtime();
printf("Durata: %f secunde\n", end - start);
```

---

## ⚡ 6️⃣ Tipuri de paralelism

Data Parallelism  – paralelizare pe elemente (bucle for) → parallel for  
Task Parallelism  – taskuri independente → sections / tasks  
Nested Parallelism – regiuni paralele în interiorul altora (avansat)

---

## ⚠️ 7️⃣ Erori frecvente

❌ Race condition – două threaduri modifică aceeași variabilă simultan → folosește critical sau reduction.  
❌ False sharing – variabile diferite împart aceeași linie de cache → scade performanța.  
❌ Overhead mare – prea multe threaduri pentru o sarcină mică → pierdere de timp.

---

## 📚 8️⃣ Ce trebuie să știi la laborator / test

✅ Cum se compilează (-fopenmp)  
✅ Diferența parallel / parallel for  
✅ Ce face reduction vs critical  
✅ Cum funcționează schedule  
✅ Cum se măsoară timpul (omp_get_wtime)  
✅ Ce e race condition și cum se evită  
✅ Ce înseamnă private / shared  

---

## 💡 Recomandări practice

- Testează diferite variante de schedule(static, dynamic, guided)
- Măsoară timpul: Speedup = T_serial / T_parallel
- Începe de la o versiune serială corectă
- Folosește reduction în loc de critical când e posibil
- Evită prea multe threaduri pentru sarcini mici

---

## 🧠 Exemple rapide

### 1️⃣ Suma vectorului

```c
int sum = 0;
#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < n; i++)
    sum += v[i];
```

### 2️⃣ Căutarea maximului

```c
int max_val = -1;
#pragma omp parallel for reduction(max:max_val)
for (int i = 0; i < n; i++)
    if (v[i] > max_val) max_val = v[i];
```

### 3️⃣ Integrare numerică (trapezoidală)

```c
double integral = 0.0, h = (b - a) / n;
#pragma omp parallel for reduction(+:integral)
for (int i = 1; i < n; i++)
    integral += f(a + i*h);
integral = h * (integral + (f(a)+f(b))/2);
```
---

# 📘 Concluzie

- OpenMP oferă o metodă simplă și eficientă de a paraleliza programele C/C++ pe sisteme cu memorie comună.  
- Ideal pentru bucle mari, calcule numerice și simulări.  
- Folosește-l cu cap, evitând zonele critice inutile și măsurând mereu performanța.
