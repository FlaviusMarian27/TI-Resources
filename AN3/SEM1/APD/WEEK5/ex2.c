/**
 * Compute the Area of the Mandelbrot Set
 *
 */
#include <stdio.h>
#include <omp.h>

#define NPOINTS 1600
#define MAXITER 1000
#define THREAD_COUNT 8

struct complex
{
    double r;
    double i;
};

void compute_serial(double *area, double *error)
{
    int numinside = 0, numoutside = 0;
    for (int i = 0; i < NPOINTS; i++)
        for (int j = 0; j < NPOINTS; j++)
        {
            // generate grid of points C in the rectangle
            // C.r in [-2  .. 0.5]
            // C.i in [0 .. 1.125 ] - will be taken also symmetric Ox
            struct complex c;
            c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS);
            c.i = 1.125 * (double)(j) / (double)(NPOINTS);
            struct complex z;
            z = c; // start computing series z for c
            for (int iter = 0; iter < MAXITER; iter++)
            {
                double temp = (z.r * z.r) - (z.i * z.i) + c.r;
                z.i = z.r * z.i * 2 + c.i;
                z.r = temp;
                if ((z.r * z.r + z.i * z.i) > 4.0)
                { // z diverges
                    numoutside++;
                    break;
                }
            }
        }
    numinside = NPOINTS * NPOINTS - numoutside;
    *area = 2.0 * 2.5 * 1.125 * (double)(numinside) / (double)(NPOINTS * NPOINTS);
    *error = *area / (double)NPOINTS;
}

void compute_paralel_static_1(double *area, double *error){
    int numinside = 0, numoutside = 0;

    #pragma omp parallel for num_threads(THREAD_COUNT) reduction(+:numoutside) schedule(static,1)
        for (int i = 0; i < NPOINTS; i++){
            for (int j = 0; j < NPOINTS; j++)
                {
                    struct complex c;
                    c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS);
                    c.i = 1.125 * (double)(j) / (double)(NPOINTS);
                    struct complex z;
                    z = c; // start computing series z for c
                    for (int iter = 0; iter < MAXITER; iter++)
                    {
                        double temp = (z.r * z.r) - (z.i * z.i) + c.r;
                        z.i = z.r * z.i * 2 + c.i;
                        z.r = temp;
                        if ((z.r * z.r + z.i * z.i) > 4.0)
                        { // z diverges
                            numoutside++;
                            break;
                        }
                    }
                }
        }
            

    numinside = NPOINTS * NPOINTS - numoutside;
    *area = 2.0 * 2.5 * 1.125 * (double)(numinside) / (double)(NPOINTS * NPOINTS);
    *error = *area / (double)NPOINTS;
}

void compute_paralel_dynamic_1(double *area, double *error){
    int numinside = 0, numoutside = 0;

    #pragma omp parallel for num_threads(THREAD_COUNT) reduction(+:numoutside) schedule(dynamic,1)
        for (int i = 0; i < NPOINTS; i++){
            for (int j = 0; j < NPOINTS; j++)
                {
                    struct complex c;
                    c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS);
                    c.i = 1.125 * (double)(j) / (double)(NPOINTS);
                    struct complex z;
                    z = c; // start computing series z for c
                    for (int iter = 0; iter < MAXITER; iter++)
                    {
                        double temp = (z.r * z.r) - (z.i * z.i) + c.r;
                        z.i = z.r * z.i * 2 + c.i;
                        z.r = temp;
                        if ((z.r * z.r + z.i * z.i) > 4.0)
                        { // z diverges
                            numoutside++;
                            break;
                        }
                    }
                }
        }
            

    numinside = NPOINTS * NPOINTS - numoutside;
    *area = 2.0 * 2.5 * 1.125 * (double)(numinside) / (double)(NPOINTS * NPOINTS);
    *error = *area / (double)NPOINTS;
}

void compute_paralel_static_20(double *area, double *error){
    int numinside = 0, numoutside = 0;

    #pragma omp parallel for num_threads(THREAD_COUNT) reduction(+:numoutside) schedule(static,20)
        for (int i = 0; i < NPOINTS; i++){
            for (int j = 0; j < NPOINTS; j++)
                {
                    struct complex c;
                    c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS);
                    c.i = 1.125 * (double)(j) / (double)(NPOINTS);
                    struct complex z;
                    z = c; // start computing series z for c
                    for (int iter = 0; iter < MAXITER; iter++)
                    {
                        double temp = (z.r * z.r) - (z.i * z.i) + c.r;
                        z.i = z.r * z.i * 2 + c.i;
                        z.r = temp;
                        if ((z.r * z.r + z.i * z.i) > 4.0)
                        { // z diverges
                            numoutside++;
                            break;
                        }
                    }
                }
        }
            

    numinside = NPOINTS * NPOINTS - numoutside;
    *area = 2.0 * 2.5 * 1.125 * (double)(numinside) / (double)(NPOINTS * NPOINTS);
    *error = *area / (double)NPOINTS;
}

void compute_paralel_dynamic_20(double *area, double *error){
    int numinside = 0, numoutside = 0;

    #pragma omp parallel for num_threads(THREAD_COUNT) reduction(+:numoutside) schedule(dynamic,20)
        for (int i = 0; i < NPOINTS; i++){
            for (int j = 0; j < NPOINTS; j++)
                {
                    struct complex c;
                    c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS);
                    c.i = 1.125 * (double)(j) / (double)(NPOINTS);
                    struct complex z;
                    z = c; // start computing series z for c
                    for (int iter = 0; iter < MAXITER; iter++)
                    {
                        double temp = (z.r * z.r) - (z.i * z.i) + c.r;
                        z.i = z.r * z.i * 2 + c.i;
                        z.r = temp;
                        if ((z.r * z.r + z.i * z.i) > 4.0)
                        { // z diverges
                            numoutside++;
                            break;
                        }
                    }
                }
        }
            

    numinside = NPOINTS * NPOINTS - numoutside;
    *area = 2.0 * 2.5 * 1.125 * (double)(numinside) / (double)(NPOINTS * NPOINTS);
    *error = *area / (double)NPOINTS;
}

int main()
{
    double area, error;
    double start, time;

    printf("Serial version:...\n");
    start = omp_get_wtime();
    compute_serial(&area, &error);
    time = omp_get_wtime() - start;
    printf("Serial :  area=%f  error=%f   time=%f \n\n", area, error, time);

    printf("Parallel version(static,1):...\n");
    start = omp_get_wtime();
    compute_paralel_static_1(&area, &error);
    time = omp_get_wtime() - start;
    printf("Paralel :  area=%f  error=%f   time=%f \n\n", area, error, time);

    printf("Parallel version(dynamic,1):...\n");
    start = omp_get_wtime();
    compute_paralel_dynamic_1(&area, &error);
    time = omp_get_wtime() - start;
    printf("Paralel :  area=%f  error=%f   time=%f \n\n", area, error, time);

    printf("Parallel version(static,20):...\n");
    start = omp_get_wtime();
    compute_paralel_static_20(&area, &error);
    time = omp_get_wtime() - start;
    printf("Paralel :  area=%f  error=%f   time=%f \n\n", area, error, time);

    printf("Parallel version(dynamic,20):...\n");
    start = omp_get_wtime();
    compute_paralel_dynamic_20(&area, &error);
    time = omp_get_wtime() - start;
    printf("Paralel :  area=%f  error=%f   time=%f \n\n", area, error, time);

    return 0;
}