#ifndef MPIDD_MANAGER_H
#define MPIDD_MANAGER_H

#include "util.h"
#include "MPIDD_Commands.h"
#include "MPIDD_State.h"
#include "MPIDD_DeadlockDetector.h"

class MPIDD_Manager {
    private:
        MPIDD_State* state;
        // Private constructor to ensure only one instance
        MPIDD_Manager();
        //Desctructor
        ~MPIDD_Manager();
        //Variables
        static int nodeProcessors; //Number of rank processes in the program excluding the detector
        
        // Delete copy constructor and assignment operator to prevent copying
        MPIDD_Manager(const MPIDD_Manager&) = delete;
        void operator=(const MPIDD_Manager&) = delete;

    public:
        // Singleton instance retrieval
        static MPIDD_Manager& getInstance();
        //Set the arguments for initialization
        static void setManagerArgs(int numNodes);
        void run();
        void monitorCommand(CommandInfo cmdInfo);
        void processCommand(CommandInfo commandInfo);
        bool detectDeadlock(int source) const;
};


#endif 