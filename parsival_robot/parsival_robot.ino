#include "Parsival.h"

JointController jointController(Serial1);
MovementList movementList(jointController);
Parsival parsival(Serial, movementList);

void setup(void)
{
  Serial.begin(115200);
  Serial1.begin(9600);
  parsival.init();
}

void loop(void)
{
  parsival.loop();
}

