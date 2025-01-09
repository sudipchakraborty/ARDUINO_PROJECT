
#ifndef MOTORDC_H
#define MOTORDC_H

//
//
//// High-resolution PWM on Arduino Mega using Timer3 (for pin 2)
//
//// Define the PWM parameters
//#define PWM_PIN 2          // Pin 2 (OC3B) connected to the motor
//#define PWM_FREQUENCY 600000// Frequency in Hz (e.g., 2 kHz)
//#define DUTY_CYCLE 50      // Duty cycle in percentage (e.g., 50%)
//
//
//
//enum FSM
//{
//  FSM_Init,
//  FSM_Read_Motion_Sensor,
//  FSM_Read_Initial_Sensor,
//  FSM_Waiting_For_Terminal_Trigger,
//  FSM_Waiting_Door_Clear,
//  FSM_Waiting_For_Full_Close
//};

class motorDC{

  public:
      motorDC();
      void init(char pin);
//      void systemStateHandler(void);
//      void setupTimer3(int frequency, int dutyCycle);
//      void setup2();
//      void updateDutyCycle(int dutyCycle);
  private:

};

//
//
//
/////////////////////////////////////////
//struct project
//{
//  char FSM;
//  int  Waiting_Count;
//};


#endif
