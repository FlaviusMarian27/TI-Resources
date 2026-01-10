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
}```

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

