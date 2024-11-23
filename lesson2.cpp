#include <Arduino.h>

// //Define LED
// int LED = 2;

// //Check if the LED is working
// void setup() {
//   pinMode(LED,OUTPUT);
// }

// //LED turn on and turn off 
// void loop() {
//   digitalWrite(LED,HIGH);
//   delay(1000);

//   digitalWrite(LED,LOW);
//   delay(1000);

// }

// Set pin numbers
// Pushbutton pin
const int buttonPin = 4; 
// The number of the LED pin
const int ledPin = 5;

// VAriable for storing the pushbutton status
int buttonState = 0;

void setup() {
 Serial.begin(115200);
 // initialize the pushbutton pin as an input
 pinMode(buttonPin, INPUT);
 // initialize the LED pin as an output
 pinMode(ledPin, OUTPUT);
 }

void loop(){
  // Read the status of the pushbutton value
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  // Cheack if the pushbutton is pressed.
  // If it is, the buttonState is HIGH
  if (buttonState == HIGH){
    //Turn LED on
    digitalWrite(ledPin, HIGH);
  } else {
    // Turn LED off
    digitalWrite(ledPin, LOW);
  }
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