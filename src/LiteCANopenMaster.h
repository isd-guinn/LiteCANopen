// LiteCANopenMaster.h
#ifndef LITE_CANOPEN_MASTER_H
#define LITE_CANOPEN_MASTER_H

#include "LiteCANopen.h"
#include "LiteCANopenNode.h"
#include <vector>  // For Linux implementation

class LiteCOMaster : public LiteCONode {
public:
    // Constructor now takes nodeId for the master node itself
    LiteCOMaster(uint8_t masterNodeId);
    
    // Master-specific functions
    bool sendNMTCommand(uint8_t command, uint8_t nodeId);
    bool startNode(uint8_t nodeId);
    bool stopNode(uint8_t nodeId);
    bool setPreOperational(uint8_t nodeId);
    bool resetNode(uint8_t nodeId);
    bool resetCommunication(uint8_t nodeId);

    // Node monitoring (extended functionality)
    bool checkNodeState(uint8_t nodeId);
    bool waitForBootup(uint8_t nodeId, uint32_t timeout_ms);
    
    // Network management
    bool scanNetwork(uint8_t startNode = 1, uint8_t endNode = 127);
    
    // Getter for active nodes
    #ifdef LITE_CO_PLATFORM_LINUX
        std::vector<uint8_t> getActiveNodes() const { return activeNodes; }
    #elifdef LITE_CO_PLATFORM_ESP32
        uint8_t* getActiveNodes(uint8_t& count) { 
            count = numActiveNodes; 
            return activeNodes; 
        }
    #endif

    // Override base class process to handle master-specific messages
    void process(const LiteCOMsg* msg) override;

private:
    // Additional private members for master functionality
    #ifdef LITE_CO_PLATFORM_LINUX
        std::vector<uint8_t> activeNodes;
        std::vector<LiteCONodeState> nodeStates;
    #else LITE_CO_PLATFORM_ESP32
        uint8_t activeNodes[127];
        LiteCONodeState nodeStates[127];
        uint8_t numActiveNodes;
    #endif

    // Track heartbeats of slave nodes
    struct NodeHeartbeat {
        uint32_t lastHeartbeat;
        uint16_t heartbeatTime;
        bool active;
    };
    
    #ifdef __linux__
        std::map<uint8_t, NodeHeartbeat> heartbeats;
    #else
        NodeHeartbeat nodeHeartbeats[127];
    #endif

    void processHeartbeat(const LiteCOMsg* msg);
    void updateNodeState(uint8_t nodeId, LiteCONodeState state);
};

#endif
