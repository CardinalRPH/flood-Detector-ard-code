#pragma once
#include "esp32-hal.h"
#include "esp32-hal-gpio.h"
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>


// Dummy sensor data

// jsn-sro4t init sensor pin
// TRIG	GPIO5
// VCC	5V
// ECHO	GPIO18
// GND	GND
#define TRIG_PIN  5  // ESP32 GPIO5
#define ECHO_PIN  18  // ESP32 GPIO18 (via voltage divider!)

// DHT 11 init sensor pin
// 1 GND
// 2 GPIO17
// 3 3V3
#define DHTPIN 17
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void initSensors() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  dht.begin();

  delay(2000);
}

//depth sensor
inline int getSensor1() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);  // pulse 10us
  digitalWrite(TRIG_PIN, LOW);

  // Baca lama pantulan echo
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // timeout 30ms ~ 5 meter

  if (duration == 0) {
    Serial.println("No echo received");
  } else {
    // Kecepatan suara 0.0343 cm/us
    float distance = duration * 0.0343 / 2;
    return distance;
  }
}

// temperature sensor
int getSensor2() {
  float t = dht.readTemperature();
  Serial.println("Temp Sens");
  Serial.println(t);
  if (isnan(t)) {
    Serial.println(-1);
    return -1;
  }
  Serial.println((int)t);
  return (int)t;
}

// humidity sensor
int getSensor3() {
  float h = dht.readHumidity();
  Serial.println("Humd Sens");
  Serial.println(h);
  if (isnan(h)) {
    Serial.println(-1);
    return -1;
  }
  Serial.println((int)h);
  return (int)h;
}
