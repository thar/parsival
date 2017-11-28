#ifndef Parsival_h
#define Parsival_h

#include "JointController.h"
#include "MovementController.h"
#include "MovementList.h"
#include <Arduino.h>


class Parsival {
public:
    Parsival(Stream& debugSerial, MovementList& movementList)
        : debugSerial(debugSerial), movementList(movementList) {
    }
    
    ~Parsival() {
    }
    
    void center() {
        movementController.addMovement(movementList.getRelaxPoseMovement());
        while (movementController.isRunning())
            movementController.doStep();
    }

    void loop() {
        processSerial();
        movementController.doStep();
    }

    void init() {
        center();
    }
    
protected:
    void processSerial() {
        while (debugSerial.available()) {
            uint8_t incommingByte = static_cast<uint8_t>(debugSerial.read());
#ifdef DEBUG
            debugSerial.print("Leido: ");
            debugSerial.println(incommingByte);
#endif
            if (incommingByte < movementList.size()) {
#ifdef DEBUG
                debugSerial.print("adding movement ");
                debugSerial.println(incommingByte);
#endif
                movementController.addMovement(movementList[incommingByte]);
            }
        }
    }
    
private:
    Stream& debugSerial;
    MovementController movementController;
    MovementList movementList;
    
};

#endif /* Parsival_h */
