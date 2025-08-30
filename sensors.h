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
#define echoPin 18
#define trigPin 5

// DHT 11 init sensor pin
// 1 GND
// 2 GPIO17
// 3 3V3
#define DHTPIN 17
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void initSensors() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  dht.begin();
  
  delay(2000);
}

//depth sensor
int getSensor1() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  if (duration == 0) {
    return -1;
  }

  int distance = duration * 0.0344 / 2;
  Serial.println("Dept sens");
  Serial.println(distance);
  return distance;
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
