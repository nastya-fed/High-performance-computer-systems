#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"
#include <time.h>

int main(int argc, char **argv)
{
    const intptr_t MAX_N = 256;
    
    int size, rank;
    MPI_Status status;
    int rows, offset, x;
    double time_start, time_end;
    int A[MAX_N][MAX_N], B[MAX_N][MAX_N],C[MAX_N][MAX_N];

    srand(time(NULL));

    for (int i = 0; i < MAX_N; i++)
        for (int j = 0; j < MAX_N; j++)
        {
            A[i][j] = 100 - rand() % 200;
            B[i][j] = 100 - rand() % 200;
        }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        time_start = MPI_Wtime();

        rows = MAX_N / size;
        offset = rows;

        for (int i = 1; i < size; i++)
        {
            MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&A[offset][0], rows * MAX_N, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&B, MAX_N * MAX_N, MPI_INT, i, 1, MPI_COMM_WORLD);

            offset = offset + rows;
        }

        for (int k = 0; k < MAX_N; k++)
            for (int m = 0; m < rows; m++)
            {
                C[m][k] = 0;
                for (int j = 0; j < MAX_N; j++)
                    C[m][k] = C[m][k] + A[m][j] * B[j][k];
            }

        for (x = 1; x < size; x++)
        {
            MPI_Recv(&offset, 1, MPI_INT, x, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, x, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[offset][0], rows * MAX_N, MPI_INT, x, 2, MPI_COMM_WORLD, &status);
        }

        time_end = MPI_Wtime();


        printf("\nTime: %f \n", time_end - time_start);

    }
    else
    {
        x = 0;

        MPI_Recv(&offset, 1, MPI_INT, x, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, x, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&A, rows * MAX_N, MPI_INT, x, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&B, MAX_N * MAX_N, MPI_INT, x, 1, MPI_COMM_WORLD, &status);

        for (int k = 0; k < MAX_N; k++)
            for (int i = 0; i < rows; i++)
            {
                C[i][k] = 0;
                for (int j = 0; j < MAX_N; j++)
                    C[i][k] = C[i][k] + A[i][j] * B[j][k];
            }

        MPI_Send(&offset, 1, MPI_INT, x, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, x, 2, MPI_COMM_WORLD);
        MPI_Send(&C, rows * MAX_N, MPI_INT, x, 2, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    delete[] A, B, C;
    return 0;
}