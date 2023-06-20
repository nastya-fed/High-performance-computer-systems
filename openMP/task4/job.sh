#!/bin/bash
#SBATCH -p gnu
#SBATCH -c 8
export OMP_NUM_THREADS=8
./a.out
