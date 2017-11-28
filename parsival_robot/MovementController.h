#ifndef MovementController_h
#define MovementController_h

#include "Arduino.h"
#include "Movement.h"
#include "Pose.h"

class MovementController {
public:
    MovementController() : actualPose_(nullptr) {}
    
    void addMovement(Movement * movement) {
        if (!actualPose_) {
#ifdef PC_VERSION
            Serial.println("New movement");
#endif
            actualPose_ = movement->getInitialPose();
            actualPose_->init();
            //Serial.print("Pose:");
            //Serial.println(actualPose_);
        }
    }
    
    void doStep() {
        //Serial.println("MovementController.doStep()");
        if (actualPose_) {
#ifdef DEBUG
            Serial.println("update pose");
#endif
            actualPose_->update();
#ifdef DEBUG
            Serial.println("get next pose");
#endif
            actualPose_ = actualPose_->getNextPose();
#ifdef DEBUG
            if (!actualPose_) {
                Serial.println("movement end");
            }
#endif
        }
    }

    bool isRunning() {
        return (nullptr != actualPose_);
    }
    
protected:
private:
    Pose * actualPose_;
    
};

#endif /* MovementController_h */
