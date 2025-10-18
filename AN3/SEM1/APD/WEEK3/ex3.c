/*
 * Measures speedup for matrix-vector multiplication
 * Uses and adapts code samples from [Pacheco]
 * For a given size m, n generates random matrix A and vector x
 * and measures runtime of serial and parallel algorithm
 * in order to compute speedup.
 * The parallel solution assumes that m is divisible by the number of threads!
 */

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* If DEBUG defined, prints arrays*/
// #define DEBUG

/* maximum range for element values */
#define MAXRANGE 5000

/* Global variables */
int thread_count;
int m, n;
double *A;
double *x;
double *y;
double *y_serial;

/*TEMA=============================================================================*/
double *a;
double *b;
double *res;

/* Serial functions */
void Usage(char *prog_name);
void Generate_matrix(char *prompt, double A[], int m, int n);
void Generate_vector(char *prompt, double x[], int n);
void Print_matrix(char *title, double A[], int m, int n);
void Print_vector(char *title, double y[], double m);
int Equal_vectors(double y[], double z[], double m);

void Mat_vect_mult(double A[], double x[], double y[], int m, int n);

/* Parallel functions */
void *Pth_mat_vect(void *rank);
void Mat_vect_mult_parallel(double A[], double x[], double y[], int m, int n);

/*------------------------------------------------------------------*/

/*==================================TEMA========================================*/
void compute_serial(double a[], double b[], double res[], int n);
void *compute_parallel_threads(void *rank);
void compute_parallel(double a[], double b[], double res[], int n);
/*==============================================================================*/
int main(int argc, char *argv[])
{

   if (argc != 2)
      Usage(argv[0]);
   thread_count = atoi(argv[1]);

   printf("Enter m and n\n");
   scanf("%d%d", &m, &n);

   if (m % thread_count != 0)
   {
      printf("m is not divisible by thread_count!\n");
      exit(1);
   }

   A = malloc(m * n * sizeof(double));
   x = malloc(n * sizeof(double));
   y_serial = malloc(m * sizeof(double));
   y = malloc(m * sizeof(double));

   Generate_matrix("Generate the matrix", A, m, n);
#ifdef DEBUG
   Print_matrix("Matrix is", A, m, n);
#endif

   Generate_vector("Generate the vector", x, n);
#ifdef DEBUG
   Print_vector("Vector is", x, n);
#endif

   struct timespec start, finish;
   double elapsed_serial, elapsed_parallel;
   printf("Serial: ");
   clock_gettime(CLOCK_MONOTONIC, &start); // measure wall clock time!

   Mat_vect_mult(A, x, y_serial, m, n);

   clock_gettime(CLOCK_MONOTONIC, &finish);

   elapsed_serial = (finish.tv_sec - start.tv_sec);
   elapsed_serial += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

   printf("time =%lf \n", elapsed_serial);
#ifdef DEBUG
   Print_vector("Serial Result", y_serial, m);
#endif

   printf("Parallel: ");

   clock_gettime(CLOCK_MONOTONIC, &start); // measure wall clock time!

   Mat_vect_mult_parallel(A, x, y, m, n);

   clock_gettime(CLOCK_MONOTONIC, &finish);

   elapsed_parallel = (finish.tv_sec - start.tv_sec);
   elapsed_parallel += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

   printf("time =%lf \n", elapsed_parallel);

#ifdef DEBUG
   Print_vector("Paralell Result", y, m);
#endif

   if (!Equal_vectors(y, y_serial, m))
      printf("Error! Serial and Parallel result vectors not equal! \n");

   printf("Number of parallel threads was %d\n", thread_count);
   printf("Measured Speedup=%f\n ", elapsed_serial / elapsed_parallel);

   free(A);
   free(x);
   free(y);
   free(y_serial);

   /*TEMA==================================================*/
   //double *a, *b, *res;
   n = 150000000;

   a = (double*)malloc(sizeof(double) * n);
   b = (double*)malloc(sizeof(double) * n);
   res = (double*)malloc(sizeof(double) * n);

   if(a == NULL || b == NULL || res == NULL){
        perror("Eroare alocare memoriei!\n");
        exit(-1);
   }

   srand(time(NULL));
   for(int i = 0; i < n; i = i + 1){
        a[i] = rand() % 100;
        b[i] = rand() % 100;
   }

   struct timespec start_s, end_s;
   double timp_s;

   clock_gettime(CLOCK_MONOTONIC, &start_s);
   compute_serial(a,b,res,n);
   clock_gettime(CLOCK_MONOTONIC, &end_s);

   timp_s = (end_s.tv_sec - start_s.tv_sec) + (end_s.tv_nsec - start_s.tv_nsec) / 1e9;
   printf("\nTema\n");
   printf("Timp serial: %lf seconds\n",timp_s);

   /*timp paralel*/
   struct timespec start_p, end_p;
   double timp_p;

   clock_gettime(CLOCK_MONOTONIC, &start_p);
   compute_parallel(a,b,res,n);
   clock_gettime(CLOCK_MONOTONIC, &end_p);
   timp_p = (end_p.tv_sec - start_p.tv_sec) + (end_p.tv_nsec - start_p.tv_nsec) / 1e9;

   printf("Timp paralel: %lf seconds\n", timp_p);

   free(a);
   free(b);
   free(res);

   return 0;
} /* main */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage(char *prog_name)
{
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
} /* Usage */

/*------------------------------------------------------------------
 * Function:    Read_matrix
 * Purpose:     Generate random the matrix
 * In args:     prompt, m, n
 * Out arg:     A
 */
void Generate_matrix(char *prompt, double A[], int m, int n)
{
   int i, j;

   srand(time(NULL));
   printf("%s\n", prompt);
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         // scanf("%lf", &A[i*n+j]);
         A[i * n + j] = rand() % MAXRANGE;
} /* Generate_matrix */

/*------------------------------------------------------------------
 * Function:        Generate_vector
 * Purpose:         Generate random the vector x
 * In arg:          prompt, n
 * Out arg:         x
 */
void Generate_vector(char *prompt, double x[], int n)
{
   int i;

   printf("%s\n", prompt);
   for (i = 0; i < n; i++)
      // scanf("%lf", &x[i]);
      x[i] = rand() % MAXRANGE;
} /* Generate_vector */

int Equal_vectors(double y[], double z[], double m)
{
   int i;
   for (i = 0; i < m; i++)
      if (y[i] != z[i])
         return 0;
   return 1;
}

/*-------------------------------------------------------------------
 * Function:   Mat_vect_mult
 * Purpose:    Multiply a matrix by a vector
 * In args:    A: the matrix
 *             x: the vector being multiplied by A
 *             m: the number of rows in A and components in y
 *             n: the number of columns in A components in x
 * Out args:   y: the product vector Ax
 */
void Mat_vect_mult(
    double A[] /* in  */,
    double x[] /* in  */,
    double y[] /* out */,
    int m /* in  */,
    int n /* in  */)
{
   int i, j;

   for (i = 0; i < m; i++)
   {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
         y[i] += A[i * n + j] * x[j];
   }
} /* Mat_vect_mult */

/*------------------------------------------------------------------
 * Function:       Pth_mat_vect
 * Purpose:        One thread in Multiply an mxn matrix by an nx1 column vector
 * In arg:         rank
 * Global in vars: A, x, m, n, thread_count
 * Global out var: y
 */
void *Pth_mat_vect(void *rank)
{
   int my_rank = *(int *)rank;
   int i, j;
   int local_m = m / thread_count;
   int my_first_row = my_rank * local_m;
   int my_last_row = (my_rank + 1) * local_m - 1;

   for (i = my_first_row; i <= my_last_row; i++)
   {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
         y[i] += A[i * n + j] * x[j];
   }

   return NULL;
} /* Pth_mat_vect */

/*-------------------------------------------------------------------
 * Function:   Mat_vect_mult_parallel
 * Purpose:    Multiply a matrix by a vector, in parallel
 * In args:    A: the matrix
 *             x: the vector being multiplied by A
 *             m: the number of rows in A and components in y
 *             n: the number of columns in A components in x
 * Out args:   y: the product vector Ax
 */
void Mat_vect_mult_parallel(
    double A[] /* in  */,
    double x[] /* in  */,
    double y[] /* out */,
    int m /* in  */,
    int n /* in  */)
{
   int thread;
   pthread_t *thread_handles;
   thread_handles = malloc(thread_count * sizeof(pthread_t));
   int *tid;
   tid = malloc(thread_count * sizeof(int));

   for (thread = 0; thread < thread_count; thread++)
   {
      tid[thread] = thread;
      pthread_create(&thread_handles[thread], NULL,
                     Pth_mat_vect, &tid[thread]);
   }

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);

   free(thread_handles);
   free(tid);
} /* Mat_vect_mult_parallel */

/*------------------------------------------------------------------
 * Function:    Print_matrix
 * Purpose:     Print the matrix
 * In args:     title, A, m, n
 */
void Print_matrix(char *title, double A[], int m, int n)
{
   int i, j;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
   {
      for (j = 0; j < n; j++)
         printf("%4.1f ", A[i * n + j]);
      printf("\n");
   }
} /* Print_matrix */

/*------------------------------------------------------------------
 * Function:    Print_vector
 * Purpose:     Print a vector
 * In args:     title, y, m
 */
void Print_vector(char *title, double y[], double m)
{
   int i;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%4.1f ", y[i]);
   printf("\n");
} /* Print_vector */

/*TEMA=======================================================================*/
void compute_serial(double a[], double b[], double res[], int n){
    for(int i = 0; i < n; i = i + 1){
        res[i] = a[i] * a[i] + b[i] * b[i];
    }
}

void *compute_parallel_threads(void *rank){
    int my_rank = *(int*)rank;
    int local_n = n / thread_count;
    int start_index = my_rank * local_n;
    int final_index = (my_rank + 1) * local_n - 1;

    for(int i = start_index; i < final_index; i = i + 1){
        res[i] = a[i] * a[i] + b[i] * b[i];
    }

    return NULL;
}

void compute_parallel(double a[], double b[], double res[], int n){
    pthread_t *thread_handles;
    thread_handles = (pthread_t*)malloc(sizeof(pthread_t) * thread_count);
    
    int *tid;
    tid = (int*)malloc(sizeof(int) * thread_count);

    for(int thread = 0; thread < thread_count; thread = thread + 1){
        tid[thread] = thread;
        pthread_create(&thread_handles[thread],NULL,compute_parallel_threads,&tid[thread]);
    }

    for(int thread = 0; thread < thread_count; thread = thread + 1){
        pthread_join(thread_handles[thread],NULL);
    }

    free(thread_handles);
    free(tid);
}