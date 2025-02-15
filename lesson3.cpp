#include <Arduino.h>
#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing WiFi...");
  WiFi.mode(WIFI_STA);
  Serial.println("Setup done!");
}

void loop() {
  Serial.println("Scanning...");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("Scan done!");
  if (n == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.println();
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}

#include <Arduino.h>

#include <cstdint>
struct Button
{
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};
Button button1 = {18, 0, false};

// variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

void IRAM_ATTR isr_button()
{
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    button1.numberKeyPresses++;
    button1.pressed = true;
    last_button_time = button_time;
  }
}
void setup()
{
  Serial.begin(9600);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr_button, FALLING);
}
void loop()
{
  if (button1.pressed)
  {
    Serial.printf("Button has been pressed %u times\n",
                  button1.numberKeyPresses);
    button1.pressed = false;
  }
}

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
