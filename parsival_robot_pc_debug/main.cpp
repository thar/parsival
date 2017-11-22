#define PC_VERSION

#include "Arduino.h"
#include "Parsival.h"

int main()
{
    Parsival parsival(Serial, Serial);
    parsival.init();

    while (!Serial.exit()) {
        parsival.loop();
    }

    return 0;
}