#define MAX_COMMAND_LENGTH 50 // Maximum command length
#define MAX_PARTS 10          // Maximum number of parts to parse

String parsedParts[MAX_PARTS]; // Array to store parsed parts
int numParts = 0;             // Number of parts parsed

char commandBuffer[MAX_COMMAND_LENGTH]; // Buffer for incoming command
int bufferIndex = 0;                   // Index for buffer
bool commandReady = false;             // Flag for complete command

void setup() {
   pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter command (separated by space or comma):");
}

void loop() {

 digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(25);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(25);    

  // Read serial data non-blocking
  while (Serial.available() > 0 && !commandReady) {
    char incomingChar = Serial.read();
    if (incomingChar == '\n') {
      commandBuffer[bufferIndex] = '\0'; // Null-terminate the string
      commandReady = true;               // Command is ready
    } else if (bufferIndex < MAX_COMMAND_LENGTH - 1) {
      commandBuffer[bufferIndex++] = incomingChar;
    }
  }

  // Process the command when ready
  if (commandReady) {
    String command = String(commandBuffer);
    command.trim(); // Remove leading and trailing whitespace

    // Parse the command
    numParts = parseCommand(command, parsedParts, MAX_PARTS);

    // Print parsed parts
    Serial.println("Parsed command parts:");
    for (int i = 0; i < numParts; i++) {
      Serial.print("Part ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(parsedParts[i]);
    }

    // Reset for next command
    bufferIndex = 0;
    commandReady = false;
    Serial.println("Enter next command:");
  }
}

// Function to parse a command into parts based on spaces or commas
int parseCommand(String command, String parts[], int maxParts) {
  int count = 0; // Counter for parts

  while (command.length() > 0 && count < maxParts) {
    int separatorIndex = findSeparatorIndex(command); // Find the next separator index
    if (separatorIndex == -1) {
      // If no separator is found, the rest of the command is a part
      parts[count++] = command;
      break;
    } else {
      // Extract the part before the separator
      parts[count++] = command.substring(0, separatorIndex);
      // Remove the extracted part from the command
      command = command.substring(separatorIndex + 1);
      command.trim(); // Remove extra spaces or commas
    }
  }

  return count; // Return the number of parts parsed
}

// Function to find the index of the next separator (space or comma)
int findSeparatorIndex(String command) {
  int spaceIndex = command.indexOf(' '); // Index of the first space
  int commaIndex = command.indexOf(','); // Index of the first comma

  if (spaceIndex == -1) return commaIndex; // No space, return comma index
  if (commaIndex == -1) return spaceIndex; // No comma, return space index
  return min(spaceIndex, commaIndex);      // Return the smaller index (nearest separator)
}











// #include "StatusBlink.h"
// #include "door.h"

// StatusBlink st;
// door dr;
// //////////////////////////////////////////////
// void setup() 
// {
//   // dr.init();
//   // st.init(23,1000);
//   // Serial.begin(115200); 
//   // Serial3.begin(115200);
// }
// //////////////////////////////////////////////
// void loop() 
// {
//   // st.blink();
//   // dr.handle();
// }
// ///////////////////////////////////////////////












// #include <Wire.h>
// #include <RTClib.h>


// //adafruit RTClib
// // Create an RTC_DS1307 object
// RTC_DS1307 rtc;

// void setup() {
//   Serial.begin(9600); // Initialize serial communication
//   Wire.begin();       // Initialize I2C communication

//   if (!rtc.begin()) {
//     Serial.println("Couldn't find RTC");
//     while (1); // Stop the program if RTC is not found
//   }

//   if (!rtc.isrunning()) {
//     Serial.println("RTC is NOT running, setting the time!");

//     // Set the date and time to the current time
//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//   }
// }

// void loop() {
//   // Get the current time
//   DateTime now = rtc.now();

//   // Print the date and time in a readable format
//   Serial.print("Date: ");
//   Serial.print(now.year(), DEC);
//   Serial.print('/');
//   Serial.print(now.month(), DEC);
//   Serial.print('/');
//   Serial.print(now.day(), DEC);

//   Serial.print("  Time: ");
//   Serial.print(now.hour(), DEC);
//   Serial.print(':');
//   Serial.print(now.minute(), DEC);
//   Serial.print(':');
//   Serial.println(now.second(), DEC);

//   delay(1000); // Wait for 1 second
// }











// //adafruit Adafruit_NeoPixel
// #include <Adafruit_NeoPixel.h>

// // Pin connected to the DIN of the LED strip
// #define LED_PIN 6

// // Number of LEDs in the strip
// #define NUM_LEDS 4

// // Create a NeoPixel object
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// void setup() {
//   strip.begin();      // Initialize the LED strip
//   strip.show();       // Turn off all LEDs at the start
//   strip.setBrightness(50); // Set brightness (0-255)
// }

// void loop() {
//   // Example effects
//   colorWipe(strip.Color(255, 0, 0), 50); // Red
//   colorWipe(strip.Color(0, 255, 0), 50); // Green
//   colorWipe(strip.Color(0, 0, 255), 50); // Blue
//   rainbow(10);                           // Rainbow effect
// }

// // Fill the strip with a single color
// void colorWipe(uint32_t color, int wait) {
//   for (int i = 0; i < strip.numPixels(); i++) {
//     strip.setPixelColor(i, color); // Set the color of pixel i
//     strip.show();                 // Update the strip to show the change
//     delay(wait);                  // Wait for the next LED
//   }
// }

// // Create a rainbow effect
// void rainbow(int wait) {
//   for (int j = 0; j < 256; j++) { // Cycle all colors
//     for (int i = 0; i < strip.numPixels(); i++) {
//       strip.setPixelColor(i, wheel((i + j) & 255));
//     }
//     strip.show();
//     delay(wait);
//   }
// }

// // Generate rainbow colors across 0-255
// uint32_t wheel(byte position) {
//   position = 255 - position;
//   if (position < 85) {
//     return strip.Color(255 - position * 3, 0, position * 3);
//   } else if (position < 170) {
//     position -= 85;
//     return strip.Color(0, position * 3, 255 - position * 3);
//   } else {
//     position -= 170;
//     return strip.Color(position * 3, 255 - position * 3, 0);
//   }
// }





















