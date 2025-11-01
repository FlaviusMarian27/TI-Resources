#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define THREAD_COUNT 8

/**
 *  Checks if  num is prime
 */
int is_prime(int num)
{
    if (num <= 1)
        return 0; // 0 and 1 are not prime numbers
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return 0; // Found a divisor, num is not prime
    }
    return 1; // num is prime
}

void generate_array(int a[], int n)
{
    // Seed the random number generator
    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        a[i] = rand();
    }
}

void print_array(int a[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
}

/**
 * Counts how many primes are in array a of length n
 */
int count_prime_serial(int a[], int n)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (is_prime(a[i]))
            count++;
    }
    return count;
}

int count_prime_parallel_v1(int a[], int n){
    int count = 0;
    #pragma omp parallel num_threads(THREAD_COUNT)
    {
        int my_rank = omp_get_thread_num();
        int local_n = n / THREAD_COUNT;
        int start = my_rank * local_n;
        int end = start + local_n;

        int local_count = 0;
        for(int i = start; i < end; i++){
            if(is_prime(a[i]) == 1){
                local_count = local_count + 1;
            }
        }

        #pragma omp critical
        {
            count = count + local_count;
        }
    }

    return count;
}

int count_prime_parallel_v2(int a[], int n){
    int count = 0;
    #pragma omp parallel num_threads(THREAD_COUNT) reduction(+: count)
    {
        int my_rank = omp_get_thread_num();
        int local_n = n / THREAD_COUNT;
        int start = my_rank * local_n;
        int end = start + local_n;
        for(int i = start; i < end; i++){
            if(is_prime(a[i]) == 1){
                count = count + 1;
            }
        }
    }

    return count;
}

int count_prime_parallel_v3(int a[], int n){
    int count = 0;

    #pragma omp parallel for num_threads(THREAD_COUNT) reduction(+: count)
    for(int i = 0; i < n; i++){
        if(is_prime(a[i]) == 1){
            count = count + 1;
        }
    }

    return count;
}

int main()
{
    int n, count = 0;

    printf("Enter the size of the array: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    generate_array(arr, n);

    double start, time;
    start = omp_get_wtime();
    count = count_prime_serial(arr, n);
    time = omp_get_wtime() - start;
    printf("\nTotal prime numbers in the array: %d\n", count);
    printf("Serial time =%f\n", time);

    start = omp_get_wtime();
    count = count_prime_parallel_v1(arr, n);  //TO BE ADDED
    time = omp_get_wtime() - start;
    printf("\nTotal prime numbers in the array: %d\n", count);
    printf("Parallel V1 time =%f\n", time);

    start = omp_get_wtime();
    count = count_prime_parallel_v2(arr, n);   //TO BE ADDED
    time = omp_get_wtime() - start;
    printf("\nTotal prime numbers in the array: %d\n", count);
    printf("Parallel V2 time =%f\n", time);

    start = omp_get_wtime();
    count = count_prime_parallel_v3(arr, n);   //TO BE ADDED
    time = omp_get_wtime() - start;
    printf("\nTotal prime numbers in the array: %d\n", count);
    printf("Parallel V3 time =%f\n", time);

    free(arr);

    return 0;
}