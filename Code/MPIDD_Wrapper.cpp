#include "MPIDD_Wrapper.h"

/*Wrapper function for MPI Send commands*/
int MPI_Send(const void *buffer, int dataCount, MPI_Datatype datatype, int destRank, int tag, MPI_Comm comm)
{   
    //Get the rank of the current process
    int rank, res, size;
    bool acknowledged = false;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    //Send MPI Send command to detector for processing
    if(rank != size - 1){
        printf("Rank %d in MPI _SEND FUNCTIRON\n", rank);
        fflush(stdout);
        acknowledged = sendMessageToDetector(rank, destRank, tag, MPI_SEND);
        if(acknowledged){
            res = PMPI_Send(buffer, dataCount, datatype, destRank, tag, comm);
        } 
    }
    else{
        printf("DETECTOR MPI_SEND\n");
        fflush(stdout);
        res = PMPI_Send(buffer, dataCount, datatype, destRank, tag, comm);
    }

    return res;
}

/*Wrapper function for MPI Recv commands*/
int MPI_Recv(void *buffer, int dataCount, MPI_Datatype datatype, int receivingRank, int tag, MPI_Comm comm, MPI_Status status)
{
    //Get the rank of the current process
    int rank, res, size;
    bool acknowledged = false;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    //Send MPI Send command to detector for processing
    if(rank != size - 1){
        printf("RANK %d IN MPI_Recv FUNCTION\n", rank);
        fflush(stdout);
        //Send the message to the detector for processing
        acknowledged = sendMessageToDetector(rank, receivingRank, tag, MPI_RECV);

        if(acknowledged){
            // Call the actual MPI_Recv function and res is the error code
            res = PMPI_Recv(buffer, dataCount, datatype, receivingRank, tag, comm, MPI_STATUS_IGNORE);
        }
    }
    else{
        printf("DETECTOR MPI_RECEIVE\n");
        fflush(stdout);
        res = PMPI_Recv(buffer, dataCount, datatype, receivingRank, tag, comm, MPI_STATUS_IGNORE);
    }

  
    return res;
}

/*Wrapper function for MPI_Init_Thread*/
int MPI_Init_thread(int *argc, char ***argv, int required, int *provided)
{
    PMPI_Init_thread(argc, argv, required, provided);

    
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == size - 1){
        //Initialize the Manager arguments
        MPIDD_Manager::setManagerArgs(size);
        //Run the manager
        MPIDD_Manager::getInstance().run();
    }
    sleep(5);
    // Call the actual MPI_Init_thread function using the PMPI naming convention
    return 1;
}


/*Function to send wrapper command to detector and wait for acknoledgement*/
bool sendMessageToDetector(int sourceRank, int destRank, int tag, MPI_CommandType commandType){

    
    
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //Get the MPI Datatype for sending rank info struct
    MPI_Datatype rankInfoType = CommandInfoType::getInstance().get();
    //Create the struct to send to detector
    CommandInfo rankInfo;
    rankInfo.sourceRank = sourceRank;
    rankInfo.destRank = destRank;
    rankInfo.commandType = commandType;
    rankInfo.tag = tag;

    //Send info to detector to process command
    PMPI_Send(&rankInfo, 1, rankInfoType, size - 1, WRAPPER_DETECTOR, MPI_COMM_WORLD);
    printf("Rank %d sent message to detector and waiting for acknowledgement \n", sourceRank);
    fflush(stdout);
    //Wait for acknowledgement from detector and then proceed
    bool isAcknowledged;
    PMPI_Recv(&isAcknowledged, 1, MPI_C_BOOL, size - 1, DETECTOR_ACKNOWLEDGEMENT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Rank %d received acknowledgement from Manager and proceeded with the MPI routine \n", sourceRank);
    fflush(stdout);
    return isAcknowledged;
}


