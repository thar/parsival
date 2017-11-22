#include "Parsival.h"

Parsival parsival(Serial, Serial2);

void setup(void)
{
  Serial2.begin(9600);
  parsival.init();
}

void loop(void)
{
  parsival.loop();
}

