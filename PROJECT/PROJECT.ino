#include "StatusBlink.h"
#include "door.h"
#include "CommandParser.h"
#include <ArduinoJson.h>
#include "Arduino.h"

StatusBlink st;
door dr;
CommandParser cmd;

String receivedDataMega = ""; // Buffer to store incoming data
//////////////////////////////////////////////
void setup() 
{
  cmd.init();
  dr.init();
  st.init(23,1000);
  Serial.begin(115200); 
  Serial3.begin(115200);
}
//////////////////////////////////////////////
void loop() 
{

  cmd.handle();
  if(cmd.available()) {
    dr.action_for_command(cmd.parsedParts);
    }
  st.blink();
  dr.handle();
  /////////////////////
  if(Serial3.available()) 
  {  // Read all available data
        char receivedChar = Serial3.read();

        if (receivedChar == '\n') 
        { // Message complete
            Serial.print("Full Message Received: ");
            Serial.println(receivedDataMega);

            StaticJsonDocument<256> doc; // Create JSON document (adjust size as needed)
    
            DeserializationError error = deserializeJson(doc, receivedDataMega);
            receivedDataMega = ""; // Clear buffer    
            
            if (error) {
                Serial.print("JSON Parsing Error: ");
                Serial.println(error.c_str());
            }

            // Example: Extracting values from JSON
            const char* command = doc["command"];
            const char* value = doc["value"];

            // // Process the extracted data
            // Serial.print("Command: ");
            // Serial.println(command);
            // Serial.print("Value: ");
            // Serial.println(value);

             // Example: Implementing commands
            if ((strcmp(command, "Door") == 0) && (strcmp(value, "Open") == 0))
            {
                dr.Web_Command="Open";
                receivedDataMega = ""; // Clear buffer    
            } 
        }
        else 
        {
            receivedDataMega += receivedChar; // Append to buffer
         //   Serial.print(receivedDataMega);
        }
    
}
}
///////////////////////////////////////////////

// // Function to parse JSON string
// StaticJsonDocument<256> parseJSON(String jsonString) 
// {
//     StaticJsonDocument<256> doc; // Create JSON document (adjust size as needed)
    
//     DeserializationError error = deserializeJson(doc, jsonString);
//     if (error) {
//         Serial.print("JSON Parsing Error: ");
//         Serial.println(error.c_str());
//     }
    
//     return doc;  // Return the parsed JSON object

//     // Example: Extracting values from JSON
//     const char* command = doc["command"];
//     int value = doc["value"];

//     // Process the extracted data
//     Serial.print("Command: ");
//     Serial.println(command);
//     Serial.print("Value: ");
//     Serial.println(value);
    
//     // Example: Implementing commands
//     if (strcmp(command, "LED_ON") == 0) {
//         digitalWrite(13, HIGH);
//         Serial.println("LED turned ON");
//     } else if (strcmp(command, "LED_OFF") == 0) {
//         digitalWrite(13, LOW);
//         Serial.println("LED turned OFF");
//     }
// }















