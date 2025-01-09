#include "Arduino.h"
#include "motorDC.h"
#include "PWM.h"


PWM pwm;
//__________________________________________________________________________________________________________________________________________________________________
motorDC::motorDC()  // // Constructor
{
    pwm.init(2, 20000, 50);
}
////__________________________________________________________________________________________________________________________________________________________________
//void PWM::init(char pin, int frequency, char duty_cycle)
//{
//  //   // pinMode(PWM_PIN, OUTPUT); // Set PWM pin as output
////   // digitalWrite(PWM_PIN,LOW);
////   // setupTimer3(PWM_FREQUENCY, DUTY_CYCLE);
////   // updateDutyCycle(1);
//}
////__________________________________________________________________________________________________________________________________________________________________
//// void setup2() {
////   // pinMode(PWM_PIN, OUTPUT); // Set PWM pin as output
////   // digitalWrite(PWM_PIN,LOW);
////   // setupTimer3(PWM_FREQUENCY, DUTY_CYCLE);
////   // updateDutyCycle(1);
//// }
//
//// void loop2() {
////   // Your main code here (if needed)
//// }
////__________________________________________________________________________________________________________________________________________________________________
//
//// Function to update duty cycle dynamically
//void updateDutyCycle(int dutyCycle) {
//  int timerCounts = ICR3;
//  int dutyCounts = (timerCounts * dutyCycle) / 100;
//  OCR3B = dutyCounts; // Update the duty cycle
//}
////__________________________________________________________________________________________________________________________________________________________________
//
//// Function to set up Timer3 for high-resolution PWM
//void setupTimer3(int frequency, int dutyCycle) {
//  // Stop Timer3
//  TCCR3A = 0;
//  TCCR3B = 0;
//  TCNT3 = 0;
//
//  // Calculate the timer counts for the given frequency
//  int timerCounts = (16000000 / (frequency * 2)) - 1; // 16 MHz clock, Prescaler = 1
//
//  // Set the duty cycle
//  int dutyCounts = (timerCounts * dutyCycle) / 100;
//
//  // Configure Timer3 for PWM
//  TCCR3A = (1 << COM3B1) | (1 << WGM31); // Non-inverting PWM on OC3B, Fast PWM mode
//  TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS30); // Fast PWM, Prescaler = 1
//
//  ICR3 = timerCounts; // Set the TOP value
//  OCR3B = dutyCounts; // Set the duty cycle
//}
////__________________________________________________________________________________________________________________________________________________________________
