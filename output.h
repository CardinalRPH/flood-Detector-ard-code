#include "esp32-hal.h"
#include "esp32-hal-gpio.h"
#define RELAY_PIN 16  // pin yang dipakai untuk relay

bool relayActive = false;
unsigned long relayStartTime = 0;
const unsigned long RELAY_DURATION = 1UL * 60UL * 1000UL;  // 1 meni

void initOutput() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void setRelay(bool RelValue) {
  if (RelValue) {
    digitalWrite(RELAY_PIN, LOW);  // nyalakan relay
    Serial.println("Relay: ON");
    relayActive = true;
    relayStartTime = millis();  // catat waktu mulai
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // matikan relay
    Serial.println("Relay: OFF");
    relayActive = false;
  }
}

void handleRelayTimer() {
  if (relayActive && (millis() - relayStartTime >= RELAY_DURATION)) {
    digitalWrite(RELAY_PIN, HIGH);  // matikan relay otomatis
    Serial.println("Relay: OFF (auto after 1 min)");
    relayActive = false;
  }
}
