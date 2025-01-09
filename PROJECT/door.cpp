#include "door.h"
#include <Arduino.h>
//___________________________________________________________________________________________________________________________________________________________
door::door()
{
}
//___________________________________________________________________________________________________________________________________________________________
void door::init(void)
{
   FSM=FSM_Init;
   Waiting_Count=0;
}
//___________________________________________________________________________________________________________________________________________________________
void door::handle(void)
{
    switch(FSM)
  {
    case FSM_Init:   
      FSM=FSM_Read_button;
    break;
    //////////////////
    case FSM_Read_button:
    
      FSM=FSM_Read_Camera_Trigger;
    break;
    ///////////////////
    case FSM_Read_Camera_Trigger:

      FSM=FSM_Read_Motion_Sensor;

    break;
    ////////////////////
    case FSM_Read_Motion_Sensor:

    Serial.print("Waiting for Motion Sensor Trigger>");Serial.println(Waiting_Count);
    FSM=FSM_Read_button;
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
    Waiting_Count++;
}
//___________________________________________________________________________________________________________________________________________________________
