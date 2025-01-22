 #ifndef DOOR_H
 #define DOOR_H
 #include <Arduino.h>
//////////////////
class door
{
    public:
    door();
    void init(void);
    void handle(void);
    void set_door_open(void);
    void set_door_close(void);
    bool door_not_closed(void);
    bool door_closed(void);
    bool door_opened(void);
    void PrintSensorStatus(void);
    void test_open(void);
    void test_close(void);
    void action_for_command(String *cmd);
    ////////////////////////
    private:
    bool JoG_Completed(unsigned int val_inc, unsigned int val_cmp);
    void setUpTimer1(long milliseconds);
    char FSM;
    int  Waiting_Count;
    long accl_count_reg;
    long jog_count_reg;
    long temp;
    int TestCount;
    char door_Activity;

};


enum FSM
{
  FSM_Init,
  FSM_check_door_open,
  FSM_Wait_For_Full_Close,
  FSM_Wait_For_Open_Sensor_trigger,
  FSM_Wait_For_Close_Sensor_Trigger,
  FSM_Wait_For_Trigger,
  FSM_Initial_Jogg,
  FSM_Acceleration,
  FSM_Jog,
  FSM_Deceleration,
  FSM_Final_Jogg,
  FSM_Waiting_For_Terminal_Trigger,
  FSM_Waiting_Door_Clear,
  FSM_Waiting_For_Full_Close
};
///////////////////////////////////////
#define  Tick  1000
#define dc_start   5 
#define  motor_start_jog    250 
#define during_accl_count  5000
#define jog_duration      500

#define PIN_DIR   33
#define Dir_Open  0
#define Dir_Close 1

#define Door_Opening   1
#define Door_Closing   2

#define open  1
#define close 2
//___________________________________________________________________________________________________________________________________________________________
 #endif




 

// #include "motorDC.h"
// ///////////////////////////////////////////////////////////////////////////
// #define PIN_Motor  23
// int count=0;

 
//  pinMode(27, OUTPUT);
//   Serial.begin(115200); 
//  

//   // systemStateHandler();
// //  StatusLED();
