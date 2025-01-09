 #ifndef DOOR_H
 #define DOOR_H
//////////////////
class door
{
    public:
    door();
    void init(void);
    void handle(void);
    ////////////////////////
    private:
    char FSM;
    int  Waiting_Count;

 

};


enum FSM
{
  FSM_Init,
  FSM_Read_button,
  FSM_Read_Camera_Trigger,
  FSM_Read_Motion_Sensor,
  FSM_Read_Initial_Sensor,
  FSM_Waiting_For_Terminal_Trigger,
  FSM_Waiting_Door_Clear,
  FSM_Waiting_For_Full_Close
};
///////////////////////////////////////
 

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



