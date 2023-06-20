#!/bin/bash
#SBATCH -p gnu
#SBATCH -c 2
export OMP_NUM_THREADS=2
./a.out
