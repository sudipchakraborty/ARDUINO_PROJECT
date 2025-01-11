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
    pinMode(PIN_DIR, OUTPUT);
    TestCount=0;
    bzr.init(31);
    btn.init(29);
    FSM=FSM_Init;
    Waiting_Count=0;
    motor.init(2,30000,0);
    count.init();  
}
//___________________________________________________________________________________________________________________________________________________________
/**
 * @brief initialize function
 * @param   to set the output
  * @param   to set the blink speed
 * @return void
 */
void door::handle(void)
{
    switch(FSM)
  {
    case FSM_Init:   
    if(btn.pressed())
      {
        Waiting_Count=0;
        bzr.beep();
        delay(1000);
        door_Activity=Door_Opening;
        FSM=FSM_Wait_For_Trigger;
      }  
      else
      {
          Serial3.print("\rWaiting for Trigger to Open The door..."); Serial3.print(Waiting_Count);
          Waiting_Count++;
          delay(100);
      }   
    break;
    //////////////////
     case FSM_Wait_For_Trigger:
      
        if(door_Activity==Door_Opening) 
        {
          Serial3.println("Door is Opening Now....");
          digitalWrite(PIN_DIR, Dir_Open);
        }
        else 
        {
          Serial3.println("Door is Closing Now....");
          digitalWrite(PIN_DIR, Dir_Close);
        }
      


          Serial3.print("\n\r============================<TEST COUNT="); Serial3.print(TestCount); Serial3.println(">=====================================");
          Serial3.println("Button Pressed");
          bzr.off();
          motor.enable();
          motor.start(dc_start);
          FSM=FSM_Initial_Jogg;
          count.set(Tick);
          temp=0;

     break;
     ///////////////////
   case FSM_Initial_Jogg:
    if(JoG_Completed(temp, motor_start_jog))
      {
        Serial3.println("\r\nInitial Jog Completed..Start Accelerating..");
        accl_count_reg=5;count.set(10000);FSM=FSM_Acceleration;    
      }  
   break;
     ///////////////////
   case FSM_Acceleration:
     if(count.over())
      {
        accl_count_reg++;
        Serial3.print("\raccl_count_reg=");Serial3.print(accl_count_reg);
        motor.updateDutyCycle(accl_count_reg);
        if(accl_count_reg>=30)
        {
          jog_count_reg=0;
          count.set(Tick);
          Serial3.println("\r\nAcceleration Completed. Going to Jog Phase..");
          FSM=FSM_Jog;
        }
      }

   break;
     ///////////////////
   case FSM_Jog:
   if(count.over())
      {
        jog_count_reg++;   
        Serial3.print("\rJog Count=");Serial3.print(jog_count_reg);  
        if(jog_count_reg>=jog_duration)
        {
          Serial3.println("\r\nJog Completed. Going to deceleration..");
          count.set(10000);
          FSM=FSM_Deceleration;
        }
      }
   break;
     ///////////////////
   case FSM_Deceleration:
    if(count.over())
      {
        accl_count_reg--;
        Serial3.print("\rdecl_count_reg=");Serial3.print(accl_count_reg);  
        motor.updateDutyCycle(accl_count_reg);
        if(accl_count_reg<10)
        {
          Serial3.println("\r\nDeceleration Completed. Waiting for Sensor..");
          jog_count_reg=0;
          count.set(Tick);
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
          Serial3.print("\r\nFinal Jog Completed. Going to Motor Stop");
          motor.updateDutyCycle(0);
          delay(10);
          motor.disable();

          if(door_Activity==Door_Opening)
          {
            delay(10000);
            FSM=FSM_Wait_For_Trigger;
            Serial3.println("Motor Stopped. Going Back..");
            TestCount++;
            door_Activity=Door_Closing;
          }
          else
          {
              FSM=FSM_Init;
          }

          
        }
        else
        {
          jog_count_reg++;
          Serial3.print("\rFinal Jog. Waiting for sensor=");Serial3.print(jog_count_reg);  
        }
      }

    break;
    ////////////////////////
    defult:
    break;
  } 
}
//___________________________________________________________________________________________________________________________________________________________
/**
 * @brief Jog function
 * @param  val_inc increment value
 * @param  val_cmp compare value
 * @return true if jog completed, else false
 */
bool door::JoG_Completed(unsigned int val_inc, unsigned int val_cmp)
{
  if(count.over())
  {
      if(val_inc>=val_cmp)
      {
        Serial3.println("\r\nJog Completed");
        return true;
      }
      else
      {
        temp++;
        Serial3.print("\rJog Count=");  Serial3.print(temp);
        return false;
      }
  }
  else
  {
      return false;
  }
}
//___________________________________________________________________________________________________________________________________________________________



