#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "mpi.h"

void merge(int *arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *left = (int *)malloc(n1 * sizeof(int));
    int *right = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        left[i] = arr[l + i];

    for (j = 0; j < n2; j++)
        right[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void mergeSort(int *arr, int l, int r)
{
    if (l >= r)
        return;

    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int main(int argc, char **argv)
{
    const intptr_t N = 1e4;
    int size, rank, i, sub_size;
    double time_start, time_end;
    int *s_array = nullptr;
    int *sorted_array  = nullptr;
    int *array = (int *)malloc(N * sizeof(int));

    srandom(time(NULL));
    for (i = 0; i < N; ++i)
    {
        array[i] = random() % 200 - 100;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    sub_size = N / size;

    MPI_Barrier(MPI_COMM_WORLD);
    time_start = MPI_Wtime();

    s_array = (int *)malloc(sub_size * sizeof(int));
    MPI_Scatter(array, sub_size, MPI_INT, s_array, sub_size, MPI_INT, 0, MPI_COMM_WORLD);

    mergeSort(s_array, 0, sub_size - 1);

    if (rank == 0)
    {
        sorted_array = (int *)malloc(N * sizeof(int));
    }
    MPI_Gather(s_array, sub_size, MPI_INT, sorted_array, sub_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        mergeSort(sorted_array, 0, N - 1);
        time_end = MPI_Wtime();
        
        printf("\nTime: %f \n", time_end - time_start);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
