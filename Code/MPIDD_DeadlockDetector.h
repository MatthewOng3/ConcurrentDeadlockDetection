#ifndef MPIDD_DEADLOCKDETECTOR_H
#define MPIDD_DEADLOCKDETECTOR_H

#include "util.h"
#include "MPIDD_State.h"

class DeadlockDetector {
private:
    const vector<NodeProcess*>& nodeProcesses;

public:
    DeadlockDetector(const vector<NodeProcess*>& nodes);
    bool detect(int source) const;
    bool isCycle(int source, int currentNode) const;
};

#endif 