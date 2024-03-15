#include "util.h"
#include <cstddef>  // for offsetof

// Constructor implementation
CommandInfoType::CommandInfoType() {
    int blocklengths[4] = {1, 1, 1, 1};
    MPI_Aint displacements[4];
    MPI_Datatype types[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};

    CommandInfo sample;
    
    displacements[0] = offsetof(CommandInfo, sourceRank);
    displacements[1] = offsetof(CommandInfo, destRank);
    displacements[2] = offsetof(CommandInfo, tag);
    displacements[3] = offsetof(CommandInfo, commandType);

    MPI_Type_create_struct(4, blocklengths, displacements, types, &MPI_RANK_INFO);
    MPI_Type_commit(&MPI_RANK_INFO);
}

// Singleton instance retrieval implementation
CommandInfoType& CommandInfoType::getInstance() {
    static CommandInfoType instance;  // Guaranteed to be destroyed; instantiated on first use
    return instance;
}

// Method to get the MPI datatype implementation
MPI_Datatype CommandInfoType::get() const {
    return MPI_RANK_INFO;
}

void CommandInfoType::freeType() {
    MPI_Type_free(&MPI_RANK_INFO);
}

// Destructor implementation
CommandInfoType::~CommandInfoType() {
    
}
