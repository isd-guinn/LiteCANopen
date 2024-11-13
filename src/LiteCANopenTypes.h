// LiteCANopenTypes.h (previously LiteCANTypes.h)
#ifndef LITE_CANOPEN_TYPES_H
#define LITE_CANOPEN_TYPES_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

#define LITE_CO_MAX_OD_ENTRIES 64
#define LITE_CO_MAX_PDO_MAPPINGS 8
#define LITE_CO_DATA_LENGTH 8

#define LITE_CO_ERR_NONE          0x00
#define LITE_CO_ERR_OD_ACCESS     0x01
#define LITE_CO_ERR_PDO_LENGTH    0x02
#define LITE_CO_ERR_SDO_TIMEOUT   0x03

#define LITE_CO_ATTR_READ_ONLY    0x01
#define LITE_CO_ATTR_READ_WRITE   0x00
#define LITE_CO_ATTR_MAPPABLE     0x02

#define LITE_CO_NMT_START          0x01
#define LITE_CO_NMT_STOP           0x02
#define LITE_CO_NMT_ENTER_PREOP    0x80
#define LITE_CO_NMT_RESET_NODE     0x81
#define LITE_CO_NMT_RESET_COMM     0x82

#define LITE_CO_ERROR_REGISTER_GENERIC       0x01
#define LITE_CO_ERROR_REGISTER_CURRENT       0x02
#define LITE_CO_ERROR_REGISTER_VOLTAGE       0x04
#define LITE_CO_ERROR_REGISTER_TEMPERATURE   0x08
#define LITE_CO_ERROR_REGISTER_COMMUNICATION 0x10
#define LITE_CO_ERROR_REGISTER_PROFILE       0x20

// NMT command structure
struct LiteCONMTCommand {
    uint8_t command;
    uint8_t nodeId;  // 0 for all nodes
};


struct LiteCOMsg {    // CANopen message structure
    uint32_t id;
    uint8_t data[LITE_CO_DATA_LENGTH];
    uint8_t len;
};

enum LiteCONodeState {
    LITE_CO_STATE_INIT,
    LITE_CO_STATE_PRE_OP,
    LITE_CO_STATE_OPERATIONAL,
    LITE_CO_STATE_STOPPED
};

enum LiteCOSDOCommand {
    LITE_CO_SDO_UPLOAD = 0x40,
    LITE_CO_SDO_DOWNLOAD = 0x20,
    LITE_CO_SDO_ABORT = 0x80
};

#endif
