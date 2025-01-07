
#define DOOR_H

void systemStateHandler(void);

enum FSM
{
  FSM_Init,
  FSM_Read_Motion_Sensor,
  FSM_Read_Initial_Sensor,
  FSM_Waiting_For_Terminal_Trigger,
  FSM_Waiting_Door_Clear,
  FSM_Waiting_For_Full_Close
};
///////////////////////////////////////
struct project
{
  char FSM;
  int  Waiting_Count;
};
