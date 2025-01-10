#include "door.h"
#include <Arduino.h>
#include "button.h"
#include "buzzer.h"
#include "PWM.h"
#include "virtualCounter.h"
///////////////////////
button btn;
buzzer bzr;
PWM motor;
virtualCounter count;
//___________________________________________________________________________________________________________________________________________________________
door::door()
{
}
//___________________________________________________________________________________________________________________________________________________________
void door::init(void)
{
    bzr.init(31);
    btn.init(29);
    FSM=FSM_Init;
    Waiting_Count=0;
    motor.init(2,40000,0);
    count.init();
}
//___________________________________________________________________________________________________________________________________________________________
void door::handle(void)
{
    switch(FSM)
  {
    case FSM_Init:   
      FSM=FSM_Wait_For_Trigger;
    break;
    //////////////////
     case FSM_Wait_For_Trigger:
      // if(btn.pressed())
      // {
          bzr.beep();
          delay(1000);
          Serial.println("Button Pressed");
          bzr.off();
          motor.enable();
          motor.start(3);
          FSM=FSM_Initial_Jogg;
          count.set(1000);
     // }
     break;
     ///////////////////
   case FSM_Initial_Jogg:
      if(count.over())
      {
        Serial.println("Initial Jog Completed..Start Accelerating..");
        count.set(10000);
        accl_count_reg=5;
        FSM=FSM_Acceleration;
      }
   break;
     ///////////////////
   case FSM_Acceleration:
     if(count.over())
      {
        accl_count_reg++;
        Serial.print("accl_count_reg=");Serial.println(accl_count_reg);
        motor.updateDutyCycle(accl_count_reg);
        if(accl_count_reg>=20)
        {
          jog_count_reg=0;
          Serial.print("Acceleration Completed. Going to Jog Phase..");
          FSM=FSM_Jog;
        }
      }

   break;
     ///////////////////
   case FSM_Jog:
   if(count.over())
      {
        jog_count_reg++;   
        Serial.print("Jog Count=");Serial.println(jog_count_reg);  
        if(jog_count_reg>=40)
        {
          Serial.print("Jog Completed. Going to deceleration..");
          decl_count_reg=50;
          FSM=FSM_Deceleration;
        }
      }
   break;
     ///////////////////
   case FSM_Deceleration:
    if(count.over())
      {
        decl_count_reg--;
        Serial.print("decl_count_reg=");Serial.println(decl_count_reg);  
        motor.updateDutyCycle(decl_count_reg);
        if(decl_count_reg<10)
        {
          Serial.print("Deceleration Completed. Going to Final jog..");
          jog_count_reg=0;
          FSM=FSM_Final_Jogg;
        }
      }
   break;
     ///////////////////
   case FSM_Final_Jogg:
      if(count.over())
      {
        if(jog_count_reg>=100)
        {
          Serial.print("Final Jog Completed. Going to Motor Stop");
          jog_count_reg=0;
          motor.disable();
          delay(3000);
          FSM=FSM_Wait_For_Trigger;
          Serial.print("Motor Stopped. Going Back..");
        }
      }

   break;
     ///////////////////

 

    // case FSM_Read_Motion_Sensor:

    // Serial.print("Waiting for Motion Sensor Trigger>");Serial.println(Waiting_Count);
    // FSM=FSM_Read_button;
    // break;
    // /////////////////
    // case FSM_Read_Initial_Sensor:
    
    // break;
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
