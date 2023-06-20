#include <iostream>
#include <omp.h>

void merge(intptr_t* arr, const intptr_t p, const intptr_t q, const intptr_t r) {
    const intptr_t N1 = q - p + 1;
    const intptr_t N2 = r - q;

    intptr_t* L = new intptr_t[N1];
    intptr_t* M = new intptr_t[N2];

    for (intptr_t i = 0; i < N1; i++) {
        L[i] = arr[p + i];
    }

    for (intptr_t j = 0; j < N2; j++) {
        M[j] = arr[q + 1 + j];
    }

    intptr_t i = 0, j = 0, k = p;

    while (i < N1 && j < N2) {
        if (L[i] <= M[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = M[j];
            j++;
        }
        k++;
    }

    while (i < N1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < N2) {
        arr[k] = M[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] M;
}

void MergeSort(intptr_t* arr, const intptr_t l, const intptr_t r) {
    if (l < r) {

    intptr_t m = l + (r - l) / 2;

#pragma omp task
     MergeSort(arr, l, m);
#pragma omp task
     MergeSort(arr, m + 1, r);

     merge(arr, l, m, r);
        
    }
}

int main() {
    
    const intptr_t N = 1e6;
    const intptr_t min_rand = -100;
    const intptr_t max_rand = 100;
    
    intptr_t* arr = new intptr_t[N];
    
    srand(time(NULL));

   
    for (intptr_t i = 0; i < N; ++i) {
        arr[i] = min_rand + rand() % (max_rand - min_rand + 1);
    }

    double time_start_p = omp_get_wtime();
#pragma omp parallel
    {
    #pragma omp single
    MergeSort(arr, 0, N - 1);
    }
    double time_end_p = omp_get_wtime();

    std::cout << "Time of sort "<< N;
    std::cout<<" (Parallel):" << time_end_p - time_start_p << std::endl;
    
    delete arr;
    return 0;
}
