#ifndef StaticTimePose_h
#define StaticTimePose_h

#include "Arduino.h"
#include "Pose.h"

class StaticTimePose : public Pose {
public:
    StaticTimePose(unsigned long waitTime) : waitTime(waitTime), timeFlagObjective(0) {}
    
    void update() {
        
    }
    
    Pose * getNextPose() {
        if (isTimeFlagOn()) {
            return getInitilizedNextPose();
        }
        else
            return this;
    }
    
    void init() {
        timeFlagObjective = millis() + waitTime;
#ifdef DEBUG
        Serial.print("init wait for ");
        Serial.println(timeFlagObjective);
#endif
    }
    
protected:
    bool isTimeFlagOn() {
        if (millis() >= timeFlagObjective) {
            return true;
        }
        return false;
    }
    
private:
    unsigned long waitTime;
    unsigned long timeFlagObjective;
    
};

#endif /* StaticTimePose_h */
