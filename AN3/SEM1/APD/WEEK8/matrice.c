#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define EPSILON 0.000001

double **alocareMatrice(int N){
    double **M = (double**)malloc(sizeof(double*) * N);
    if(M == NULL){
        perror("Eroare alocare pentru linii!\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < N; i++){
        M[i] = (double*)malloc(sizeof(double) * N);
        if(M[i] == NULL){
            perror("Eroare alocare pt coloane!\n");
            exit(EXIT_FAILURE);
        }
    }

    return M;
}

void freeMemory(int N, double **M){
    for(int i = 0; i < N; i++){
        free(M[i]);
    }
    free(M);
}

void printMatrix(int N, double **M){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.2lf ", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void initilizareMatrice(int N, double **M){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            M[i][j] = 0;
        }
    }
}

void generareValoriRandom(int N, double **M){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            M[i][j] = (double)(rand()%100)/10;
        }
    }
}

int Equal_matrixes(double **M1, double **M2, int N){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (fabs(M1[i][j] - M2[i][j]) > EPSILON){
                return 0;
            }
        }
    }
    return 1;
}

void serial_multiply_ijk_v1(int N, double **A, double **B, double **C_serial){
    int i,j,k;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            for(k = 0; k < N; k++){
                C_serial[i][j] = C_serial[i][j] + A[i][k] * B[k][j];
            }
        }
    }
}

void parallel_multiply_ijk_v1(int N, double **A, double **B, double **C_parallel,
                                int chunk_size, int nr_threads){

    int i, j,k;
#pragma omp parallel num_threads(nr_threads) default(none) private(i,j,k) shared(N,A,B,C_parallel,chunk_size)
    {
        #pragma omp for schedule(static,chunk_size)
        for(i = 0; i < N; i++){
            for(j = 0; j < N; j++){
                for(k = 0; k < N; k++){
                    C_parallel[i][j] = C_parallel[i][j] + A[i][k] * B[k][j];
                }
            }
        }
    }
}

void serial_multiply_ikj_v2(int N, double **A, double **B, double **C_serial){
    int i,j,k;
    for(i = 0; i < N; i++){
        for(k = 0; k < N; k++){
            for(j = 0; j < N; j++){
                C_serial[i][j] = C_serial[i][j] + A[i][k] * B[k][j];
            }
        }
    }
}

void parallel_multiply_ikj_v2(int N, double **A, double **B, double **C_parallel,
                                int chunk_size, int nr_threads){

    int i, j,k;
#pragma omp parallel num_threads(nr_threads) default(none) private(i,j,k) shared(N,A,B,C_parallel,chunk_size)
    {
        #pragma omp for schedule(static,chunk_size)
        for(i = 0; i < N; i++){
            for(k = 0; k < N; k++){
                for(j = 0; j < N; j++){
                    C_parallel[i][j] = C_parallel[i][j] + A[i][k] * B[k][j];
                }
            }
        }
    }
}

void serial_multiply_jik_v3(int N, double **A, double **B, double **C_serial){
    int i, j, k;
    for(j = 0; j < N; j++){
        for(i = 0; i < N; i++){
            for(k = 0; k < N; k++){
                C_serial[i][j] = C_serial[i][j] + A[i][k] * B[k][j];
            }
        }
    }
}

void parallel_multiply_jik_v3(int N, double **A, double **B, double **C_parallel,
                            int chunk_size, int nr_threads){
    int i, j, k;
#pragma omp parallel num_threads(nr_threads) default(none) private(i,j,k) shared(N,A,B,C_parallel,chunk_size)
    {
        #pragma omp for schedule(static,chunk_size)
        for(j = 0; j < N; j++){
            for(i = 0; i < N; i++){
                for (k = 0; k < N; k++){
                    C_parallel[i][j] = C_parallel[i][j] + A[i][k] * B[k][j];
                }
            }
        }
    }
}

void serial_multiply_jki_v4(int N, double **A, double **B, double **C_serial){
    int i, j, k;
    for(j = 0; j < N; j++){
        for(k = 0; k < N; k++){
            for(i = 0; i < N; i++){
                C_serial[i][j] = C_serial[i][j] + A[i][k] * B[k][j];
            }
        }
    }
}

void parallel_multiply_jki_v4(int N, double **A, double **B, double **C_parallel,
                            int chunk_size, int nr_threads){
    int i, j, k;
#pragma omp parallel num_threads(nr_threads) default(none) private(i,j,k) shared(N,A,B,C_parallel,chunk_size)
    {
        #pragma omp for schedule(static,chunk_size)
        for(j = 0; j < N; j++){
            for(k = 0; k < N; k++){
                for(i = 0; i < N; i++){
                    C_parallel[i][j] = C_parallel[i][j] + A[i][k] * B[k][j];
                }
            }
        }
    }
}

void serial_multiply_kij_v5(int N, double **A, double **B, double **C_serial){
    int i, j, k;
    for(k = 0; k < N; k++){
        for(i = 0; i < N; i++){
            for(j = 0; j < N; j++){
                C_serial[i][j] = C_serial[i][j] + A[i][k] * B[k][j];
            }
        }
    }
}

void parallel_multiply_kij_v5(int N, double **A, double **B, double **C_parallel,
                            int chunk_size, int nr_threads){
    int i, j, k;
#pragma omp parallel num_threads(nr_threads) default(none) private(i,j,k) shared(N,A,B,C_parallel,chunk_size)
    {
        #pragma omp for schedule(static,chunk_size)
        for(k = 0; k < N; k++){
            for(i = 0; i < N; i++){
                for(j = 0; j < N; j++){
                    #pragma omp atomic
                    C_parallel[i][j] = C_parallel[i][j] + A[i][k] * B[k][j];
                }
            }
        }
    }
}

void serial_multiply_kji_v6(int N, double **A, double **B, double **C_serial){
    int i, j, k;
    for(k = 0; k < N; k++){
        for(j = 0; j < N; j++){
            for(i = 0; i < N; i++){
                C_serial[i][j] = C_serial[i][j] + A[i][k] * B[k][j];
            }
        }
    }
}

void parallel_multiply_kji_v6(int N, double **A, double **B, double **C_parallel,
                            int chunk_size, int nr_threads){
    int i, j, k;
#pragma omp parallel num_threads(nr_threads) default(none) private(i,j,k) shared(N,A,B,C_parallel,chunk_size)
    {
        #pragma omp for schedule(static,chunk_size)
        for(k = 0; k < N; k++){
            for(j = 0; j < N; j++){
                for(i = 0; i < N; i++){
                    #pragma omp atomic
                    C_parallel[i][j] = C_parallel[i][j] + A[i][k] * B[k][j];
                }
            }
        }
    }
}

void serial_blocked(int N, double **A, double **B, double **C_serial, int q){
    int i, j, k;
    int i_start, i_end;
    int j_start, j_end;
    int k_start, k_end;

    int block_size = N / q;
    int i_block, j_block, k_block;

    for (i_block = 0; i_block < q; i_block++){

        i_start = i_block * block_size;
        i_end = (i_block + 1) * block_size;

        for (j_block = 0; j_block < q; j_block++){

            j_start = j_block * block_size;
            j_end = (j_block + 1) * block_size;

            for (k_block = 0; k_block < q; k_block++){

                k_start = k_block * block_size;
                k_end = (k_block + 1) * block_size;

                for (i = i_start; i < i_end; i++){
                    for (k = k_start; k < k_end; k++){
                        for (j = j_start; j < j_end; j++){
                            C_serial[i][j] = C_serial[i][j] + A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

void parallel_blocked(int N, double **A, double **B, double **C_parallel, int q, int nr_threads){
    int i, j, k;
    int i_start, i_end;
    int j_start, j_end;
    int k_start, k_end;

    int block_size = N / q;
    int i_block, j_block, k_block;

#pragma omp parallel for num_threads(nr_threads) default(none) private(i,j,k,i_start,i_end,j_start,j_end,k_start,k_end,i_block,j_block,k_block) shared(N,A,B,C_parallel,q,block_size)
    for (i_block = 0; i_block < q; i_block++){

        i_start = i_block * block_size;
        i_end = (i_block + 1) * block_size;

        for (j_block = 0; j_block < q; j_block++){

            j_start = j_block * block_size;
            j_end = (j_block + 1) * block_size;

            for (k_block = 0; k_block < q; k_block++){

                k_start = k_block * block_size;
                k_end = (k_block + 1) * block_size;

                for (i = i_start; i < i_end; i++){
                    for (k = k_start; k < k_end; k++){
                        for (j = j_start; j < j_end; j++){
                            C_parallel[i][j] = C_parallel[i][j] + A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr,"Usage %s nr_threads dim_matrice\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    int nr_threaduri = atoi(argv[1]);
    int N = atoi(argv[2]);
    int chunk_size = 5;

    double **A = alocareMatrice(N);
    double **B = alocareMatrice(N);
    double **C_serial = alocareMatrice(N);
    double **C_parallel = alocareMatrice(N);
    double **C_ijk = alocareMatrice(N);

    srand(time(NULL));
    generareValoriRandom(N,A);
    generareValoriRandom(N,B);

    printf("Number threads: %d\n", nr_threaduri);
    printf("Total size: %d\n",N);
    printf("Chunk size: %d\n",chunk_size);

    printf("\n================V1================\n");
    initilizareMatrice(N,C_serial);
    initilizareMatrice(N,C_parallel);
    initilizareMatrice(N,C_ijk);
    //printMatrix(N,A);
    double start = omp_get_wtime();
    serial_multiply_ijk_v1(N,A,B,C_ijk);
    double end = omp_get_wtime();
    double timp_serial = end - start;
    printf("Timp serial i-j-k: %f sec\n", timp_serial);

    start = omp_get_wtime();
    parallel_multiply_ijk_v1(N,A,B,C_parallel,chunk_size,nr_threaduri);
    end = omp_get_wtime();
    double timp_parallel = end - start;
    printf("Timp paralel i-j-k: %f sec\n",timp_parallel);

    printf("Speedup = %2.2lf sec\n", timp_serial / timp_parallel);
    if(!Equal_matrixes(C_ijk,C_parallel, N)){
        printf("V1 is not the same!\n");
    }else{
        printf("V1 same result!\n");
    }

    //printMatrix(N,A);
    //printMatrix(N,B);
    //printMatrix(N,C_serial);
    printf("\n================V2================\n");

    initilizareMatrice(N,C_serial);
    initilizareMatrice(N,C_parallel);
    start = omp_get_wtime();
    serial_multiply_ikj_v2(N,A,B,C_serial);
    end = omp_get_wtime();
    timp_serial = end - start;
    printf("Timp serial i-k-j: %f sec\n",timp_serial);

    start = omp_get_wtime();
    parallel_multiply_ikj_v2(N,A,B,C_parallel,chunk_size,nr_threaduri);
    end = omp_get_wtime();
    timp_parallel = end - start;
    printf("Timp paralel i-k-j: %f sec\n", timp_parallel);

    printf("Speedup = %2.2lf sec\n", timp_serial / timp_parallel);
    if(!Equal_matrixes(C_serial,C_parallel, N)){
        printf("V2 is not the same!\n");
    }else{
        printf("V2 same result!\n");
    }

    if(!Equal_matrixes(C_serial,C_ijk, N)){
        printf("Is not the same result with ijk!\n");
    }else{
        printf("Is the same result with ijk\n");
    }

    printf("\n================V3================\n");

    initilizareMatrice(N,C_serial);
    initilizareMatrice(N,C_parallel);
    start = omp_get_wtime();
    serial_multiply_jik_v3(N,A,B,C_serial);
    end = omp_get_wtime();
    timp_serial = end - start;
    printf("Timp serial j-i-k: %f sec\n",timp_serial);

    start = omp_get_wtime();
    parallel_multiply_jik_v3(N,A,B,C_parallel,chunk_size,nr_threaduri);
    end = omp_get_wtime();
    timp_parallel = end - start;
    printf("Timp paralel j-i-k: %f sec\n", timp_parallel);

    printf("Speedup = %2.2lf sec\n", timp_serial / timp_parallel);
    if(!Equal_matrixes(C_serial,C_parallel, N)){
        printf("V3 is not the same!\n");
    }else{
        printf("V3 same result!\n");
    }

    if(!Equal_matrixes(C_serial,C_ijk, N)){
        printf("Is not the same result with ijk!\n");
    }else{
        printf("Is the same result with ijk\n");
    }

    printf("\n================V4================\n");

    initilizareMatrice(N,C_serial);
    initilizareMatrice(N,C_parallel);
    start = omp_get_wtime();
    serial_multiply_jki_v4(N,A,B,C_serial);
    end = omp_get_wtime();
    timp_serial = end - start;
    printf("Timp serial j-k-i: %f sec\n",timp_serial);

    start = omp_get_wtime();
    parallel_multiply_jki_v4(N,A,B,C_parallel,chunk_size,nr_threaduri);
    end = omp_get_wtime();
    timp_parallel = end - start;
    printf("Timp paralel j-k-i: %f sec\n", timp_parallel);

    printf("Speedup = %2.2lf sec\n", timp_serial / timp_parallel);
    if(!Equal_matrixes(C_serial,C_parallel, N)){
        printf("V4 is not the same!\n");
    }else{
        printf("V4 same result!\n");
    }

    if(!Equal_matrixes(C_serial,C_ijk, N)){
        printf("Is not the same result with ijk!\n");
    }else{
        printf("Is the same result with ijk\n");
    }

    printf("\n================V5================\n");

    initilizareMatrice(N,C_serial);
    initilizareMatrice(N,C_parallel);
    start = omp_get_wtime();
    serial_multiply_kij_v5(N,A,B,C_serial);
    end = omp_get_wtime();
    timp_serial = end - start;
    printf("Timp serial k-i-j: %f sec\n",timp_serial);

    start = omp_get_wtime();
    parallel_multiply_kij_v5(N,A,B,C_parallel,chunk_size,nr_threaduri);
    end = omp_get_wtime();
    timp_parallel = end - start;
    printf("Timp paralel k-i-j: %f sec\n", timp_parallel);

    printf("Speedup = %2.2lf sec\n", timp_serial / timp_parallel);
    if(!Equal_matrixes(C_serial,C_parallel, N)){
        printf("V5 is not the same!\n");
    }else{
        printf("V5 same result!\n");
    }

    if(!Equal_matrixes(C_serial,C_ijk, N)){
        printf("Is not the same result with ijk!\n");
    }else{
        printf("Is the same result with ijk\n");
    }


    printf("\n================V6================\n");

    initilizareMatrice(N,C_serial);
    initilizareMatrice(N,C_parallel);
    start = omp_get_wtime();
    serial_multiply_kji_v6(N,A,B,C_serial);
    end = omp_get_wtime();
    timp_serial = end - start;
    printf("Timp serial k-j-i: %f sec\n",timp_serial);

    start = omp_get_wtime();
    parallel_multiply_kji_v6(N,A,B,C_parallel,chunk_size,nr_threaduri);
    end = omp_get_wtime();
    timp_parallel = end - start;
    printf("Timp paralel k-j-i: %f sec\n", timp_parallel);

    printf("Speedup = %2.2lf sec\n", timp_serial / timp_parallel);
    if(!Equal_matrixes(C_serial,C_parallel, N)){
        printf("V6 is not the same!\n");
    }else{
        printf("V6 same result!\n");
    }

    if(!Equal_matrixes(C_serial,C_ijk, N)){
        printf("Is not the same result with ijk!\n");
    }else{
        printf("Is the same result with ijk\n");
    }


    printf("\n============Blocked============\n");

    int q = 50;
    initilizareMatrice(N,C_serial);
    initilizareMatrice(N,C_parallel);
    start = omp_get_wtime();
    serial_blocked(N,A,B,C_serial,q);
    end = omp_get_wtime();
    timp_serial = end - start;
    printf("Timp serial blocked: %f sec\n",timp_serial);

    start = omp_get_wtime();
    parallel_blocked(N,A,B,C_parallel,q,nr_threaduri);
    end = omp_get_wtime();
    timp_parallel = end - start;
    printf("Timp paralel blocked: %f sec\n", timp_parallel);

    printf("Speedup = %2.2lf sec\n", timp_serial / timp_parallel);
    if(!Equal_matrixes(C_serial,C_parallel, N)){
        printf("Blocked is not the same!\n");
    }else{
        printf("Blocked same result!\n");
    }

    if(!Equal_matrixes(C_serial,C_ijk, N)){
        printf("Is not the same result with ijk!\n");
    }else{
        printf("Is the same result with ijk\n");
    }

    freeMemory(N,A);
    freeMemory(N,B);
    freeMemory(N,C_serial);
    freeMemory(N,C_parallel);
    freeMemory(N,C_ijk);

    return 0;
}