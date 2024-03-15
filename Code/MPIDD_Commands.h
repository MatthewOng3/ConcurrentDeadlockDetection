#ifndef MPIDD_COMMANDS_H
#define MPIDD_COMMANDS_H

#include "util.h"
#include "MPIDD_State.h"
#include "MPIDD_Manager.h"

/*Abstract Class for different interpretations of each command*/
class CommandInterpreter {
public:
    virtual ~CommandInterpreter() = default; //Default implementation of destructor
    virtual void interpretCommand(CommandInfo commandInfo, MPIDD_State* state) = 0; //Abstract method,  = 0 means pure virtual , no implementation and must be overriden
};

// Concrete interpreter for MPI_Send command
class SendCommandInterpreter : public CommandInterpreter {
public:
    void interpretCommand(CommandInfo  commandInfo, MPIDD_State* state) override;
};

// Concrete interpreter for MPI_Recv Command
class RecvCommandInterpreter : public CommandInterpreter {
public:
    void interpretCommand(CommandInfo  commandInfo, MPIDD_State* state) override;
};


 

#endif  