// LiteCOOD.h (Object Dictionary header)
#ifndef LITE_CO_OD_H
#define LITE_CO_OD_H

#include "LiteCANopen.h"
#include "LiteCANopenTypes.h"

struct LiteCOODEntry {
    uint16_t index;
    uint8_t subIndex;
    uint8_t* data;
    uint16_t length;
    uint8_t attr;
};

class LiteCOOD {
public:
    LiteCOOD();
    
    bool addEntry(uint16_t index, uint8_t subIndex, uint8_t* data, uint16_t length, uint8_t attr);
    LiteCOODEntry* findEntry(uint16_t index, uint8_t subIndex);
    bool updateEntry(uint16_t index, uint8_t subIndex, const uint8_t* data, uint16_t length);

private:
    LiteCOODEntry entries[LITE_CO_MAX_OD_ENTRIES];
    uint8_t numEntries;
};

#endif
