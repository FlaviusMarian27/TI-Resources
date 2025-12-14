/*
N-Queens problem

This program generates parallel tasks starting from first k rows placements.

Compile with:
gcc -g mpi_queens.c -I "c:\Program Files (x86)\Microsoft SDKs\MPI\Include" -L "c:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -lmsmpi -o mpi_queens.exe

*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define WORK_TAG 1
#define FINAL_TAG 2

// #define DEBUG

/* checks if a queen can be placed at row=depth and col */
int safe(int *pos, int depth, int col)
{
    for (int i = 0; i < depth; i++)
    {
        if (pos[i] == col ||
            abs(pos[i] - col) == abs(depth - i))
            return 0;
    }
    return 1;
}

/* Backtracking N-queens from a given partial configuration */
long solve(int *pos, int depth, int N)
{
    // queens are already placed in all rows until depth-1
     // pos[i] = the column of the queen from row i

    if (depth == N)
        return 1; // found a solution

    // for row=depth, explore all columns col where a queen can be placed
    long count = 0;
    for (int col = 0; col < N; col++)
    {
        if (safe(pos, depth, col))
        {
            pos[depth] = col;
            count += solve(pos, depth + 1, N);
        }
    }
    return count;
}

//functie pt gasirea primei solutii----------------
int solve_first(int *pos, int depth, int N){
    if(depth == N){
        return 1;
    }

    for(int col = 0; col < N; col++){
        if(safe(pos,depth,col)){
            pos[depth] = col;
            if(solve_first(pos,depth + 1,N)){
                return 1;
            }
        }
    }
    return 0;
}

/* Recursive generator for pos arrays after expanding k levels.
Executes the first k levels of the solution backtracking algo.
Fills out the array tasks, tasks[i] contains pos used as start.
The number of tasks will be in count. */
void gen_k_tasks_rec(int **tasks, int *count, 
                     int *pos, int depth, int k, int N)
{
    if (depth == k)
    {  
        int *task = malloc(k * sizeof(int));
        for (int i = 0; i < k; i++)
            task[i] = pos[i];
        tasks[(*count)++] = task; // add a new task to tasks array
        return;
    }
    for (int col = 0; col < N; col++)
    {
        if (safe(pos, depth, col))
        {
            pos[depth] = col;
            gen_k_tasks_rec(tasks, count,  pos, depth + 1, k, N);
        }
    }
}

/* Generates tasks from level k.
Returns an array of "tasks". Each "task" is a pos 
array containing Â the positions of the first k queens. */

int** generate_k_row_tasks( int N, int k, int *count)
{
    int capacity = 1;
    for (int i = 1; i <= k; i++)
        capacity = capacity * N; // capacity = N^k
    int **tasks = malloc(capacity * sizeof(int *)); // an array of pos arrays
    
    *count = 0; // actual number of nodes=tasks

    int *pos = calloc(N, sizeof(int)); // init pos vector

    gen_k_tasks_rec(tasks, count, pos, 0, k, N);

    free(pos);
    
    return tasks;
}

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3)
    {
        if (rank == 0)
            printf("arguments: N k\n");
        MPI_Finalize();
        return 1;
    }

    int N = atoi(argv[1]);
    int k = atoi(argv[2]);
    if (k < 1 || k > N)
    {
        if (rank == 0)
            printf("k must be between 1 and N.\n");
        MPI_Finalize();
        return 1;
    }

    double start = MPI_Wtime();
    if (rank == 0)
    {
        /* ---------------- MASTER ---------------- */

        int **tasks;
        int num_tasks;
        tasks = generate_k_row_tasks( N, k, &num_tasks);
#ifdef DEBUG
        printf("generated %d tasks  \n", num_tasks);
        fflush(stdout);
#endif
        int next = 0;
        int num_workers = size - 1;

        // send initial tasks
        for (int w = 1; w <= num_workers && next < num_tasks; w++)
        {
            MPI_Send(tasks[next], k, MPI_INT, w, WORK_TAG, MPI_COMM_WORLD);
            next++;
        }

        // asteptam pt prima solutie
        int *solution = malloc(N * sizeof(int));
        MPI_Recv(solution, N, MPI_INT,MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

        double t = MPI_Wtime();
        printf("First solution found by worker %d\n", status.MPI_SOURCE);
        printf("Row -> Col: ");
        for (int i = 0; i < N; i++){
            printf("%d", solution[i]);
            if (i == N - 1){
                printf("\n");
            }else{
                printf(" ");
            }
        }
            
        printf("Time: %.6f seconds\n", t - start);

        //stop
        MPI_Abort(MPI_COMM_WORLD, 0);
    }
    else
    {
        /* ---------------- WORKER ---------------- */
        while (1){
            int *task = malloc(N * sizeof(int));
            MPI_Recv(task, N, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            int ok = solve_first(task,k,N);
            if(ok == 1){
                MPI_Send(task, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Abort(MPI_COMM_WORLD, 0);
            }

            free(task);
        }
    }

    MPI_Finalize();
    return 0;
}

/*
mpicc -o queens queens.c
mpirun -np 2 ./queens 4 4
*/