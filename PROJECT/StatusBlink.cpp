#include "StatusBlink.h"
#include <Arduino.h>
///////////////////////////
StatusBlink::StatusBlink()  // Constructor
{   
}
//__________________________________________________________________________________________________________________________________________________________________
void StatusBlink::init(char pin, unsigned int speed)
{
    pin_no=pin;
    speed_val=speed;
    pinMode(pin, OUTPUT);
    state=true;
    delay_reg=0;
}
//__________________________________________________________________________________________________________________________________________________________________
 void StatusBlink::blink()
{
    if(state==true)
    {
      delay_reg++;
      if(delay_reg>=speed_val)
      {
        state=false;
        digitalWrite(pin_no, LOW);    
        delay_reg=0;
      }
    }
    else
    {
      delay_reg++;
      if(delay_reg>=speed_val)
      {
        state=true;
        digitalWrite(pin_no, HIGH);
        delay_reg=0;
      }
    }
}
//__________________________________________________________________________________________________________________________________________________________________
                
 
 
