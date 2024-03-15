#include "MPIDD_DeadlockDetector.h"

DeadlockDetector::DeadlockDetector(const vector<NodeProcess*>& nodes) : nodeProcesses(nodes) {}

bool DeadlockDetector::detect(int source) const {
    return isCycle(source, source);
}

bool DeadlockDetector::isCycle(int source, int currentNode) const
{
    printf("Checking for cycle: source = %d, currentNode = %d\n", source, currentNode);
    fflush(stdout);

    // Check if currentNode is MPIDD_IGNORE (not sure what value this is in your code, so I'm leaving it as is)
    if (currentNode == -1)
        return false;

    NodeProcess* node = nodeProcesses[currentNode];
    const vector<int>& edges = node->getRecvRequests();
 
    
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i] == 1 && i != currentNode) {  // If there's an outgoing edge to node i, and cuirrent edge is not the current node
            int dependency = i;
            if (source == dependency || isCycle(source, dependency)) {
                printf("\nRank %d is part of a deadlock!!!!!!!!!!!\n", dependency);
                // recordNodeAsPartOfDeadlock(currentNode);  // Placeholder for marking the node as part of a deadlock
                return true;
            }
        }
    }

    printf("No cycle found for currentNode %d with respect to source %d\n", currentNode, source);
    fflush(stdout);
    return false;
}
