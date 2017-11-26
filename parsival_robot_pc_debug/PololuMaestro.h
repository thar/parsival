//
// Created by Miguel Angel Julian Aguilar on 26/11/17.
//

#ifndef PARSIVAL_POLOLUMAESTRO_H
#define PARSIVAL_POLOLUMAESTRO_H


#include "Stream.h"

class MiniMaestro {
public:
    MiniMaestro(Stream& stream) : stream_(stream) {}
    void setAcceleration(int idx, int acceleration) {}
    void setSpeed(int idx, int speed) {}
    void setTarget(int idx, int position) {
        stream_.print(idx);
        stream_.print("->");
        stream_.println(position);
    }

private:
    Stream& stream_;
};


#endif //PARSIVAL_POLOLUMAESTRO_H
