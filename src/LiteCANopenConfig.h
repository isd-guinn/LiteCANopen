// LiteCANopenConfig.h
#ifndef LITE_CANOPEN_CONFIG_H
#define LITE_CANOPEN_CONFIG_H

#include "LiteCANopen.h"

// #define LITE_CO_PLATFORM_LINUX     
#define LITE_CO_PLATFORM_ESP32     


#ifdef LITE_CO_PLATFORM_LINUX
    #define LITE_CO_USE_DYNAMIC_MEMORY 1
    #define LITE_CO_MAX_STATIC_OD_ENTRIES 64

#elifdef LITE_CO_PLATFORM_ESP32
    #define LITE_CO_USE_DYNAMIC_MEMORY 0
    #define LITE_CO_MAX_STATIC_OD_ENTRIES 64

#endif

// Buffer configuration
#define LITE_CO_RX_BUFFER_SIZE 32 

#if LITE_CO_USE_DYNAMIC_MEMORY
    #include <stdlib.h>
    #define LITE_CO_MEMORY_ALLOC(size) ((LiteCOODEntry*)malloc(size))
    #define LITE_CO_MEMORY_FREE(ptr) free(ptr)
#else
    #define LITE_CO_MEMORY_ALLOC(size) nullptr
    #define LITE_CO_MEMORY_FREE(ptr)
#endif

#endif
