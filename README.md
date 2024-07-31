# ConcurrentDeadlockDetection
Based on a research paper on concurrent deadlock detection in parallel programs by W.Haque(source is in the repo), built a proof of concept of the MPIDD model that was discussed in the paper, using C++ and MPI.
Unlike the MPI_Check and Umpire deadlock detection systems, MPIDD does not require preprocessing and works in distributed memory environment.

#How to run
- Needs a linux kernel
- I personally used ubuntu virtual box

1. Just type in terminal make -f Makefile

