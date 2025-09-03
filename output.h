#include "esp32-hal-gpio.h"
#define RELAY_PIN 16   // pin yang dipakai untuk relay

void initOutput() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void setRelay(int RelValue) {
  if(RelValue == 1) {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Relay: ON");
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Relay: OFF");
  }
}
