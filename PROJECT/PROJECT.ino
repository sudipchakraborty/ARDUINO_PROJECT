#include "StatusBlink.h"
#include "door.h"
#include "CommandParser.h"

StatusBlink st;
door dr;
CommandParser cmd;
//////////////////////////////////////////////
void setup() 
{
  cmd.init();
  dr.init();
  st.init(23,1000);
  Serial.begin(115200); 
  Serial3.begin(115200);
}
//////////////////////////////////////////////
void loop() 
{
  cmd.handle();
  if(cmd.available()) {
    dr.action_for_command(cmd.parsedParts);
    }
  st.blink();
  dr.handle();
}
///////////////////////////////////////////////

















