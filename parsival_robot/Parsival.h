#ifndef Parsival_h
#define Parsival_h

#include "JointController.h"
#include "MovementController.h"
#include "MovementList.h"
#ifdef PC_VERSION
#include <cstring>
#include "Stream.h"
#include "HardwareSerial.h"
#endif

class Parsival {
public:
    Parsival(Stream& commSerial, HardwareSerial& debugSerial)
        : debugSerial(debugSerial), jointController(commSerial), movementList(jointController),
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
        debugSerial.begin(115200);
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
#ifndef PC_VERSION
            debugSerial.print("Leido: ");
            debugSerial.println(incommingByte);
#endif
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
    HardwareSerial& debugSerial;
    JointController jointController;
    MovementController movementController;
    MovementList movementList;
    char rxBuffer[MovementList::size()];
    
};

#endif /* Parsival_h */
