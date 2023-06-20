#include <iostream>
#include <omp.h>
#include <iomanip>

int main(){
    std::cout << "PI" << std::endl;
    std::cout << "N  treads  time  pi" << std::endl;
    const intptr_t N = 12;
    const intptr_t threads[] = {1, 1, 2, 2, 4, 4, 8, 8, 10, 10, 12, 12};
    intptr_t n_0 = 1e2;
    intptr_t n_1 = 1e6;

    for (intptr_t k = 0; k < N; ++k) {
        double pi = .0;
        intptr_t  n = (k % 2 == 0) ? n_0 : n_1;
        intptr_t treads = threads[k];

        double time_start = omp_get_wtime();

#pragma omp parallel for num_threads(treads), reduction(+:pi)
        for (intptr_t i = 0; i < n; ++i) {
           pi += 4 / (1 + (i + 0.5) * (i + 0.5) * 1/(n*n));
        }

        pi /= n;

        double time_end = omp_get_wtime();

	std::cout << std::setprecision(8) << std::fixed;
        std::cout << n <<  " ";
        std::cout << treads << " ";
        std::cout << (time_end - time_start) << " ";
        std::cout <<  pi << std::endl;
        
    }   


    return 0;
}
