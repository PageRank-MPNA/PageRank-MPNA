#!/bin/bash

OMP_NUM_THREADS=1 srun mpirun -np 1 ./bin/bench >> output.txt

OMP_NUM_THREADS=1 srun mpirun -np 2 ./bin/bench >> output.txt

OMP_NUM_THREADS=1 srun mpirun -np 4 ./bin/bench >> output.txt

OMP_NUM_THREADS=1 srun mpirun -np 8 ./bin/bench >> output.txt

OMP_NUM_THREADS=1 srun mpirun -np 16 ./bin/bench >> output.txt

OMP_NUM_THREADS=1 srun mpirun -np 32 ./bin/bench >> output.txt

OMP_NUM_THREADS=1 srun mpirun -np 64 ./bin/bench >> output.txt

OMP_NUM_THREADS=2 srun mpirun -np 1 ./bin/bench >> output.txt

OMP_NUM_THREADS=4 srun mpirun -np 1 ./bin/bench >> output.txt

OMP_NUM_THREADS=8 srun mpirun -np 1 ./bin/bench >> output.txt

OMP_NUM_THREADS=16 srun mpirun -np 1 ./bin/bench >> output.txt

OMP_NUM_THREADS=32 srun mpirun -np 1 ./bin/bench >> output.txt

OMP_NUM_THREADS=64 srun mpirun -np 1 ./bin/bench >> output.txt

OMP_NUM_THREADS=64 srun mpirun -np 64 ./bin/bench >> output.txt