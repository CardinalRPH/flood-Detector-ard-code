#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"
#include "storage.h"
#include "webserver_utils.h"
#include <ArduinoJson.h>

unsigned long lastTime = 0;

JsonDocument httpResData;
DeserializationError parseError;
long duration; // Time taken for the pulse to reach the receiver
int distance; 

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  
  // Serial.begin(9600);
  Serial.println("Distance measurement using JSN-SR04T");
  delay(500);
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

  // initSensors();
  // Web server
  // setupWebServer();
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);  // turn on the Trigger to generate pulse
  delayMicroseconds(10);        // keep the trigger "ON" for 10 ms to generate pulse
  digitalWrite(trigPin, LOW);   // Turn off the pulse trigger to stop pulse

  // If pulse reached the receiver echoPin
  // become high Then pulseIn() returns the
  // time taken by the pulse to reach the receiver
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0344 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);
  // server.handleClient();

  // // POST evry interval
  // if ((millis() - lastTime) >= configData.timerDelay) {
  //   lastTime = millis();
  //   if (WiFi.status() == WL_CONNECTED) {
  //     HTTPClient http;
  //     WiFiClient client;

  //     http.begin(client, configData.serverName);
  //     http.addHeader("Content-Type", "application/json");

  //     JsonDocument docToSent;
  //     docToSent["uuid_device"] = configData.uuidDevice;
  //     docToSent["depth"] = getSensor1();
  //     docToSent["temperature"] = getSensor2();
  //     docToSent["humidity"] = getSensor3();

  //     String bodyDoc;
  //     serializeJson(docToSent, bodyDoc);

  //     int httpResCode = http.POST(bodyDoc);
  //     if (httpResCode > 0) {
  //       Serial.printf("[POST] Code: %d\n", httpResCode);
  //       String httpPayload = http.getString();

  //       parseError = deserializeJson(httpResData, httpPayload);

  //       if (parseError) {
  //         Serial.print("Failed to parse JSON: ");
  //         Serial.println(parseError.c_str());
  //         return;
  //       }

  //       int statusCode = httpResData["status"];
  //       if (statusCode == 0) {
  //         Serial.print("Zero (0) status code");
  //         Serial.println("Please check on the server response");
  //         return;
  //       }
  //       Serial.println("Code Fetched");
  //     } else {
  //       Serial.print("Error code: ");
  //       Serial.println(httpResCode);
  //     }
  //     //end of http
  //     http.end();
  //   }
  // }
}
