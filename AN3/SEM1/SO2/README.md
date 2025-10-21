# 💻 Sisteme de Operare 2 — Laborator (2024–2025)

Acest repository conține rezolvările, notițele și exercițiile realizate în cadrul laboratoarelor de **Sisteme de Operare 2**.  
Scopul este de a consolida conceptele avansate de programare la nivel de sistem: apeluri de sistem, procese, semnale, thread-uri și comunicație între procese.

---

## 🗓️ Calendarul laboratoarelor

| **Săptămâna** | **Interval**       | **Tematică laborator** |
|----------------|--------------------|--------------------------|
| 1 | 29.09 – 05.10 | Introducere și recapitulare SO |
| 2 | 06.10 – 12.10 | Fișiere și directoare. Apeluri de sistem |
| 3 | 13.10 – 19.10 | Fișiere și directoare. Apeluri de sistem |
| 4 | 20.10 – 26.10 | Procese. Pipes. Semnale |
| 5 | 27.10 – 02.11 | Procese. Pipes. Semnale |
| 6 | 03.11 – 09.11 | Procese. Named Pipes |
| 7 | 10.11 – 16.11 | ✅ Test evaluare 1 |
| 8 | 17.11 – 23.11 | Multithreading. Pthreads |
| 9 | 24.11 – 30.11 | Multithreading. Pthreads. Mutex |
| 10 | 01.12 – 07.12 | Multithreading. Pthreads. Mutex |
| 11 | 08.12 – 14.12 | ✅ Test evaluare 2 |
| 12 | 15.12 – 21.12 | Socket TCP |
| 13 | 12.01 – 18.01 | Socket TCP |
| 14 | 19.01 – 25.01 | ✅ Test evaluare 3 |

---

## 📘 Conținut principal

### 🔹 Laborator 1–3 — Fișiere și directoare
- Apeluri de sistem: `open`, `read`, `write`, `close`
- Funcții: `stat`, `lstat`, `fstat`
- Lucrul cu directoare: `opendir`, `readdir`, `closedir`
- Permisiuni, tipuri de fișiere, legături simbolice
- Exerciții: copiere fișiere, filtrare directoare, analiză fișiere binare

### 🔹 Laborator 4–6 — Procese, semnale și comunicare
- `fork`, `wait`, `exec`
- Ierarhia de procese și comunicarea părinte–copil
- `pipe`, `dup2`, `execlp`, `execvp`
- Semnale: `signal`, `kill`, `raise`, `SIGINT`, `SIGCHLD`
- Named pipes (FIFO)
- Exerciții: compilare fișiere `.c` în paralel, numărare caractere, execuții concurente

### 🔹 Laborator 8–10 — Multithreading (Pthreads)
- Crearea și gestionarea thread-urilor (`pthread_create`, `pthread_join`)
- Acces concurent la resurse
- Mecanisme de sincronizare: `pthread_mutex_t`, `pthread_cond_t`
- Exerciții: contorizări paralele, sincronizare acces la fișiere comune

### 🔹 Laborator 12–13 — Socket TCP
- Programare în rețea: model client–server
- Apeluri: `socket`, `bind`, `listen`, `accept`, `connect`
- Transmisie de date între procese pe mașini diferite

---

## 🧠 Tehnologii și instrumente
- Limbaj: **C (C99 / C11)**
- Compilator: **gcc**
- Mediu de lucru: **CLion / Linux Ubuntu**
- Tool-uri: `make`, `valgrind`, `gdb`, `strace`

---

## 📂 Structura recomandată a proiectului
    SO2-Labs/
    │
    ├── Week1-3_Files/
    │ ├── ex1_copy_file.c
    │ ├── ex2_stat_info.c
    │ └── ex3_recursive_dir.c
    │
    ├── Week4-6_Processes/
    │ ├── ex1_fork_wait.c
    │ ├── ex2_exec_compile.c
    │ └── ex3_pipe_sum.c
    │
    ├── Week8-10_Threads/
    │ ├── ex1_threads_sum.c
    │ └── ex2_mutex_sync.c
    │
    └── Week12-13_Sockets/
    ├── server.c
    └── client.c

 📌 *Scopul acestor laboratoare este înțelegerea modului în care procesele, fișierele și thread-urile interacționează cu kernelul Linux prin apeluri de sistem și mecanisme IPC.*