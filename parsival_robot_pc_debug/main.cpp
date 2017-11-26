#include "Arduino.h"
#include "Parsival.h"

int main()
{
    JointController jointController(Serial);
    MovementList movementList(jointController);
    Parsival parsival(Serial, movementList);
    parsival.init();

    while (!Serial.exit()) {
        parsival.loop();
    }

    return 0;
}