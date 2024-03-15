#include "MPIDD_Commands.h"


/*MPI Send command interpreter*/
void SendCommandInterpreter::interpretCommand(CommandInfo commandInfo, MPIDD_State* state) {
     
    // Interpret the MPI_SEND command to create a new command object
    Command* cmd = new Command(commandInfo.sourceRank, commandInfo.destRank, commandInfo.tag, commandInfo.commandType);

    //Check if there is a waiting MPI Recv call and if so match that with the MPI Send call and remove the command
    bool hasRecvReq = state->hasMatchingRecv(commandInfo.sourceRank, cmd);
 
    
    //If the command already exists, meaning a receive is waiting for the current send, we can remove that command from the graph
    if(hasRecvReq){
        //Remove command from dependency graph
        state->removeCommand(commandInfo.sourceRank, cmd);
    }
    //Meaning there is no receive  
    else{
        // Update state graph by adding an edge, so sourceRank will draw an edge to destRank, signaling source is waiting on destRank
        state->addCommand(commandInfo.sourceRank, cmd);
    }
}

/*MPI Recv command interpreter, we add an edge in the dependency graph*/
void RecvCommandInterpreter::interpretCommand(CommandInfo commandInfo, MPIDD_State* state) {

    // Interpret the MPI_RECV command to create a new command object
    Command* cmd = new Command(commandInfo.sourceRank, commandInfo.destRank, commandInfo.tag, commandInfo.commandType);

    bool hasSendReq = state->hasMatchingSend(commandInfo.sourceRank, cmd);

   
    //If an existing send request exists, remove it
    if(hasSendReq){
        state->removeCommand(commandInfo.sourceRank, cmd);
    }
    else{
        // Update state graph by adding an edge, so sourceRank will draw an edge to destRank, signaling source is waiting on destRank
        state->addCommand(commandInfo.sourceRank, cmd);
    }
    
   
}

 