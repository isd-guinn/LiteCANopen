// LiteCANopenMaster.cpp
#include "LiteCANopenMaster.h"

LiteCOMaster::LiteCOMaster(uint8_t masterNodeId) : LiteCONode(masterNodeId) {
    #ifndef LITE_CO_PLATFORM_LINUX
        numActiveNodes = 0;
        // Initialize arrays
        memset(activeNodes, 0, sizeof(activeNodes));
        memset(nodeStates, 0, sizeof(nodeStates));
        memset(nodeHeartbeats, 0, sizeof(nodeHeartbeats));
    #endif
}


void LiteCOMaster::process(const LiteCOMsg* msg) {
    if (!msg) return;

    // First, process as a normal node
    LiteCONode::process(msg);

    // Then handle master-specific processing
    uint32_t cobId = msg->id;

    // Process heartbeat messages from other nodes
    if ((cobId & 0x700) == 0x700) {
        processHeartbeat(msg);
    }
}

void LiteCOMaster::processHeartbeat(const LiteCOMsg* msg) {
    uint8_t nodeId = msg->id & 0x7F;
    if (nodeId == getNodeId()) return; // Skip own heartbeat

    LiteCONodeState state = static_cast<LiteCONodeState>(msg->data[0]);
    updateNodeState(nodeId, state);

    // Update heartbeat timing
    #ifdef LITE_CO_PLATFORM_LINUX
        heartbeats[nodeId].lastHeartbeat = millis();
        heartbeats[nodeId].active = true;

    #elifdef LITE_CO_PLATFORM_ESP32
        if (nodeId < 127) {
            nodeHeartbeats[nodeId].lastHeartbeat = millis();
            nodeHeartbeats[nodeId].active = true;
        }
    #endif
}

bool LiteCOMaster::sendNMTCommand(uint8_t command, uint8_t nodeId) {
    LiteCOMsg msg;
    msg.id = 0x000;
    msg.data[0] = command;
    msg.data[1] = nodeId;
    msg.len = 2;
    
    return sendCANMessage(&msg);
}

// Implement other functions...
bool LiteCOMaster::startNode(uint8_t nodeId) {
    return sendNMTCommand(LITE_CO_NMT_START, nodeId);
}

bool LiteCOMaster::stopNode(uint8_t nodeId) {
    return sendNMTCommand(LITE_CO_NMT_STOP, nodeId);
}

bool LiteCOMaster::setPreOperational(uint8_t nodeId) {
    return sendNMTCommand(LITE_CO_NMT_ENTER_PREOP, nodeId);
}

bool LiteCOMaster::resetNode(uint8_t nodeId) {
    return sendNMTCommand(LITE_CO_NMT_RESET_NODE, nodeId);
}

bool LiteCOMaster::resetCommunication(uint8_t nodeId) {
    return sendNMTCommand(LITE_CO_NMT_RESET_COMM, nodeId);
}

bool LiteCOMaster::scanNetwork(uint8_t startNode, uint8_t endNode) {
    // Clear previous scan results

    #ifdef LITE_CO_PLATFORM_LINUX
        activeNodes.clear();
    #elifdef LITE_CO_PLATFORM_ESP32
        numActiveNodes = 0;
    #endif

    return false;
}

void LiteCOMaster::updateNodeState(uint8_t nodeId, LiteCONodeState state) {
    if (nodeId >= 127) return;
    
    #ifdef LITE_CO_PLATFORM_LINUX
        nodeStates[nodeId] = state;
    #else LITE_CO_PLATFORM_ESP32
        nodeStates[nodeId] = state;
    #endif
}

bool LiteCOMaster::checkNodeState(uint8_t nodeId) {
    if (nodeId >= 127) return false;
    
    #ifdef LITE_CO_PLATFORM_LINUX
        auto it = heartbeats.find(nodeId);
        if (it != heartbeats.end()) {
            return (getTimeMs() - it->second.lastHeartbeat) < it->second.heartbeatTime;
        }
    #else
        if (nodeHeartbeats[nodeId].active) {
            return (getTimeMs() - nodeHeartbeats[nodeId].lastHeartbeat) < 
                   nodeHeartbeats[nodeId].heartbeatTime;
        }
    #endif
    return false;
}

bool LiteCOMaster::waitForBootup(uint8_t nodeId, uint32_t timeout_ms) {
    uint32_t startTime = getTimeMs();
    
    while (getTimeMs() - startTime < timeout_ms) {
        LiteCOMsg msg;
        if (receiveMessage(&msg)) {
            if (msg.id == (0x700 + nodeId) && msg.data[0] == 0) {
                return true;
            }
        }
        delayMs(1);
    }
    return false;
}