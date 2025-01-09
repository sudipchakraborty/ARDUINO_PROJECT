//#include "door.h"
#include "StatusBlink.h"

#include "motorDC.h"
///////////////////////////////////////////////////////////////////////////
#define PIN_Motor  23
int count=0;


///////////////////////////////////////
struct project
{
  char FSM;
  int  Waiting_Count;
};

enum FSM
{
  FSM_Init,
  FSM_Read_Motion_Sensor,
  FSM_Read_Initial_Sensor,
  FSM_Waiting_For_Terminal_Trigger,
  FSM_Waiting_Door_Clear,
  FSM_Waiting_For_Full_Close
};


struct project prj;



////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(27, OUTPUT);
  Serial.begin(115200); 
  prj.FSM=FSM_Init;
  prj.Waiting_Count=0;

}
////////////////////////////////////////////////////////////////////////////
void loop() 
{
  // systemStateHandler();
//  StatusLED();
//  Serial.print("COUNT="); Serial.println(count);
//  count++;
}
