#include <Arduino.h>

//Define LED
int LED = 2;

// //Check if the LED is working
void setup() {
  pinMode(LED,OUTPUT);
}

// //LED turn on and turn off 
void loop() {
  digitalWrite(LED,HIGH);
  delay(1000);

  digitalWrite(LED,LOW);
  delay(1000);

}