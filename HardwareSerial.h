//
// Created by Miguel Angel Julian Aguilar on 12/11/17.
//

#ifndef PARSIVAL_HARDWARESERIAL_H
#define PARSIVAL_HARDWARESERIAL_H


#include "Stream.h"

class HardwareSerial : public Stream {
public:
    void begin(unsigned int baudrate) {}
};


#endif //PARSIVAL_HARDWARESERIAL_H
