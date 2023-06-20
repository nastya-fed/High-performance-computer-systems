#include <iostream>
#include <omp.h>

int main(){
	const intptr_t MAX_N = 500;
	intptr_t A[MAX_N][MAX_N];
	intptr_t B[MAX_N][MAX_N];
	intptr_t C[MAX_N][MAX_N];

	intptr_t min_rand = -50;
	intptr_t max_rand = 50;
    
	srand(time(NULL));

	intptr_t n = 3;

	for (intptr_t i = 0; i < n; ++i){
	    for (intptr_t j = 0; j < n; ++j) {
	        A[i][j] = min_rand + rand() % (max_rand - min_rand + 1);
	        B[i][j] = min_rand + rand() % (max_rand - min_rand + 1);
	        C[i][j] = 0;
	    }
	}
	double time_start = omp_get_wtime();
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
	    for (int j = 0; j < n; ++j) {
	        for (int k = 0; k < n; ++k) {
	            C[i][j] += A[i][k] * B[k][j];
	        }
	     }
	}
	double time_end = omp_get_wtime();

	std::cout << "A = " << std::endl;    
	for (int i = 0; i < n; i++){
	    for (int j = 0; j < n; j++)
	        std::cout << A[i][j] << " ";
	        std::cout << std::endl;
	}
	 std::cout << "B = " << std::endl;
	 for (int i = 0; i < n; i++){
	    for (int j = 0; j < n; j++)
	        std::cout << B[i][j] << " ";
	        std::cout << std::endl;
	}
	std::cout << "C = A * B = " << std::endl;
	for (int i = 0; i < n; i++){
	    for (int j = 0; j < n; j++)
	        std::cout << C[i][j] << " ";
	        std::cout << std::endl;
	} 
	std::cout << "Time of multiplication: " << time_end - time_start << std::endl;
}
