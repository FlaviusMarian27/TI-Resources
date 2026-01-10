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

