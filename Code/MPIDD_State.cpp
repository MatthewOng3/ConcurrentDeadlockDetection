#include "MPIDD_State.h"

/*------------State Class-------------*/

//Total nodes is the total number of processes that is not the base station, base station rank is 0
MPIDD_State::MPIDD_State(int totalNodes)
{
    nodeProcesses.resize(totalNodes - 1); 
    for (int i = 0; i < totalNodes - 1; i++) {
        nodeProcesses[i] = new NodeProcess(i, totalNodes - 1);
    }
}

/*Add an edge from a node to another */
void MPIDD_State::addCommand(int nodeRank, Command *newCommand)
{   
    //Depending on the type add to their respective array
    if(newCommand->getCommandType() == MPI_SEND){
        //Add the node's rank as key and new command edge, passing in the to rank to add 
        nodeProcesses[nodeRank]->addSendRequest(newCommand->getTo());
    }
    else if(newCommand->getCommandType() == MPI_RECV){
        nodeProcesses[nodeRank]->addRecvRequest(newCommand->getTo());
    }
    
}

/*Only can remove commands when a match occurs*/
void MPIDD_State::removeCommand(int fromRank, Command *delCommand)
{
    
    if(delCommand->getCommandType() == MPI_SEND){
        printf("\nMATCH FOUND from MPI_SEND! Removing %d to %d\n\n", fromRank, delCommand->getTo());
        fflush(stdout);
        nodeProcesses[fromRank]->removeSendRequest(delCommand->getTo());
        nodeProcesses[delCommand->getTo()]->removeRecvRequest(fromRank);
    }
    else if(delCommand->getCommandType() == MPI_RECV){
        printf("\nMATCH FOUND from MPI_RECV! Removing %d to %d\n\n", fromRank, delCommand->getTo());
        fflush(stdout);
        nodeProcesses[fromRank]->removeRecvRequest(delCommand->getTo());
        nodeProcesses[delCommand->getTo()]->removeSendRequest(fromRank);
    }
    //Delete the command object
    delete delCommand;
}

/*Check if cmd node process has an outgoing edge to cmd's to rank */
bool MPIDD_State::hasMatchingSend(int fromRank, Command *checkingCommand)
{
    return this->nodeProcesses[checkingCommand->getTo()]->hasSendRequest(fromRank);
}


bool MPIDD_State::hasMatchingRecv(int fromRank, Command *checkingCommand)
{
    return this->nodeProcesses[checkingCommand->getTo()]->hasRecvRequest(fromRank);
}

bool MPIDD_State::isResolved(int fromRank, int toRank, MPI_CommandType commandType){
    if(commandType == MPI_SEND){
        return this->nodeProcesses[fromRank]->hasSendRequest(toRank);
    }
    else{
        return this->nodeProcesses[fromRank]->hasRecvRequest(toRank);
    }
}

void MPIDD_State::setDeadloockStatus(bool status)
{
    this->deadlockStatus = status;
}


void MPIDD_State::printState() const
{
    std::cout << "\nState Information:" << std::endl;
    for (int i = 0; i < nodeProcesses.size(); i++) {
        
        std::cout << "Node Rank: " << nodeProcesses[i]->getRank() << std::endl;
        std::cout << "Receiving Requests: " << std::endl;
        
        const vector<int>& recvEdges = nodeProcesses[i]->getRecvRequests();
        for (int j = 0; j < recvEdges.size(); j++) {
            std::cout << "Rank: " << j << " ";  // j is the rank (index) here
            std::cout << "Edge value: " << recvEdges[j] << std::endl;
        }

        std::cout << std::endl;
        std::cout << "Sending Requests: " << std::endl;
        
        const vector<int>& sendEdges = nodeProcesses[i]->getSendRequests();
        for (int j = 0; j < sendEdges.size(); j++) {
            std::cout << "Rank: " << j << " ";  // j is the rank (index) here
            std::cout << "Edge value: " << sendEdges[j] << std::endl;
        }
        std::cout << std::endl;
        
    }
    std::cout << "--------------------------" << std::endl;
}

const vector<NodeProcess*>& MPIDD_State::getNodeProcesses() const {
    return nodeProcesses;
}

/*----------Node Process Class------------*/
NodeProcess::NodeProcess(int processRank, int nodeProcesses){
    this->processRank = processRank;
    //Resize the vector, -1 for base station, 
    recvRequests.resize(nodeProcesses, 0);
    sendRequests.resize(nodeProcesses, 0);
}

/*Functions for receive request vector*/
void NodeProcess::addRecvRequest(int toRank) {
    recvRequests[toRank] = 1;
}

void NodeProcess::removeRecvRequest(int toRank) {
    recvRequests[toRank] = 0;
}

bool NodeProcess::hasRecvRequest(int toRank) {
    if(recvRequests[toRank] == 1){
        return true;
    }
    return false;
}
 
/*Functions for send requests array*/
void NodeProcess::addSendRequest(int toRank) {
    sendRequests[toRank] = 1;
}

void NodeProcess::removeSendRequest(int toRank) {
    sendRequests[toRank] = 0;
}

bool NodeProcess::hasSendRequest(int toRank) {
    if(sendRequests[toRank] == 1){
        return true;
    }
    return false;
}

int NodeProcess::getRank() {
    return this->processRank;
}

const vector<int>& NodeProcess::getRecvRequests() const {
    return recvRequests;
}

const vector<int>& NodeProcess::getSendRequests() const {
    return sendRequests;
}

/*-----------Command Class----------------*/
Command::Command(int from, int to, int tag, MPI_CommandType commandType)
{
    this->from = from;
    this->to = to;
    this->tag = tag;
    this->commandType = commandType;
}

int Command::getFrom()
{
    return this->from;
}

int Command::getCommandType()
{
    return this->commandType;
}

int Command::getTo()
{
    return this->to;
}