#ifndef MPIDD_WRAPPER_H
#define MPIDD_WRAPPER_H

#include "util.h"
#include "MPIDD_Manager.h"

/*Wrapper class that intercepts MPI Calls, sending the info to the detector*/
 
//MPI Wrapper functions
int MPI_Send(const void *buffer, int dataCount, MPI_Datatype datatype, int destRank, int tag, MPI_Comm comm);
int MPI_Recv(void *buffer, int dataCount, MPI_Datatype datatype, int destRank, int tag, MPI_Comm comm, MPI_Status status);
int MPI_Init_thread(int *argc, char ***argv, int required, int *provided);

//Send message to detector and wait for acknowledgement
bool sendMessageToDetector(int sourceRank, int destRank, int tag, MPI_CommandType commandType);





#endif 