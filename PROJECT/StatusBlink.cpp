#include "StatusBlink.h"
#include <Arduino.h>


void StatusLED(void)
{
  digitalWrite(27, HIGH);                  // turn the LED on (HIGH is the voltage level)
  delay(75);                                      // wait for a second
  digitalWrite(27, LOW);                 // turn the LED off by making the voltage LOW
  delay(75);                                      // wait for a second
}