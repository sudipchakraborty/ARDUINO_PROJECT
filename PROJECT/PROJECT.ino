#include "StatusBlink.h"
#include "door.h"

StatusBlink st;
door dr;
//////////////////////////////////////////////
void setup() 
{
  st.init(23,1000);
  Serial.begin(115200); 
  dr.init();
  Serial3.begin(115200);
}
//////////////////////////////////////////////
void loop() 
{
  st.blink();
  dr.handle();
}
///////////////////////////////////////////////
