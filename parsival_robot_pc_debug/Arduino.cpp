//
// Created by Miguel Angel Julian Aguilar on 26/11/17.
//

#include "Arduino.h"

HardwareSerial Serial;
//HardwareSerial Serial2;

auto initTime = std::chrono::system_clock::now();

unsigned long millis() {
    std::chrono::milliseconds ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - initTime);
    return static_cast<unsigned long>(ms.count());
}