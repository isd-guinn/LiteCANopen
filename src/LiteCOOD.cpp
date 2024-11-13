// LiteCOOD.cpp
#include "LiteCOOD.h"

LiteCOOD::LiteCOOD() : numEntries(0) {
    // Initialize entries array
    for (uint8_t i = 0; i < LITE_CO_MAX_OD_ENTRIES; i++) {
        entries[i].index = 0;
        entries[i].subIndex = 0;
        entries[i].data = nullptr;
        entries[i].length = 0;
        entries[i].attr = 0;
    }
}

bool LiteCOOD::addEntry(uint16_t index, uint8_t subIndex, uint8_t* data, uint16_t length, uint8_t attr) {
    if (numEntries >= LITE_CO_MAX_OD_ENTRIES) return false;
    
    // Check for existing entry
    for (uint8_t i = 0; i < numEntries; i++) {
        if (entries[i].index == index && entries[i].subIndex == subIndex) {
            // Update existing entry
            entries[i].data = data;
            entries[i].length = length;
            entries[i].attr = attr;
            return true;
        }
    }
    
    // Add new entry
    entries[numEntries].index = index;
    entries[numEntries].subIndex = subIndex;
    entries[numEntries].data = data;
    entries[numEntries].length = length;
    entries[numEntries].attr = attr;
    numEntries++;
    
    return true;
}

LiteCOODEntry* LiteCOOD::findEntry(uint16_t index, uint8_t subIndex) {
    for (uint8_t i = 0; i < numEntries; i++) {
        if (entries[i].index == index && entries[i].subIndex == subIndex) {
            return &entries[i];
        }
    }
    return nullptr;
}

bool LiteCOOD::updateEntry(uint16_t index, uint8_t subIndex, const uint8_t* data, uint16_t length) {
    LiteCOODEntry* entry = findEntry(index, subIndex);
    if (!entry || (entry->attr & LITE_CO_ATTR_READ_ONLY)) return false;
    
    if (length > entry->length) return false;
    
    for (uint16_t i = 0; i < length; i++) {
        entry->data[i] = data[i];
    }
    
    return true;
}
