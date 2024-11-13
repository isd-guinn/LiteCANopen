// LiteCANopenNode.cpp
#include "LiteCANopenNode.h"

LiteCONode::LiteCONode(uint8_t id) : 
    nodeId(id), 
    state(LITE_CO_STATE_INIT),
    heartbeatTime(0)
{
    // Add mandatory OD entries
    uint32_t deviceType = 0;
    od.addEntry(0x1000, 0, (uint8_t*)&deviceType, 4, LITE_CO_ATTR_READ_ONLY);
    
    uint8_t errorRegister = 0;
    od.addEntry(0x1001, 0, &errorRegister, 1, LITE_CO_ATTR_READ_ONLY);
    
    uint16_t heartbeatTime = 1000;
    od.addEntry(0x1017, 0, (uint8_t*)&heartbeatTime, 2, LITE_CO_ATTR_READ_WRITE);
    
    // Add device name
    const char* deviceName = "LiteCANopen Node";
    od.addEntry(0x1008, 0, (uint8_t*)deviceName, sizeof(deviceName) / sizeof(deviceName[0]), LITE_CO_ATTR_READ_ONLY);
}

void LiteCONode::setState(LiteCONodeState newState) {
    state = newState;
}

LiteCONodeState LiteCONode::getState() {
    return state;
}

void LiteCONode::process(const LiteCOMsg* msg) {
    if (!msg) return;
    
    // Process based on COB-ID
    uint32_t cobId = msg->id;
    
    if (cobId == 0x000) {  // NMT
        handleNMT(msg);
    }
    else if ((cobId & 0x580) == 0x580) {  // SDO
        handleSDORequest(msg);
    }
    else if ((cobId & 0x180) == 0x180) {  // PDO
        receivePDO(msg);
    }
}

bool LiteCONode::handleSDORequest(const LiteCOMsg* msg) {
    if (!msg || msg->len < 4) return false;
    
    uint8_t command = msg->data[0];
    uint16_t index = (msg->data[1] << 8) | msg->data[2];
    uint8_t subIndex = msg->data[3];
    
    LiteCOODEntry* entry = od.findEntry(index, subIndex);
    if (!entry) return false;
    
    if (command == LITE_CO_SDO_UPLOAD) {
        return sendSDOResponse(index, subIndex, entry->data, entry->length);
    }
    else if (command == LITE_CO_SDO_DOWNLOAD) {
        return od.updateEntry(index, subIndex, &msg->data[4], msg->len - 4);
    }
    
    return false;
}

void LiteCONode::handleNMT(const LiteCOMsg* msg) {
    if (!msg || msg->len != 2) return;
    
    uint8_t command = msg->data[0];
    uint8_t targetNode = msg->data[1];
    
    if (targetNode == 0 || targetNode == nodeId) {
        switch (command) {
            case 0x01:  // Start
                setState(LITE_CO_STATE_OPERATIONAL);
                break;
            case 0x02:  // Stop
                setState(LITE_CO_STATE_STOPPED);
                break;
            case 0x80:  // Pre-operational
                setState(LITE_CO_STATE_PRE_OP);
                break;
            case 0x81:  // Reset node
                setState(LITE_CO_STATE_INIT);
                break;
        }
    }
}

void LiteCONode::sendHeartbeat() {
    LiteCOMsg msg;
    msg.id = 0x700 + nodeId;
    msg.data[0] = static_cast<uint8_t>(state);
    msg.len = 1;
    sendCANMessage(&msg);
}

bool LiteCONode::sendCANMessage(LiteCOMsg* msg) {
    return sendMessage(msg); 
}

bool LiteCONode::transmitPDO(uint8_t pdoNum) {
    if (pdoNum >= LITE_CO_MAX_PDO_MAPPINGS || state != LITE_CO_STATE_OPERATIONAL) {
        return false;
    }

    LiteCOMsg msg;
    msg.id = 0x180 + pdoNum * 0x100 + nodeId;
    
    // Get PDO mapping from OD
    uint16_t pdoMapIndex = 0x1A00 + pdoNum;
    LiteCOODEntry* mapEntry = od.findEntry(pdoMapIndex, 0);
    if (!mapEntry) return false;
    
    // Implement PDO mapping and data collection
    msg.len = 8; // Set according to mapping
    return sendCANMessage(&msg);
}

void LiteCONode::receivePDO(const LiteCOMsg* msg) {
    if (!msg || state != LITE_CO_STATE_OPERATIONAL) return;
    
    uint8_t pdoNum = (msg->id - 0x180) / 0x100;
    if (pdoNum >= LITE_CO_MAX_PDO_MAPPINGS) return;
    
    // Get PDO mapping from OD
    uint16_t pdoMapIndex = 0x1600 + pdoNum;
    LiteCOODEntry* mapEntry = od.findEntry(pdoMapIndex, 0);
    if (!mapEntry) return;
    
    // Process PDO data according to mapping
}

void LiteCONode::processSYNC(const LiteCOMsg* msg) {
    if (!msg || state != LITE_CO_STATE_OPERATIONAL) return;
    
    // Process synchronous PDOs
    for (uint8_t i = 0; i < LITE_CO_MAX_PDO_MAPPINGS; i++) {
        // Check if PDO is synchronous
        uint16_t commParamIndex = 0x1800 + i;
        LiteCOODEntry* commEntry = od.findEntry(commParamIndex, 2); // Transmission type
        if (commEntry && commEntry->data[0] == 1) { // Synchronous
            transmitPDO(i);
        }
    }
}

bool LiteCONode::sendSDOResponse(uint16_t index, uint8_t subIndex, uint8_t* data, uint16_t length) {
    LiteCOMsg msg;
    msg.id = 0x580 + nodeId;  // SDO response COB-ID
    msg.data[0] = LITE_CO_SDO_UPLOAD;  // Response command specifier
    msg.data[1] = (index >> 8) & 0xFF;
    msg.data[2] = index & 0xFF;
    msg.data[3] = subIndex;
    
    // Copy data (up to 4 bytes in expedited transfer)
    uint8_t dataLen = (length > 4) ? 4 : length;
    for (uint8_t i = 0; i < dataLen; i++) {
        msg.data[4 + i] = data[i];
    }
    
    msg.len = 4 + dataLen;
    return sendCANMessage(&msg);
}
