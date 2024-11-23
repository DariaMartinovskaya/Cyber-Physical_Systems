#include <Arduino.h>

// //Define LED
// int LED = 2;

// // //Check if the LED is working
// void setup() {
//   pinMode(LED,OUTPUT);
// }

// // //LED turn on and turn off
// void loop() {
//   digitalWrite(LED,HIGH);
//   delay(1000);

//   digitalWrite(LED,LOW);
//   delay(1000);

// }

// #include <cstdint>
// struct Button
// {
//   const uint8_t PIN;
//   uint32_t numberKeyPresses;
//   bool pressed;
// };
// Button button1 = {18, 0, false};

// // variables to keep track of the timing of recent interrupts
// unsigned long button_time = 0;
// unsigned long last_button_time = 0;

// void IRAM_ATTR isr_button()
// {
//   button_time = millis();
//   if (button_time - last_button_time > 250)
//   {
//     button1.numberKeyPresses++;
//     button1.pressed = true;
//     last_button_time = button_time;
//   }
// }
// void setup()
// {
//   Serial.begin(9600);
//   pinMode(button1.PIN, INPUT_PULLUP);
//   attachInterrupt(button1.PIN, isr_button, FALLING);
// }
// void loop()
// {
//   if (button1.pressed)
//   {
//     Serial.printf("Button has been pressed %u times\n",
//                   button1.numberKeyPresses);
//     button1.pressed = false;
//   }
// }

#define LED 2
hw_timer_t *My_timer = NULL;
void IRAM_ATTR onTimer(){
    digitalWrite(LED, !digitalRead(LED));
}
void setup() {
//Toggle the LED
    pinMode(LED, OUTPUT);
    My_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(My_timer, &onTimer, true);
    timerAlarmWrite(My_timer, 1000000, true);
    timerAlarmEnable(My_timer); //Enable the timer
}
void loop() {
}