#include "MPIDD_Manager.h"

//Number of total processors
int MPIDD_Manager::nodeProcessors = 0;


MPIDD_Manager::MPIDD_Manager()
{
    this->state = new MPIDD_State(nodeProcessors);
};

/*Getter for the singleton instance*/
MPIDD_Manager& MPIDD_Manager::getInstance() {
    static MPIDD_Manager instance;   
    return instance;
}

/**/
void MPIDD_Manager::run(){
 
    //Set number of threads for parallel region, 1 thread for main loop and rest for spawning childs
    omp_set_num_threads(nodeProcessors);

    //Get the Datatype MPI
    MPI_Datatype cmdInfoType = CommandInfoType::getInstance().get();


    double managerLoopStart = MPI_Wtime();
   
    bool hasBeenAcknowledged;
    MPI_Request detectorStatus;
    
    #pragma omp parallel
    {
        //Thread that keeps looping
        #pragma omp single
        {
            //Runs for the manager duration time
            while(MPI_Wtime() - managerLoopStart < MANAGER_DURATION){
                int commandAvailable;
                MPI_Status probeStatus;

                MPI_Iprobe(MPI_ANY_SOURCE, WRAPPER_DETECTOR, MPI_COMM_WORLD, &commandAvailable, &probeStatus);
                hasBeenAcknowledged = false;
            
                //If a request has been found
                if(commandAvailable){
                    CommandInfo receivedInfo;
                    //Receive command inputs from MPI_Wrappers
                    MPI_Irecv(&receivedInfo, 1, cmdInfoType, MPI_ANY_SOURCE, WRAPPER_DETECTOR, MPI_COMM_WORLD, &detectorStatus);
                    
                   

                    //Process the incoming MPI Command
                    processCommand(receivedInfo);

                    //Send back acknowledgement to node
                    hasBeenAcknowledged = true;
                    MPI_Request acknowledgeReq;
                    MPI_Isend(&hasBeenAcknowledged, 1, MPI_C_BOOL, receivedInfo.sourceRank, DETECTOR_ACKNOWLEDGEMENT, MPI_COMM_WORLD, &acknowledgeReq);

                    
              
                    if(hasBeenAcknowledged){
                        #pragma omp task
                        {
                 
                            monitorCommand(receivedInfo);
                        }
                    }
        
                }
                
            }
        }

    }
 
    CommandInfoType::getInstance().freeType();
 
}

/*Process the received command from wrapper, calling the necessary interepreter*/
void MPIDD_Manager::processCommand(CommandInfo commandInfo)
{
    CommandInterpreter* interpreter = nullptr;

    // Interpret the send command
    if (commandInfo.commandType == MPI_SEND){
        interpreter = new SendCommandInterpreter();
    }
    //Interpret the receive command
    else if(commandInfo.commandType == MPI_RECV){
        interpreter = new RecvCommandInterpreter();
    }

    if (interpreter) {
        
        
        //Call the respective interpretCommand function
        #pragma omp critical
        {
            interpreter->interpretCommand(commandInfo, this->state);
        }
        delete interpreter;  //Free allocated memory

        //Use a thread to spawn a child 
        state->printState();
    }
}

/*Manager function to call the deadlock detector and pass in the state at the time it is called*/
bool MPIDD_Manager::detectDeadlock(int source) const
{
    DeadlockDetector detector(state->getNodeProcesses());
    return detector.detect(source);
}

void MPIDD_Manager::monitorCommand(CommandInfo cmdInfo) {

    double startTime = MPI_Wtime();
    double elapsedTime = 0; 
    
    bool testFlag = false;
    //While loop runs for as long as the command represented is 1
    while(this->state->isResolved(cmdInfo.sourceRank, cmdInfo.destRank, cmdInfo.commandType)){
         
        //Check elapsed time
        elapsedTime = MPI_Wtime() - startTime;
        //If exceeds 10 seconds call and it is still 1, run detection algorithm
        if (elapsedTime > 5.0) {
            testFlag = true;
            break;
        }
    }

    if(testFlag){
        this->detectDeadlock(cmdInfo.sourceRank);
    }
    
}


/*Sets the variables needed when creating instance*/
void MPIDD_Manager::setManagerArgs(int numNodes) {
    nodeProcessors = numNodes;
};

MPIDD_Manager::~MPIDD_Manager() {
    delete state;
};