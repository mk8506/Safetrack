// // https://wokwi.com/
// #include <Arduino.h>
// #include <Wire.h>

// volatile unsigned long startTime = 0;
// volatile unsigned long currentTime = 0;
// volatile bool ledState = 0;
// u_int8_t serialInput[4]; 

// void setup() {
//   pinMode(LED_BUILTIN, OUTPUT);
//   digitalWrite(LED_BUILTIN, 0);
//   startTime = millis();
// }

// void loop() {
//   currentTime = millis(); //counts up
//   if ((currentTime - startTime) > 1000)  {   //if reached 1000ms 
//     ledState = !ledState; 
//     startTime = currentTime; //reset to 0 and count up
//     Serial.println("turn on/off");
//   }
//   digitalWrite(LED_BUILTIN, ledState);
//   // delay(1000);  
//   //arduino code without delay: delaying every input so need to be avoided
//   int length = Serial.readBytes(serialInput, 4); //dnm whether input == null
//   Serial.print("\nserialInput length: "+String(length)+"\n");
//   for (int i=0; i<length; i++) {
//     Serial.print(serialInput[i]);
//   }
//   Serial.print("\n");
// }