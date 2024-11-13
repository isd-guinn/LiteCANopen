// LiteCANopenInterface.cpp
#include "LiteCANopenInterface.h"

#ifdef LITE_CO_PLATFORM_LINUX
    #include <net/if.h>
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <linux/can.h>
    #include <linux/can/raw.h>
    #include <time.h>
    #include <unistd.h>
    static int socket_fd;

#elif defined(LITE_CO_PLATFORM_ESP32)
    #include <Arduino.h>
#endif

LiteCOCANInterface::LiteCOCANInterface() {
    
}

// Platform specific implementations
bool LiteCOCANInterface::init() {
    #ifdef LITE_CO_PLATFORM_LINUX

    #elif defined(LITE_CO_PLATFORM_ESP32)

    #endif
}

bool LiteCOCANInterface::sendMessage(const LiteCOMsg* msg) {
    #ifdef LITE_CO_PLATFORM_LINUX

    #elif defined(LITE_CO_PLATFORM_ESP32)

    #endif
}

bool LiteCOCANInterface::receiveMessage(LiteCOMsg* msg) {
    #ifdef LITE_CO_PLATFORM_LINUX

    #elif defined(LITE_CO_PLATFORM_ESP32)

    #endif
}

bool LiteCOCANInterface::isMessageAvailable() {
    #ifdef LITE_CO_PLATFORM_LINUX

    #elif defined(LITE_CO_PLATFORM_ESP32)

    #endif
}

void LiteCOCANInterface::reset() {
    #ifdef LITE_CO_PLATFORM_LINUX

    #elif defined(LITE_CO_PLATFORM_ESP32)

    #endif
}

// Time functions implementation
uint32_t getTimeMs() {
    #ifdef LITE_CO_PLATFORM_LINUX

    #elif defined(LITE_CO_PLATFORM_ESP32)

    #endif
}

void delayMs(uint32_t ms) {
    #ifdef LITE_CO_PLATFORM_LINUX

    #elif defined(LITE_CO_PLATFORM_ESP32)
        delay(ms);
    #endif
}
