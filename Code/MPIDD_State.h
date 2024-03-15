#ifndef MPIDD_STATE_H
#define MPIDD_STATE_H


#include "util.h"
using namespace std;


/*Basically representing the connections between nodes, so an edge class*/
class Command {
    private:
        int from;
        int to;
        int tag;
        MPI_CommandType commandType;
    public:
        //Constructor
        Command(int from, int to, int tag, MPI_CommandType commandType); 
        //Getters
        int getFrom();
        int getTo();
        int getCommandType();
};


/*Representing each node process info, so like a node class*/
class NodeProcess {
    private:
        int processRank;
        vector<int> recvRequests;
        vector<int> sendRequests;
    public:
        NodeProcess(int processRank, int totalProcess);

        int getRank();

        //Receive requests
        void addRecvRequest(int destRank);
        void removeRecvRequest(int destRank);
        bool hasRecvRequest(int destRank);

        //Send requests
        void addSendRequest(int destRank);
        void removeSendRequest(int destRank);
        bool hasSendRequest(int destRank);
        
        

        // Function to retrieve the outgoingEdges vector
        const vector<int>& getRecvRequests() const;
        const vector<int>& getSendRequests() const;
};

/*State class of the detector*/
class MPIDD_State {
private:
    vector<NodeProcess*> nodeProcesses;
    bool deadlockStatus;
public:
    // Constructor
    MPIDD_State(int totalNodes);

    // Methods to manage and query the state
    void addCommand(int nodeRank, Command* newCommand);
    void removeCommand(int fromRank, Command *delCommand);
    bool hasMatchingSend(int fromRank, Command *checkingCommand);
    bool hasMatchingRecv(int fromRank, Command *checkingCommand);
    
    bool isResolved(int fromRank, int toRank, MPI_CommandType commandType);

    void setDeadloockStatus(bool status);
    void printState() const;
    const vector<NodeProcess*>& getNodeProcesses() const;
    
};

#endif 