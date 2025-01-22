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

int state_count=0;

#define Emergency_SW_PIN 3   // Switch 1 connected to pin 3
#define Close_Sensor_PIN 19  // Switch 2 connected to pin 19
#define Open_Sensor_PIN 20  // Switch 3 connected to pin 20

volatile bool Emergency_SW= false; // State of Switch 1
volatile bool Close_Sensor= false; // State of Switch 2
volatile bool Open_Sensor= false; // State of Switch 3
volatile  char SensorTrigger=false;
/////////////////////////////////////
void EmergencySwISR() 
{
  Serial3.println("Emergency Switch ");
}
////////////////////////////////
void CloseSensorISR() 
{
  motor.updateDutyCycle(0);
  SensorTrigger=close;
  cli(); // Disable all interrupts globally
}
///////////////////////////////
void OpenSensorISR() 
{ 
  motor.updateDutyCycle(0);
  SensorTrigger=open;
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
    motor.init(2,30000);
    motor.disable();
    count.init(); 
     
    pinMode(Emergency_SW_PIN, INPUT_PULLUP);
    pinMode(Close_Sensor_PIN, INPUT_PULLUP);
    pinMode(Open_Sensor_PIN, INPUT_PULLUP);

     // Attach interrupts for the switches
    // attachInterrupt(digitalPinToInterrupt(Emergency_SW_PIN), EmergencySwISR,LOW);
    // attachInterrupt(digitalPinToInterrupt(Close_Sensor_PIN), CloseSensorISR, LOW);
    // attachInterrupt(digitalPinToInterrupt(Open_Sensor_PIN), OpenSensorISR, LOW);

    t1.init(25);
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
  ////////////////////////////////////
  switch(FSM)
  {
    case FSM_Init:  
    test_open();
    // test_close();
    // PrintSensorStatus(); 
      // Serial3.println(" FSM state: FSM_Init.."); 
      FSM=FSM_check_door_open;
    break;
    //////////////////
    case FSM_check_door_open:
    return;

      if(door_not_closed())
      {
         Serial3.println("door_not_closed");
         delay(1000);
         set_door_close();
         Serial3.println(" Set Motor Direction is to be closed");
         motor.updateDutyCycle(10);
         motor.enable();
         FSM=FSM_Wait_For_Full_Close;
      }
      else
      {
         Serial3.println("door closed");
      }
      return;

    break;
    ///////////////////
    case FSM_Wait_For_Full_Close:

    if(door_closed())
    {
       motor.updateDutyCycle(0);
       Serial3.println("\rdoor closed Now. Waiting for Trigger to open the door..");
       FSM=FSM_Wait_For_Trigger;
    }
    else
    {
      Serial3.print("\rWaiting for door close="); Serial3.print(state_count);state_count++;
    }
    break;
    ///////////////////////////////
     case FSM_Wait_For_Trigger:

      if(btn_trig.pressed())
      {
            Serial3.println("Button Pressed");
            Serial3.println("Door is Opening Now....");
            set_door_open();
            Serial3.println("Going for intitial Jog..");
            FSM=FSM_Initial_Jogg;
            door_Activity=Door_Opening;
            t1.setInterval(100);
            motor.updateDutyCycle(5);
      }
      else
      {
        Serial3.print("\rFSM_Wait_For_Trigger="); Serial3.print(state_count);state_count++;
      }
     break;
     ///////////////////
   case FSM_Initial_Jogg:
    if(t1.completed)
      {
        Serial3.println("\r\nInitial Jog Completed..Start Accelerating..");
        accl_count_reg=5;
        t1.setInterval(5);
        FSM=FSM_Acceleration;    
      }  
   break;
     ///////////////////
   case FSM_Acceleration:
     if(t1.completed)
      {
        t1.reset();
        accl_count_reg++;
        Serial3.print("accl_count_reg=");Serial3.println(accl_count_reg);
        motor.updateDutyCycle(accl_count_reg);
        if(accl_count_reg>=20)
        {
          jog_count_reg=0;
          Serial3.println("\r\nAcceleration Completed. Going to Jog Phase..");
          FSM=FSM_Jog;
        }
      }

   break;
     ///////////////////
   case FSM_Jog:
   if(t1.completed)
      {
        t1.reset();
        jog_count_reg++;   
        Serial3.print("\rJog Count=");Serial3.print(jog_count_reg);  
        if(jog_count_reg>=10)
        {
          Serial3.println("\r\nJog Completed. Going to deceleration..");
          FSM=FSM_Deceleration;
        }
      }
   break;
     ///////////////////
   case FSM_Deceleration:
    if(t1.completed)
      {
        t1.reset();
        accl_count_reg--;
        Serial3.print("\rdecl_count_reg=");Serial3.print(accl_count_reg);  
        motor.updateDutyCycle(accl_count_reg);
        if(accl_count_reg<10)
        {
          Serial3.println("\r\nDeceleration Completed. Waiting for Sensor..");
          jog_count_reg=0;
          FSM=FSM_Final_Jogg;
          SensorTrigger=false;
        }
      }
   break;
     ///////////////////
   case FSM_Final_Jogg:

      if(t1.completed)
      {
          t1.reset();
          jog_count_reg++;
          Serial3.print("\rFinal Jog. Waiting for sensor=");Serial3.print(jog_count_reg);  

          if(door_Activity==Door_Opening)
          {
            if(door_opened())
            {
              motor.updateDutyCycle(0);
              Serial3.print("\r\n Open Sensor Triggered. Motor Stopped");
              delay(5000);         
              TestCount++;
              door_Activity=Door_Closing;   
    
              Serial3.println("Door is closing Now....");
              set_door_close();
              Serial3.println("Going for intitial Jog..");
              t1.setInterval(100);
              motor.updateDutyCycle(5);
              FSM=FSM_Initial_Jogg;
            }
          }
          else if(door_Activity==Door_Closing)
          {
            if(door_closed())
            {
              motor.updateDutyCycle(0);
              Serial3.print("\r\n close Sensor Triggered. Motor Stopped");
              delay(50);         
              TestCount++;       
              FSM=FSM_Wait_For_Trigger;
            }
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
/**
 * @brief set motor direction bit as door open
 * @param  void
 * @param  void
 * @return void
 */
void door::set_door_open(void)
{
  digitalWrite(PIN_DIR, Dir_Open);
}
//___________________________________________________________________________________________________________________________________________________________________________________
/**
 * @brief set motor direction bit as door close
 * @param  void
 * @param  void
 * @return void
 */
void door::set_door_close(void)
{
  digitalWrite(PIN_DIR, Dir_Close);
}
//___________________________________________________________________________________________________________________________________________________________________________________
 /**
 * @brief set motor direction bit as door close
 * @param  void
 * @param  void
 * @return void
 */
 bool door:: door_not_closed(void)
 {
    bool st=digitalRead(Close_Sensor_PIN);
    if(st !=0) return true;
    else return false;
 } 
 //___________________________________________________________________________________________________________________________________________________________________________________
 /**
 * @brief set motor direction bit as door close
 * @param  void
 * @param  void
 * @return void
 */
 bool door:: door_closed(void)
 {
    bool st=digitalRead(Close_Sensor_PIN);
    if(st ==0) return true;
    else return false;
 } 
 //___________________________________________________________________________________________________________________________________________________________________________________
 /**
 * @brief set motor direction bit as door close
 * @param  void
 * @param  void
 * @return void
 */
 bool door:: door_opened(void)
 {
    bool st=digitalRead(Open_Sensor_PIN);
    if(st ==0) return true;
    else return false;
 } 
 //__________________________________________________________________________________________________________________________________________________________________________________
/**
 * @brief set motor direction bit as door close
 * @param  void
 * @param  void
 * @return void
 */
void door::PrintSensorStatus(void)
{
    bool cs=digitalRead(Close_Sensor_PIN);
    Serial.print("\rCLOSE SENSOR STATUS="); 
    if(cs==0) Serial.println("CLOSED"); else  Serial.println("OPEN");

   // bool os=digitalRead(Open_Sensor_PIN);
 
}
//__________________________________________________________________________________________________________________________________________________________________________________
/**
 * @brief set motor direction bit as door close
 * @param  void
 * @param  void
 * @return void
 */
void door::test_close(void)
{
   set_door_close();
   t1.setInterval(10);
   motor.updateDutyCycle(20);
   motor.enable();

   delay(45000);
   motor.updateDutyCycle(0);
}
//__________________________________________________________________________________________________________________________________________________________________________________
/**
 * @brief set motor direction bit as door close
 * @param  void
 * @param  void
 * @return void
 */
void door::test_open(void)
{
   set_door_open();
   t1.setInterval(10);
   motor.updateDutyCycle(20);
   motor.enable();

   delay(40000);
   motor.updateDutyCycle(0);
}
//__________________________________________________________________________________________________________________________________________________________________________________
/**
 * @brief fuction to take action against user command through terminal
 * @param  void
 * @param  void
 * @return void
 */
void door::action_for_command(String *cmd)
{
  String s=*cmd;

  if(s=="f") // forward
  {
    String dist= *(cmd+1); 
    int intValue = dist.toInt(); 
    Serial.print("Motor Moving Forward  upto=");
    Serial.println(String(intValue));
  }
  /////////////////////////
  if(s=="r") // forward
  {

  }
  /////////////////////////
  if(s=="s") // start
  {

  }
  /////////////////////////
  if(s=="p") // start
  {

  }
  /////////////////////////


}
//__________________________________________________________________________________________________________________________________________________________________________________












