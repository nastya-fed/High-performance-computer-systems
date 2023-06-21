#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    
    int rank, size;
    double pi = .0;
    double lok_pi = .0;
    const int n = 1e6;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Barrier(MPI_COMM_WORLD);
    
    double time_start = MPI_Wtime();

    for (int i = rank; i < n; i += size)
    {
        pi += 4 / (1 + ((i + 0.5) / n) * ((i + 0.5) / n));
    }

    MPI_Reduce(&pi, &lok_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double time_end = MPI_Wtime();
   
    if (rank == 0)
    {
        printf("PI: %.7f\n", lok_pi / n);
        printf("Time: %.7f\n", time_end - time_start);
    }

    MPI_Finalize();
    return 0;
}