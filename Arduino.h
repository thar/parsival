//
// Created by Miguel Angel Julian Aguilar on 12/11/17.
//

#ifndef PARSIVAL_ARDUINO_H
#define PARSIVAL_ARDUINO_H

#include "HardwareSerial.h"
#include <chrono>

HardwareSerial Serial;

auto initTime = std::chrono::system_clock::now();

unsigned long millis() {
    std::chrono::milliseconds ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - initTime);
    return static_cast<unsigned long>(ms.count());
}

class Arduino {

};


#endif //PARSIVAL_ARDUINO_H
