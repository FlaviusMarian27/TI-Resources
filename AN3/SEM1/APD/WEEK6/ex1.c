/**
 * Heat 2D 
 * A rectangular plane of material has heat applied to the center of the upper edge.
 * The rectangular plane is represented as a grid of points.
 * The Laplace equation dictates how the heat will transfer from grid point to grid point over time.
 * The program simulates the diffusion of temperature in all points over time.
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

#define N 4000 /* grid points */
#define MAXITER 100
#define NTHREADS 8

// #define DEBUG

double *grid; // the grid and next generation grid
double *new_grid;

double *groundtruth; // saved result for comparison

void init_temperatures(void)
{
    grid = (double *)malloc(N * N * sizeof(double));
    if (!grid)
    {
        printf("Memory allocation error for grid\n");
        exit(1);
    }
    new_grid = (double *)malloc(N * N * sizeof(double));
    if (!new_grid)
    {
        printf("Memory allocation error for new grid\n");
        exit(1);
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            grid[i * N + j] = 20;
        }
    /* init boundary conditions:
    apply heat on middle part of upper boundary */
    for (int j = N / 3; j < N * 2 / 3; j++)
    {
        grid[0 * N + j] = 100;
    }
}

void print_grid(void)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%5.1lf ", grid[i * N + j]);
        }
        printf("\n");
    }
}

int equal_groundtruth(void)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            if (grid[i * N + j] != groundtruth[i * N + j])
                return 0;
        }
    return 1;
}

void save_groundtruth(void)
{
    groundtruth = (double *)malloc(N * N * sizeof(double));
    if (!groundtruth)
    {
        printf("Memory allocation error for groundtruth result\n");
        exit(1);
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            groundtruth[i * N + j] = grid[i * N + j];
        }
}

void swap_ptr(double **p1, double **p2)
{
    double *tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void serial_temp()
{
    int i, j, time;

    for (time = 0; time < MAXITER; time++)
    {
#ifdef DEBUG
        printf("\nIteration %d \n", time);
        print_grid();
#endif
        for (i = 1; i < N - 1; i++) // iterate grid but skip boundary
            for (j = 1; j < N - 1; j++)
            {
                new_grid[i * N + j] = (grid[(i + 1) * N + j] +
                                       grid[(i - 1) * N + j] +
                                       grid[i * N + j + 1] +
                                       grid[i * N + j - 1]) *
                                      0.25;
            }

        // Make new grid to current grid for the next generation
        swap_ptr(&grid, &new_grid);
    }
}

void parallel_temp()
{
    for (int time = 0; time < MAXITER; time++)
    {
#pragma omp parallel for num_threads(NTHREADS) default(none) shared(grid, new_grid)
        for (int i = 1; i < N - 1; i++) // iterate grid but skip boundary
            for (int j = 1; j < N - 1; j++)
            {
                new_grid[i * N + j] = (grid[(i + 1) * N + j] +
                                       grid[(i - 1) * N + j] +
                                       grid[i * N + j + 1] +
                                       grid[i * N + j - 1]) *
                                      0.25;
            }
        // Make new grid to current grid for the next generation
        swap_ptr(&grid, &new_grid);
    }
}

void parallel_temp_static_1(){
    for (int time = 0; time < MAXITER; time++)
    {
    #pragma omp parallel for num_threads(NTHREADS) schedule(static,1) default(none) shared(grid, new_grid)
            for (int i = 1; i < N - 1; i++) // iterate grid but skip boundary
                for (int j = 1; j < N - 1; j++)
                {
                    new_grid[i * N + j] = (grid[(i + 1) * N + j] +
                                        grid[(i - 1) * N + j] +
                                        grid[i * N + j + 1] +
                                        grid[i * N + j - 1]) *
                                        0.25;
                }
            // Make new grid to current grid for the next generation
            swap_ptr(&grid, &new_grid);
        }
}

void parallel_temp_static_20(){
    for (int time = 0; time < MAXITER; time++)
    {
    #pragma omp parallel for num_threads(NTHREADS) schedule(static,20) default(none) shared(grid, new_grid)
            for (int i = 1; i < N - 1; i++) // iterate grid but skip boundary
                for (int j = 1; j < N - 1; j++)
                {
                    new_grid[i * N + j] = (grid[(i + 1) * N + j] +
                                        grid[(i - 1) * N + j] +
                                        grid[i * N + j + 1] +
                                        grid[i * N + j - 1]) *
                                        0.25;
                }
            // Make new grid to current grid for the next generation
            swap_ptr(&grid, &new_grid);
        }
}

void parallel_temp_dynamic_1(){
    for (int time = 0; time < MAXITER; time++)
    {
    #pragma omp parallel for num_threads(NTHREADS) schedule(dynamic,1) default(none) shared(grid, new_grid)
            for (int i = 1; i < N - 1; i++) // iterate grid but skip boundary
                for (int j = 1; j < N - 1; j++)
                {
                    new_grid[i * N + j] = (grid[(i + 1) * N + j] +
                                        grid[(i - 1) * N + j] +
                                        grid[i * N + j + 1] +
                                        grid[i * N + j - 1]) *
                                        0.25;
                }
            // Make new grid to current grid for the next generation
            swap_ptr(&grid, &new_grid);
        }
}

void parallel_temp_dynamic_20(){
    for (int time = 0; time < MAXITER; time++)
    {
    #pragma omp parallel for num_threads(NTHREADS) schedule(dynamic,20) default(none) shared(grid, new_grid)
            for (int i = 1; i < N - 1; i++) // iterate grid but skip boundary
                for (int j = 1; j < N - 1; j++)
                {
                    new_grid[i * N + j] = (grid[(i + 1) * N + j] +
                                        grid[(i - 1) * N + j] +
                                        grid[i * N + j + 1] +
                                        grid[i * N + j - 1]) *
                                        0.25;
                }
            // Make new grid to current grid for the next generation
            swap_ptr(&grid, &new_grid);
        }
}

void parallel_temp_guided_1(){
    for (int time = 0; time < MAXITER; time++)
    {
    #pragma omp parallel for num_threads(NTHREADS) schedule(guided,1) default(none) shared(grid, new_grid)
            for (int i = 1; i < N - 1; i++) // iterate grid but skip boundary
                for (int j = 1; j < N - 1; j++)
                {
                    new_grid[i * N + j] = (grid[(i + 1) * N + j] +
                                        grid[(i - 1) * N + j] +
                                        grid[i * N + j + 1] +
                                        grid[i * N + j - 1]) *
                                        0.25;
                }
            // Make new grid to current grid for the next generation
            swap_ptr(&grid, &new_grid);
        }
}

void parallel_temp_guided_20(){
    for (int time = 0; time < MAXITER; time++)
    {
    #pragma omp parallel for num_threads(NTHREADS) schedule(guided,20) default(none) shared(grid, new_grid)
            for (int i = 1; i < N - 1; i++) // iterate grid but skip boundary
                for (int j = 1; j < N - 1; j++)
                {
                    new_grid[i * N + j] = (grid[(i + 1) * N + j] +
                                        grid[(i - 1) * N + j] +
                                        grid[i * N + j + 1] +
                                        grid[i * N + j - 1]) *
                                        0.25;
                }
            // Make new grid to current grid for the next generation
            swap_ptr(&grid, &new_grid);
        }
}

void parallel_temp_manual()
{
    for (int time = 0; time < MAXITER; time++)
    {
        #pragma omp parallel num_threads(NTHREADS) default(none) shared(grid, new_grid)
        {
            int my_rank = omp_get_thread_num();
            int chunk = (N - 2 - 1)/ omp_get_num_threads();
            int start = 1 + my_rank *chunk;
            int end = start + chunk;
            if(end > N-2){
                end = N-2;
            }

            for (int i = start; i < end; i++) // iterate grid but skip boundary
                for (int j = 1; j < N - 1; j++)
                {
                    new_grid[i * N + j] = (grid[(i + 1) * N + j] +
                                        grid[(i - 1) * N + j] +
                                        grid[i * N + j + 1] +
                                        grid[i * N + j - 1]) *
                                        0.25;
                }
        }
        // Make new grid to current grid for the next generation
        swap_ptr(&grid, &new_grid);
    }
}

int main(int argc, char *argv[])
{

    double start, end, serial, parallel;

    printf("Initialize grid size N=%d\n", N);
    init_temperatures();

    printf("Start Serial with MAXITER=%d\n", MAXITER);
    start = omp_get_wtime();
    serial_temp();
    end = omp_get_wtime();
    serial = end - start;
    printf("Serial Time %lf \n", serial);
    save_groundtruth(); // keep values from serial result as ground truth for later comparison

    printf("Initialize grid size N=%d\n", N);
    init_temperatures(); // init again the same grid for parallel version

    printf("Start Paralel with NTHREADS=%d\n", NTHREADS);
    start = omp_get_wtime();
    parallel_temp();
    end = omp_get_wtime();
    parallel = end - start;
    printf("Parallel Time %lf  Speedup %lf \n", parallel, serial / parallel);

    if (!equal_groundtruth())
        printf("!!! Parallel version produces a different result! \n\n");
    else
        printf("Parallel version produced the same result \n\n");


    printf("Initialize grid size N=%d\n", N);
    init_temperatures(); // init again the same grid for parallel version
    printf("Static 1 NTHREADS=%d\n", NTHREADS);
    start = omp_get_wtime();
    parallel_temp_static_1();
    end = omp_get_wtime();
    parallel = end - start;
    printf("Parallel Time %lf  Speedup %lf \n\n", parallel, serial / parallel);
    if (!equal_groundtruth())
        printf("!!! Parallel version produces a different result! \n\n");
    else
        printf("Parallel version produced the same result \n\n");

    printf("Initialize grid size N=%d\n", N);
    init_temperatures(); // init again the same grid for parallel version
    printf("Static 20 NTHREADS=%d\n", NTHREADS);
    start = omp_get_wtime();
    parallel_temp_static_20();
    end = omp_get_wtime();
    parallel = end - start;
    printf("Parallel Time %lf  Speedup %lf \n\n", parallel, serial / parallel);
    if (!equal_groundtruth())
        printf("!!! Parallel version produces a different result! \n\n");
    else
        printf("Parallel version produced the same result \n\n");

    printf("Initialize grid size N=%d\n", N);
    init_temperatures(); // init again the same grid for parallel version
    printf("Dynamic 1 NTHREADS=%d\n", NTHREADS);
    start = omp_get_wtime();
    parallel_temp_dynamic_1();
    end = omp_get_wtime();
    parallel = end - start;
    printf("Parallel Time %lf  Speedup %lf \n\n", parallel, serial / parallel);
    if (!equal_groundtruth())
        printf("!!! Parallel version produces a different result! \n\n");
    else
        printf("Parallel version produced the same result \n\n");

    printf("Initialize grid size N=%d\n", N);
    init_temperatures(); // init again the same grid for parallel version
    printf("Dynamic 20 NTHREADS=%d\n", NTHREADS);
    start = omp_get_wtime();
    parallel_temp_dynamic_20();
    end = omp_get_wtime();
    parallel = end - start;
    printf("Parallel Time %lf  Speedup %lf \n\n", parallel, serial / parallel);
    if (!equal_groundtruth())
        printf("!!! Parallel version produces a different result! \n\n");
    else
        printf("Parallel version produced the same result \n\n");

    printf("Initialize grid size N=%d\n", N);
    init_temperatures(); // init again the same grid for parallel version
    printf("Guided 1 NTHREADS=%d\n", NTHREADS);
    start = omp_get_wtime();
    parallel_temp_guided_1();
    end = omp_get_wtime();
    parallel = end - start;
    printf("Parallel Time %lf  Speedup %lf \n\n", parallel, serial / parallel);
    if (!equal_groundtruth())
        printf("!!! Parallel version produces a different result! \n\n");
    else
        printf("Parallel version produced the same result \n\n");

    printf("Initialize grid size N=%d\n", N);
    init_temperatures(); // init again the same grid for parallel version
    printf("Guided 20 NTHREADS=%d\n", NTHREADS);
    start = omp_get_wtime();
    parallel_temp_guided_20();
    end = omp_get_wtime();
    parallel = end - start;
    printf("Parallel Time %lf  Speedup %lf \n\n", parallel, serial / parallel);
    if (!equal_groundtruth())
        printf("!!! Parallel version produces a different result! \n\n");
    else
        printf("Parallel version produced the same result \n\n");

    printf("Initialize grid size N=%d\n", N);
    init_temperatures(); // init again the same grid for parallel version
    printf("Manual NTHREADS=%d\n", NTHREADS);
    start = omp_get_wtime();
    parallel_temp_manual();
    end = omp_get_wtime();
    parallel = end - start;
    printf("Parallel Time %lf  Speedup %lf \n\n", parallel, serial / parallel);
    if (!equal_groundtruth())
        printf("!!! Parallel version produces a different result! \n\n");
    else
        printf("Parallel version produced the same result \n\n");

}