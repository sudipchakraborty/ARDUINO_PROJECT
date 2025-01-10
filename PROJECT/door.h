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
    long accl_count_reg;
    long jog_count_reg;
    long decl_count_reg;
};


enum FSM
{
  FSM_Init,
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



