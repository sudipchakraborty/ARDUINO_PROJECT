#include "StatusBlink.h"
#include "door.h"

StatusBlink st;
door dr;
//////////////////////////////////////////////
void setup() 
{
  dr.init();
  st.init(23,1000);
  Serial.begin(115200); 

  Serial3.begin(115200);
}
//////////////////////////////////////////////
void loop() 
{
  //st.blink();
 dr.handle();
}
///////////////////////////////////////////////
