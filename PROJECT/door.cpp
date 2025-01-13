#include "door.h"
#include <Arduino.h>
#include "button.h"
#include "buzzer.h"
#include "PWM.h"
#include "virtualCounter.h"
#include "timer1.h"
///////////////////////
button btn_trig;
button btn_home;
buzzer bzr;
PWM motor;
virtualCounter count;
extern timer1 t1;

#define SWITCH1_PIN 3   // Switch 1 connected to pin 3
#define SWITCH2_PIN 19  // Switch 2 connected to pin 19
#define SWITCH3_PIN 20  // Switch 3 connected to pin 20

volatile bool switch1State = false; // State of Switch 1
volatile bool switch2State = false; // State of Switch 2
volatile bool switch3State = false; // State of Switch 3


void switch1ISR() {
  // Toggle the state of switch 1
  switch1State = !switch1State;
  Serial3.println("Switch 1 Toggled: " + String(switch1State));
}

void switch2ISR() {
  // Toggle the state of switch 2
  switch2State = !switch2State;
  Serial3.println("Switch 2 Toggled: " + String(switch2State));
}

void switch3ISR() {
  // Toggle the state of switch 3
  switch3State = !switch3State;
  Serial3.println("Switch 3 Toggled: " + String(switch3State));
}


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
    btn_trig.init(29);
    btn_home.init(4);
    FSM=FSM_Init;
    Waiting_Count=0;
    motor.init(2,30000,0);
    count.init();  
    
    pinMode(SWITCH1_PIN, INPUT_PULLUP);
    pinMode(SWITCH2_PIN, INPUT_PULLUP);
    pinMode(SWITCH3_PIN, INPUT_PULLUP);
     // Attach interrupts for the switches
    attachInterrupt(digitalPinToInterrupt(SWITCH1_PIN), switch1ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SWITCH2_PIN), switch2ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SWITCH3_PIN), switch3ISR, CHANGE);

    t1.init(10);
    t1.setInterval(5);
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
  if(btn_home.pressed())
  {
    Serial3.println(" Home button pressed..");
    delay(2000);
  }

    if(btn_trig.pressed())
  {
    Serial3.println(" Triggered button pressed..");
     delay(2000);
  }

  switch(FSM)
  {
    case FSM_Init:   
    if(btn_trig.pressed())
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
          t1.setInterval(100);
          temp=0;

     break;
     ///////////////////
   case FSM_Initial_Jogg:
    if(t1.completed)
      {
        Serial3.println("\r\nInitial Jog Completed..Start Accelerating..");
        accl_count_reg=5;
        t1.setInterval(1000);
        FSM=FSM_Acceleration;    
      }  
   break;
     ///////////////////
   case FSM_Acceleration:
     if(t1.completed)
      {
        accl_count_reg++;
        Serial3.print("\raccl_count_reg=");Serial3.print(accl_count_reg);
        motor.updateDutyCycle(accl_count_reg);
        if(accl_count_reg>=30)
        {
          jog_count_reg=0;
          t1.setInterval(100);
          Serial3.println("\r\nAcceleration Completed. Going to Jog Phase..");
          FSM=FSM_Jog;
        }
      }

   break;
     ///////////////////
   case FSM_Jog:
   if(t1.completed)
      {
        jog_count_reg++;   
        Serial3.print("\rJog Count=");Serial3.print(jog_count_reg);  
        if(jog_count_reg>=jog_duration)
        {
          Serial3.println("\r\nJog Completed. Going to deceleration..");
          t1.setInterval(1000);
          FSM=FSM_Deceleration;
        }
      }
   break;
     ///////////////////
   case FSM_Deceleration:
    if(t1.completed)
      {
        accl_count_reg--;
        Serial3.print("\rdecl_count_reg=");Serial3.print(accl_count_reg);  
        motor.updateDutyCycle(accl_count_reg);
        if(accl_count_reg<10)
        {
          Serial3.println("\r\nDeceleration Completed. Waiting for Sensor..");
          jog_count_reg=0;
          t1.setInterval(100);
          FSM=FSM_Final_Jogg;
        }
      }
   break;
     ///////////////////
   case FSM_Final_Jogg:
      if(t1.completed)
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
//___________________________________________________________________________________________________________________________________________________________________________________