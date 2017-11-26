//
// Created by Miguel Angel Julian Aguilar on 26/11/17.
//

#ifndef PARSIVAL_SINUSOIDCONTROLLER_H
#define PARSIVAL_SINUSOIDCONTROLLER_H


#include "JointInterface.h"
#include <Arduino.h>

class SinusoidMovementJoint : public JointInterface {
public:
    void refresh() {
        eleapsedTime_ = millis() - t0_;
        if (isRunning()) {
            actualPosition_ =  static_cast<int>(
                    initialPosition_ + (1 + sin(PI * eleapsedTime_/timeToDestiny_ - PI/2)) *
                                       (objectivePosition_ - initialPosition_)/2);
        } else {
            actualPosition_ = objectivePosition_;
        }
    }

    int getActualPosition() const {
        return actualPosition_;
    }

    void setActualPosition(int actualPosition) {
        actualPosition_ = actualPosition;
    }

    void setObjectivePosition(int objectivePosition, float timeToDestiny) {
        t0_ = millis();
        objectivePosition_ = objectivePosition;
        initialPosition_ = actualPosition_;
        timeToDestiny_ = timeToDestiny;
        eleapsedTime_ = 0;
    }

    bool isRunning() const {
        if (0 < timeToDestiny_ && eleapsedTime_ <= timeToDestiny_) {
            return true;
        } else {
            return false;
        }
    }

protected:
private:
    unsigned long t0_;
    float timeToDestiny_;
    int initialPosition_;
    int objectivePosition_;
    int actualPosition_;
    unsigned long eleapsedTime_;

};


#endif //PARSIVAL_SINUSOIDCONTROLLER_H
