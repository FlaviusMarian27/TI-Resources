/*
 * Measures speedup for matrix-vector multiplication
 * Uses and adapts code samples from [Pacheco]
 * For a given size m, n generates random matrix A and vector x
 * and measures runtime of serial and parallel algorithm
 * in order to compute speedup
 */

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* If DEBUG defined, prints arrays*/
// #define DEBUG

/* Global variables */
int thread_count;
int m, n;
double *A;
double *x;
double *y;
double *y_serial;

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
int main(int argc, char *argv[])
{

    if (argc != 2)
        Usage(argv[0]);
    thread_count = atoi(argv[1]);

    printf("Enter m and n\n");
    scanf("%d%d", &m, &n);

    if (m % thread_count !=0){
        printf("size m is not divisible by number of threads ! \n");
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

    //-----
    struct timespec start, stop;
    double serial_time, paralell_time;

    printf("Serial... ");
    clock_gettime(CLOCK_MONOTONIC,&start);
    Mat_vect_mult(A, x, y_serial, m, n);
    clock_gettime(CLOCK_MONOTONIC,&stop);
    serial_time = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
    printf("Serial time: %lf sec\n",serial_time);


#ifdef DEBUG
    Print_vector("Serial Result", y_serial, m);
#endif

    printf("Parallel... ");
    clock_gettime(CLOCK_MONOTONIC,&start);
    Mat_vect_mult_parallel(A, x, y, m, n);
    clock_gettime(CLOCK_MONOTONIC,&stop);
    paralell_time = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
    printf("Paralell time: %lf sec\n",paralell_time);

#ifdef DEBUG
    Print_vector("Paralell Result", y, m);
#endif

    if (!Equal_vectors(y, y_serial, m))
        printf("Error! Serial and Parallel result vectors not equal! \n");

    printf("Number of parallel threads was %d\n", thread_count);

    //speedup & efficiency
    double speedup = serial_time / paralell_time;
    double efficiency = speedup / thread_count;

    printf("\n======================================\n");
    printf("Threads: %d\n",thread_count);
    printf("Matrix size: %d x %d\n", m, n);
    printf("Serial time:   %lf sec\n", serial_time);
    printf("Parallel time: %lf sec\n", paralell_time);
    printf("Speedup:       %lf\n", speedup);
    printf("Efficiency:    %lf\n", efficiency);

    free(A);
    free(x);
    free(y);
    free(y_serial);

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
            A[i * n + j] = rand();
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
        x[i] = rand();
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
    int my_rank = *(int*)rank;
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
    tid=malloc(thread_count *sizeof(int));

    for (thread = 0; thread < thread_count; thread++) {
        tid[thread]= thread;
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