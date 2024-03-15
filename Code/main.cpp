#include "MPIDD.h"

using namespace std;

void testNoDeadlock(int rank){
    if (rank == 0) {
        sleep(2);
        int message = 123;
        MPI_Status testStatus;
        MPI_Send(&message, 1, MPI_INT, 1, STANDARD_TAG, MPI_COMM_WORLD); // Send to Process 2
        MPI_Recv(&message, 1, MPI_INT, 1, STANDARD_TAG, MPI_COMM_WORLD, testStatus); // Wait for message from Process 2
        
    }
    else if (rank == 1) {
        
        int message = 456;
        MPI_Status testStatus;
        MPI_Recv(&message, 1, MPI_INT, 0, STANDARD_TAG, MPI_COMM_WORLD, testStatus); // Wait for message from Process 1
        MPI_Send(&message, 1, MPI_INT, 0, STANDARD_TAG, MPI_COMM_WORLD); // Send to Process 1
    }
}

void testDeadlock(int rank){
    if (rank == 0) {
        int message;
        MPI_Status testStatus;
        MPI_Recv(&message, 1, MPI_INT, 1, STANDARD_TAG, MPI_COMM_WORLD, testStatus); // Wait for message from Process 2
    }
    else if (rank == 1) {
        sleep(2);
        int message;
        MPI_Status testStatus;
        MPI_Recv(&message, 1, MPI_INT, 0, STANDARD_TAG, MPI_COMM_WORLD, testStatus); // Wait for message from Process 1
    }
}

int main(int argc, char **argv) {
    
    int rank, size;

    //Init multi threaded 
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    
    //Returns size of group of machines/tasks that the current task is involved with 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 

    testNoDeadlock(rank);
     
    MPI_Finalize();
    return 0;
}