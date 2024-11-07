#include <Arduino.h>

int x = 2;

//Check if the LED is working
void setup() {
  pinMode(x,OUTPUT);
}

//LED turn on and turn off 
void loop() {
  digitalWrite(x,HIGH);
  delay(1000);
  digitalWrite(x,LOW);
  delay(1000);

}

// // put function declarations here:
// int myFunction(int, int);

// void setup() {
//   // put your setup code here, to run once:
//   int result = myFunction(2, 3);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }