//#include "door.h"
#include "StatusBlink.h"
#include "PWM.h"
///////////////////////////////////////////////////////////////////////////
#define PIN_Motor  23
int count=0;
extern struct project prj;

PWM pwm;

////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(27, OUTPUT);
  Serial.begin(115200); 
  prj.FSM=FSM_Init;
  prj.Waiting_Count=0;
  pwm.init(2, 20000, 50);
}
////////////////////////////////////////////////////////////////////////////
void loop() 
{
  // systemStateHandler();
//  StatusLED();
//  Serial.print("COUNT="); Serial.println(count);
//  count++;
}
