#include <iostream>
#include <omp.h>


void task0(intptr_t n, int method) {
    intptr_t sum = 0;
    intptr_t* A = new intptr_t[n];
    double time_start = 0;

    for (intptr_t i = 0; i < n; ++i)
        A[i] = i;

    if (method == 1) {
        std::cout << "Atomic" << std::endl;
        time_start = omp_get_wtime();

#pragma omp parallel for
        for (intptr_t i = 0; i < n; ++i) {
#pragma omp atomic
            sum += A[i];
        }
    }
    else if (method == 2) {
        std::cout << "Reduction" << std::endl;
        time_start = omp_get_wtime();

#pragma omp parallel for reduction(+:sum)
        for (intptr_t i = 0; i < n; ++i)
            sum += A[i];
    }
    else  if (method == 3){
        std::cout << "Sequential" << std::endl;
        time_start = omp_get_wtime();
    
        for (intptr_t i = 0; i < n; ++i)
            sum += A[i];
    }

    double time_end = omp_get_wtime();

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Time: " << time_end - time_start << std::endl;

    delete[] A;
}

int main(){
 std::cout << "Sum:" << std::endl;
 intptr_t n = 1e7;

 task0(n,1);
 task0(n,2);
 task0(n,3);
 return 0;

}

