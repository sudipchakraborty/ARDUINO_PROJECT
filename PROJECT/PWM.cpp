// High-resolution PWM on Arduino Mega using Timer3 (for pin 2)

// Define the PWM parameters
#define PWM_PIN 2          // Pin 2 (OC3B) connected to the motor
#define PWM_FREQUENCY 600000// Frequency in Hz (e.g., 2 kHz)
#define DUTY_CYCLE 50      // Duty cycle in percentage (e.g., 50%)

void setup() {
  pinMode(PWM_PIN, OUTPUT); // Set PWM pin as output
  digitalWrite(PWM_PIN,LOW);
  setupTimer3(PWM_FREQUENCY, DUTY_CYCLE);
  updateDutyCycle(1);
}

void loop() {
  // Your main code here (if needed)
}

// Function to set up Timer3 for high-resolution PWM
void setupTimer3(int frequency, int dutyCycle) {
  // Stop Timer3
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;

  // Calculate the timer counts for the given frequency
  int timerCounts = (16000000 / (frequency * 2)) - 1; // 16 MHz clock, Prescaler = 1

  // Set the duty cycle
  int dutyCounts = (timerCounts * dutyCycle) / 100;

  // Configure Timer3 for PWM
  TCCR3A = (1 << COM3B1) | (1 << WGM31); // Non-inverting PWM on OC3B, Fast PWM mode
  TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS30); // Fast PWM, Prescaler = 1

  ICR3 = timerCounts; // Set the TOP value
  OCR3B = dutyCounts; // Set the duty cycle
}

// Function to update duty cycle dynamically
void updateDutyCycle(int dutyCycle) {
  int timerCounts = ICR3;
  int dutyCounts = (timerCounts * dutyCycle) / 100;
  OCR3B = dutyCounts; // Update the duty cycle
}
