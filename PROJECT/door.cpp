#include "door.h"
#include <Arduino.h>

struct project prj;

//////////////////////////////////////
void systemStateHandler(void)
{
  switch(prj.FSM)
  {
    case FSM_Init:
      Serial.println("System Init State");
      prj.FSM=FSM_Read_Motion_Sensor;
    break;
    //////////////////
    case FSM_Read_Motion_Sensor:

    Serial.print("Waiting for Motion Sensor Trigger>");Serial.println(prj.Waiting_Count);
    break;
    /////////////////
    case FSM_Read_Initial_Sensor:
    
    break;
    ////////////////
    case FSM_Waiting_For_Terminal_Trigger:
    
    break;
    ////////////////
    case FSM_Waiting_Door_Clear:
    
    break;
    ////////////////
    case FSM_Waiting_For_Full_Close:

    break;
    ////////////////
    defult:
    break;
  } 
    prj.Waiting_Count++;
}
///////////////////////////////////////////////
