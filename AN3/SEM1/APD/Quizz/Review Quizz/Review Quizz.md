#### **A few review questions and exercises. This is not an exhaustive coverage of all topics!**

---
# **Question.** 

If for example 20% of a program cannot be parallelized because it is intrinsec sequential, then according to Amdahl's law  the maximum speedup regardless of increasing the number of processors is ......

# **Answer**:

$$
S = \frac{T_{serial}}{T_{parallel}}
$$

$$
T_{parallel} = (1 - r)\left(\frac{T_{serial}}{p}\right) + r \cdot T_{serial}
$$

$$
S = \frac{T_{serial}}
{(1 - r)\frac{T_{serial}}{p} + rT_{serial}}
$$

$$
S(p) = \frac{T_{serial}}
{T_{serial}\left(r + \frac{1-r}{p}\right)}
= \frac{1}{r + \frac{1-r}{p}}
$$

$$
S_{max} = \lim_{p \to \infty}
\frac{1}{r + \frac{1-r}{p}}
= \frac{1}{r}
$$

$$
S_{max} = \frac{1}{0.2} = 5
$$

---
# **Question:**

Is it possible to encounter in practice a situation  when we measure a Speedup S, S > p where p is the number of processors? _**Explain.**_

# **Answer:**

Da, este posibil o astfel de situație, aceasta se numeste speedup superlinear și apare atunci când speedup-ul este mai mare decât numărul de procese utile (S > p). Un motiv ar fi de cache memory.

---
# **Question:**

Give a short example of code where the performance of the algorithm is affected by _**false sharing**_. _**Explain.**_

# **Answer**

False sharing este atunci când nu există un sharing adevărat. Practic avem 2 threaduri spre exemplu, care modifică 2 variabile diferite fiecare și se află pe același nivel de memorie cache.

```c
#include <omp.h>
#define N 100000000

long long counters[2];

int main() {
  #pragma omp parallel num_threads(2)
  {
    int tid = omp_get_thread_num();
    for (int i = 0; i < N; i++)
      counters[tid]++;     // thread0 -> counters[0], thread1 -> counters[1]
  }
}
```

Thread-urile modifică variabile diferite( counters[0] și counters[1]), ele sunt în același cache line, iar fiecare "++" provoacă false sharing.

---

# **Question**: 

Explain the concepts:  _**Critical section**_ and _**Race condition.**_

# **Answer:**

Race condition apare atunci când două sau mai multe thread-uri, accesează aceeași variabilă partajată în același timp. Exemplu pentru counter:

```c
void *inc_count(void *t)
{
    int i;
    int my_id = *(int *)t;

    count++;
    printf("Thread %d incremented count to %d \n", my_id, count);
    return NULL;
}
```

Critical section este o parte de cod, dintr-un program care accesează date partajate și care nu trebuie rulată simultan de mai multe thread-uri, doar de unul singur. De obicei, astfel de zone trebuie protejate, cu un mutex spre exemplu.

---

# **Question:** 

Analyze _carefully_ the program given below. What are the threads doing? Are there any concurrency problems?  What output is printed?

```C
int x = 0;  
  
void *thread1_function(void *arg) {  
    for (int i = 0; i < 10; i++)  {  
        printf("X");  
        x = 1;  
    }  
  
    return NULL;  
}  
  
   
void *thread2_function(void *arg) {  
    for (int i = 0; i < 10; i++)  {  
        while (x == 0)  {  }  
        printf("Y");  
        x = 0;  
    }  
    return NULL;  
}   
  
int main() {  
    pthread_t thread1, thread2;  
    pthread_create(&thread1, NULL, thread1_function, NULL);  
    pthread_create(&thread2, NULL, thread2_function, NULL);  
    pthread_join(thread1, NULL);  
    pthread_join(thread2, NULL);  
  
    return 0;    
}
```

# **Answer:**

În mod normal, fiecare thread ar trebui să afișeze fiecare literă de 10 ori. Astfel, există probleme din cauză că x este accesat fără sincronizări, iar în Thread2 poate rămâne blocat în while deoarece schimbările lui x nu sunt garantat vizibile. În plus bucla e busy-wait și consumă CPU.

---

# **Question:** 

For the program below, answer the questions:
- How many threads are created during the entire program execution?
- For every element $a[i][j]$, show which thread computes it.

```C
#define N 4
int a[N][N];

int main(int argc, char* argv[]) {  
  for (int i=0; i<N; i++)  
    #pragma omp parallel for num_threads(2) schedule(static, 1)  
    for (int j=0; j<N; j++)  
      a[i][j]=i+j;  
}
```

# **Answer:**

Pe parcursul execuție programului se creează doar 2 thread-uri. Practic la prima iterație, după întâlnirea directivei `pragma omp parallel for`,  se creează un grup de 2 thread-uri, iar pentru restul de iterații se refolosesc din nou cele 2 thread-uri fără să mai fie altele create.

Pentru fiecare $a[i][j]$ avem 2 thread-uri:

thread 0 -> j = 0, j = 2
thread 1 -> j = 1, j = 3

---
# **Question:**

**_Explain_** the concept of _**parallel reduction**_.  When is it useful?  Which ones of the following technologies support the parallel reduction concept: POSIX threads, OpenMP, MPI?

# **Answer:**

Parallel reduction înlocuiește secțiunile critice pentru acumulări. Fiecare thread calculează un rezultat parțial apoi le combină la final într-un singur rezultat(suma, maxim etc). Este specific pentru OpenMP (reduce(op:var)) și MPI prin MPI_Reduce / MPI_Allreduce.

---
# **Question.** 

Which form of parallelization is best for the following sequence of code?  _**Explain**_ your answer.

```c
#define N 1000
int a[N];  
// ... a[i] initialized with some random values, details omitted

for (int i=0; i<N; i++) {  
  int p=1;  
  for (int j=0; j<i; j++)  
      p=p*a[i];  
  a[i]=p;  
}
```

# **Answer** 

Cea mai bun paralelizare este pe bucla exterioară, adică pe primul for. Fiecare iterație calculează independent $a[i]$, deci nu există dependențe între valori diferite de `i`. Pentru că numărul de iterații în bucla interioară crește cu `i`, e recomandat `schedule(dynamic/guided)` pentru load balancing.

---

# **Question.**   

The code below computes how many times x occurs in each quarter of an array.  Are there any improvements that should be made in order to have a better performance? _**Explain.**_

```c
int arr[N];

// ... init arr[i] with some values
int x = 2; // Value to count occurencies
int quarter_counts[4] = {0, 0, 0, 0};

#pragma omp parallel num_threads(4){
	int tid = omp_get_thread_num();
	int start = tid * (n / 4);
	int end = start + (n / 4);
	int count = 0;
	
	for (int i = start; i < end; i++) {
		if (arr[i] == x) {
			quarter_counts[tid]++;
		}
	}
}

// Print counts for each quarter
for (int i = 0; i < 4; i++) {
	printf("Quarter %d count of %d: %d\n", i+1, x, quarter_counts[i]);
}
```

# **Answer:**

- **Folosește contor local** (`count`) în buclă și scrie o singură dată la final: `quarter_counts[tid] = count;` (eviți multe scrieri în memorie shared).
- Așa reduci și **false sharing** pe `quarter_counts[ ]` (altfel thread-urile se invalidează reciproc pe aceeași linie de cache).
- Corectează împărțirea: dacă `N` nu e divizibil cu 4, ultimul thread trebuie să ia **restul** (`end = (tid==3 ? N : start+N/4)`).
- (opțional) poți folosi și `#pragma omp parallel for` cu `schedule(static)` și să calculezi pe quarters fără manual `start/end`, dar varianta de mai sus e suficientă.

---

# **Question:**

Can you parallelize the following sequence of code using openMP ? _**Explain**_ your answer.

```c
previousFib = 0;       
currentFib  = 1;       
for (i = 2; i <= n; i++) {  
    nextFib     = previousFib + currentFib;    
    previousFib = currentFib;                
    currentFib  = nextFib;  
}  
fibonacciN = currentFib;
```

# **Answer**

Nu poate fi paralelizată direct cum ar fi cu `#pragma omp parallel for` spre exemplu sau altă directivă, deoarece există dependențe de date între iterații: la pasul i, previousFib și currentFib sunt rezultatele pasului i-1. Deci ordinea execuției trebuie să fie secvențială. 

---

# **Question.** 

Can you parallelize computing the height of a binary tree using openMP? _**Explain**_ your answer.

```c
int height(binary_tree *tree) {
	if (tree == NULL)
		return 0;
	int left, right;
	left = height(tree->left);
	right = height(tree->right);
	return maximum(left, right) + 1;
}
...
h1=height(t1);
```

# **Answer:**

Da se poate paraleliza, cu ajutorul task-urilor. Practic atât apelul height(tree->left), cât și apelul
height(tree->right) pot rula în paralel.  Astfel pentru cele 2  apeluri folosim `#pragma omp task`, iar pentru maximum folosim taskwait. Pentru performanță vom folosi un cutoff cu care vom evita overhead-ul mare, pe care îl creează task-urile, trecând în metoda serială.

```c
int height_parallel(binary_tree *tree, int level) {
	if (tree == NULL)
		return 0;
		
	int left, right;
	if(level >= 10){
		return height(tree);
	}
	
	#pragma omp task
	{
		left = height(tree->left);
	}
	
	#pragma omp task
	{
		right = height(tree->right);
	}
	
	#pragma omp taskwait
		return maximum(left, right) + 1;
}

int make_task(binary_tree *tree){
	int h = 0;
	#pragma omp parallel num_threads(NUMTHREADS) shared(h)
	{
		#pragma omp single
		{
			h = height_p(tree,0);
		}
	}
}
```

---

# **Question:**  

Given an array `A` of `n` integers, construct a **Binary Search Tree** such that for every node `x,` all keys in the left subtree of `x` are smaller than `x->key` and all keys in the right subtree of `x` are greater than `x->key`

Can you find **a parallel strategy for the construction of the Binary Search Tree?** You do not have to write code,  pseudocode or textual description is OK.

# **Answer:**

1. Alege o cheie ca rădăcină( de exemplu A[0])
2. Partiționăm vectorul în două submulțimi:
	- L = { a ∈ A | a < root }
	- R = { a ∈ A | a > root }
3. Construiești în paralel după:
	- `root->left = buildBST(L)`
	- `root->right = buildBST(R)`
4. După repetăm recursiv. Punem și un cutoff: când subvectorul devine mic, construiești serial.

De ce e paralelizabil:
- după partiționare, **subarborele stâng și drept sunt independente**, deci pot fi făcute simultan (task-uri OpenMP / fork-join).

---

# **Question.**

What will be the output printed in the console where the following program is launched with  the command mpiexec -n 2 prog. c 
Explain your answer.

```c
/* prog.c */
void main(int argc, char *argv[])
{
    int rank, size;
    int x=0;
   
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

   if (rank ==0) {
    printf("%d ", rank); fflush(stdout);
    x=1;
   }
   if (rank ==1){
    while (x==0) {;}
    printf("%d", rank);   fflush(stdout);
   }
    MPI_Finalize();
}
```

# **Answer:**

_ = spatiu 

Rezultatul va fi: 0__

`mpiexec -n 2` pornește 2 **procese** cu **spații de adrese separate**, deci `x` nu e shared (fiecare rank are propria copie). Rank 0 afișează `0` și își pune `x=1`, dar rank 1 vede în continuare `x=0` în procesul lui și rămâne blocat în `while` deoarece nu există `MPI_Send/MPI_Recv` care să transmită valoarea.

---

# **Question:**

What is the output of the following program? Explain your answer. The program is launched with mpiexec -n 3 prog

```c
int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int msg;
	int next = (rank + 1) % size; // next process
	int prev = (rank + 2) % size; // previous process
	
	MPI_Recv(&msg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Send(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
	
	printf("Process %d received %d from process %d\n", rank, msg, prev);
	
	MPI_Finalize();
	return 0;
}
```

# **Answer:**

Avem trei procese lansate în execuție și fiecare face prima oară recv blocking, ceea ce duce la un deadlock. Ca și o soluție ar fi un if, în care luam spre exemplu rank 0, care să înceapă cu un Send, iar ceilalți cu un recv. 
- rank 0: `Send` → `Recv` 
- rank 1,2: `Recv` → `Send`

Asta rupe așteptarea circulară.

---

# **Question:** 

What is the output of the following program? Explain your answer. The program is launched with mpiexec -n 3 prog

Reminder: the arguments of MPI_Scatter are:  
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,  
               void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,  
               MPI_Comm comm);

```c
int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int x[3], y[3]; x[0]=y[0]=rank; x[1]=y[1]=rank+1; x[2]=y[2]=rank+2;
	
	MPI_Scatter(x, 1, MPI_INT, y, 1, MPI_INT, 2, MPI_COMM_WORLD);
	printf("%d %d %d\n", y[0], y[1], y[2]);
	MPI_Finalize();
	
	return 0;
}
```

# **Answer**

Root este rank 2, deci doar el folosește `x=[2,3,4]`. `MPI_Scatter` cu `sendcount=recvcount=1` trimite câte un element: rank0 primește 2, rank1 primește 3, rank2 primește 4, și îl scrie în `y[0]` (restul lui `y` rămâne neschimbat). Se afișează liniile `2 1 2`, `3 2 3`, `4 3 4` în ordine nedeterministă.

---

# **Question.** 

Exploratory decomposition algorithms (based on the exploration of a solution space) are implemented using:  
a.) OpenMP - #pragma omp parallel for  
b.) OpenMP - #pragma omp parallel  
c.) Openmp - #pragma omp task  
d.) MPI  
Can you _rank these methods_ from the _best suited_ to the _least suited_? Justify your answer.

# **Answer:**

1. #pragma omp task - cel mai potrivit, ideal pentru programe recursive. `Task` permite **scheduling dinamic / work stealing**, deci load balancing bun.
2.  MPI (foarte bun, dar mai greu/overhead) - Se pot implementa **master–worker / work pool** pentru distribuire dinamică pe procese; merge pe cluster, dar are **overhead de comunicare** și cod mai complex.
3.  `#pragma omp parallel` (mai puțin potrivit)
4. `#pragma omp parallel for` (cel mai puțin potrivit)

---

# **Question:**  

Enumerate a few reasons why programmers should use collective communication  operations instead of achieving the same semantics using only point-to-point messages.

# **Answer:**

- **Cod mai simplu și mai puține bug-uri:** mai puține apeluri, logică mai clară.
- **Performanța:** colectivele sunt optimizate în implementările MPI, de obicei mai rapide decât un set manual de `Send/Recv`.
- **Portabilitate**: același apel colectiv rulează bine pe hardware diferit

---

# **Question:** 

Explain the difference between  block partitioning and block-cyclic partitioning. When would you use one or the other?

# **Answer:**

- **Block partitioning (block):** datele sunt împărțite în bucăți contigue, practic fiecare proces primește ce un subinterval. Vom avea: 
	- Subintervalul = dimensiunea_totala/nr_de_procese.
- **Block-cyclic:** împarți în **blocuri mici**, apoi le distribui **round-robin** între procese.

---

# **Question:**

Compare 1D and 2D decomposition for a 2D matrix. How does it affect performance of a stencil computation like the Heat simulation?

# **Answer:**

Pentru varianta 1D decomposition, împărțirea se face pe linii/coloane, unde fiecare proces are 2 vecini fiecare (sus/jos). Mesajele sunt mai lungi(lungime N).

Pentru varianta 2D decomposition, împărțirea se face pe blocuri, unde fiecare proces are 4 vecini fiecare(sus/jos/stânga/dreapta). Mesajele sunt mai scurte(lungime N/sqrt(p)).

Astfel de obicei 2D scalează mult mai bine.

---

# **Question:**

Discuss strategies to overlap communication with computation by using non-blocking  (asynchronous) communication in a typical stencil computation like the Heat simulation. You do not have to write code, it is expected that you use diagrams or pseudocode and textual explanations.

# **Answer:**

Într-un stencil/Heat, fiecare proces are o submatrice locală + **halo/ghost cells**. La fiecare iterație trebuie să schimbe marginile cu vecinii. Ca să **suprapui comunicarea cu calculul**, folosești comunicație **non-blocking** (`MPI_Isend/Irecv`) și calculezi cât timp mesajele sunt în tranzit.
### Strategia (overlap comm/comp)
1. **Postezi recepțiile non-blocking** pentru halo (sus/jos, și stânga/dreapta la 2D).
2. **Trimiți non-blocking** marginile tale către vecini.
3. **Calculezi interiorul** submatricei (celule care nu depind de halo).
4. **Aștepți** finalizarea comunicării (`MPI_Waitall`).
5. **Calculezi border-ul** (celulele care depind de halo).
6. Swap `old/new` și treci la iterația următoare.

### Pseudocod (1 iterație)

```scss
MPI_Irecv(halo_up,    from up)
MPI_Irecv(halo_down,  from down)
MPI_Isend(my_top_row, to up)
MPI_Isend(my_bot_row, to down)

compute interior cells (not using halo)

MPI_Waitall(all requests)

compute border cells (need halo)

swap(old, new)
```

### Diagramă (timp)

```diff
comm halo:   [===========]
compute int:     [========]
wait+border:              [===]
```

---

# **Question:**  

An array of N numbers is held by a process P0.  Design a distributed-memory algorithm to sort this array. You do not have to write code, it is OK to use pseudocode or textual explanations.

# **Answer:**

1. Avem un vector A de dimensiune N la P0.
2. **Distribuția:** P0 împarte vectorul A în p procese aproape egale și le trimite cu MPI_Scatter, unde fiecare proces primeste N/p elemente.
3. **Sortarea:** sortarea se face local, unde fiecare proces va face sortarea pe bucata sa(quicksort/mergesort).
4. În log_2 p pași, procesele fac **pairwise merge**:
	- la pasul s, procesele cu rank multiplu de $2^{s+1}$ primesc lista de la vecinul lor și fac merge; celelalte trimit și “ies”.
5. La final, P0 are lista complet sortată.

---

# **Question:**

**_Compare:_** sending data over a TCP socket vs sending data with MPI_Send, MPI_Recv

# **Answer:**

La TCP trimiți practic bucăți de octeți. Dacă vrei să trimiți un vector de numere, trebuie să îl transformi tu în bytes și să îl refaci la destinație. La MPI este nevoie această operație, practic trimiți normal cei 100 de întregi și se ocupă biblioteca de restul.

Din punct de vedere al colectivității TCP este strict punct-la-punct, pe când MPI este și el punct-la-punct, dar mai oferă și operații colective(Broadcast, Scatter, Gather, Reduction).

---

# **Question:** 

Explain the difference between an iterative server and a concurrent server. Ennumerate 2 possible ways to  implement a concurrent server using BSD sockets.

# **Answer:**

**Iterative server:** acceptă doar un client, îl servește complet(read/write) apoi după ce a terminat cu acesta, îl acceptă pe următorul, iar ceilalți clienți așteaptă.

**Concurrent server:**  poate suporta gestionarea mai multor clienți în același timp, practic acceptă un client, server-ul nu se blochează pe acel client, apoi îl delegă și revine imediat la `accept` ca să poată servi mai mulți clienți în paralel.

BSD sockets:
- process per connection: `accept`, faci `fork()`; copilul tratează clientul, părintele revine la `accept`.
- thread per connection: după `accept`, creezi un thread (pthreads) care tratează clientul, iar thread-ul principal revine la `accept`.