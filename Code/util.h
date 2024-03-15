#ifndef LIBRARIES_H
#define LIBRARIES_H

#include <vector>
#include <string>
#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <map>
#include <unistd.h>

//Constants
#define DETECTOR_RANK 0
#define WAIT_DURATION 10
#define MANAGER_DURATION 30

//Command types
enum MPI_CommandType {
    MPI_SEND = 69,
    MPI_RECV = 70
};


//Tags
#define WRAPPER_DETECTOR 4
#define DETECTOR_ACKNOWLEDGEMENT 9
#define STANDARD_TAG 100

//MPI datatypes
struct CommandInfo {
    int sourceRank;
    int destRank;
    int tag;
    MPI_CommandType commandType;
};

class CommandInfoType {
private:
    MPI_Datatype MPI_RANK_INFO;

    // Private constructor to prevent instantiation from outside
    CommandInfoType();

public:
    // Delete copy constructor and assignment operator to ensure only one instance
    CommandInfoType(const CommandInfoType&) = delete;
    CommandInfoType& operator=(const CommandInfoType&) = delete;

    // Public method to get the singleton instance
    static CommandInfoType& getInstance();

    // Method to get the MPI datatype
    MPI_Datatype get() const;
    void freeType();
    // Destructor to free the MPI datatype
    ~CommandInfoType();
};




#endif  