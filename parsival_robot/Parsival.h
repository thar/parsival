#ifndef Parsival_h
#define Parsival_h

#include "JointController.h"
#include "MovementController.h"
#include "MovementList.h"
#include <Arduino.h>

class Parsival {
public:
    Parsival(Stream& debugSerial, MovementList& movementList)
        : debugSerial(debugSerial), movementList(movementList),
          rxBuffer{0} {
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
        Serial.println("inicio de centrado");
        center();
        debugSerial.println("center finished");
    }
    
protected:
    void processRxBuffer() {
        for (unsigned int i=0; i < movementList.size(); ++i) {
            if (rxBuffer[i] == '1') {
                debugSerial.print("adding movement ");
                debugSerial.println(i);
                movementController.addMovement(movementList[i]);
            }
        }
    }

    void processSerial() {
        while (debugSerial.available()) {
            char incommingByte = debugSerial.read();
            debugSerial.print("Leido: ");
            debugSerial.println(incommingByte);
            if (bufferIndex < movementList.size() && incommingByte != END_OF_LINE) {
                rxBuffer[bufferIndex] = incommingByte;
                ++bufferIndex;
            }
            else if (incommingByte == END_OF_LINE) {
                bufferIndex = 0;
                processRxBuffer();
                memset(rxBuffer, 0, movementList.size());
            } else {
                memset(rxBuffer, 0, movementList.size());
            }
        }
    }
    
private:
    static const char END_OF_LINE = 'x';
    unsigned int bufferIndex = 0;
    Stream& debugSerial;
    MovementController movementController;
    MovementList movementList;
    char rxBuffer[MovementList::size()];
    
};

#endif /* Parsival_h */
