// LiteCANopenNode.h
#ifndef LITE_CANOPEN_NODE_H
#define LITE_CANOPEN_NODE_H

#include "LiteCANopen.h"
#include "LiteCANopenTypes.h"
#include "LiteCOOD.h"

class LiteCONode : public LiteCOCANInterface{
public:
    LiteCONode(uint8_t nodeId);
    
    void setState(LiteCONodeState state);
    LiteCONodeState getState();
    virtual void process(const LiteCOMsg* msg);
    
    bool handleSDORequest(const LiteCOMsg* msg);
    bool sendSDOResponse(uint16_t index, uint8_t subIndex, uint8_t* data, uint16_t length);
    
    bool transmitPDO(uint8_t pdoNum);
    void receivePDO(const LiteCOMsg* msg);
    
    void handleNMT(const LiteCOMsg* msg);
    void sendHeartbeat();
    
    LiteCOOD* getOD() { return &od; }

    virtual ~LiteCONode() = default;

    virtual bool init();
    virtual bool sendMessage(const LiteCOMsg* msg);
    virtual LiteCOMsg receiveMessage(LiteCOMsg* buffer, uint8_t buffer_size);
    virtual bool isMessageAvailable();
    virtual uint32_t getTimeMs();
    virtual void delayMs(uint32_t ms);

protected:
    bool sendCANMessage(LiteCOMsg* msg);
    uint8_t getNodeId() const { return nodeId; }


private:
    uint8_t nodeId;
    LiteCONodeState state;
    LiteCOOD od;
    uint32_t heartbeatTime;
    
    bool sendCANMessage(LiteCOMsg* msg);
    void processSYNC(const LiteCOMsg* msg);
};

#endif
