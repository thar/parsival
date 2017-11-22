#ifndef MovementController_h
#define MovementController_h

#ifdef PC_VERSION
#include "Arduino.h"
#endif
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
            //Serial.println("update pose");
            actualPose_->update();
            //Serial.println("get next pose");
            actualPose_ = actualPose_->getNextPose();
            //Serial.print("Pose:");
            //Serial.println(actualPose_);
            if (!actualPose_) {
                Serial.println("movement end");
            }
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
