#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"
#include "output.h"
#include "webserver_utils.h"
#include <ArduinoJson.h>

unsigned long lastTime = 0;

// gunakan kapasitas yang jelas
StaticJsonDocument<512> httpResData1;
StaticJsonDocument<512> httpResData2;

void setup() {
  delay(500);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

  // init Sensor Pin
  initSensors();
  //init Output Pin
  initOutput();
  // Web server
  setupWebServer();
}

void loop() {
  server.handleClient();

  handleRelayTimer();

  // POST setiap interval
  if ((millis() - lastTime) >= timerDelay) {
    lastTime = millis();

    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;  // bikin client baru setiap request
      HTTPClient http;

      if (http.begin(client, serverNameA)) {
        http.useHTTP10(true);  // disable keep-alive, aman untuk ESP32
        http.addHeader("Content-Type", "application/json");

        // JSON yang dikirim
        StaticJsonDocument<256> docToSend;
        docToSend["uuid_device"] = uuidDevice;
        docToSend["depth"] = getSensor1();
        // docToSend["temperature"] = getSensor2();
        // docToSend["humidity"] = getSensor3();

        String bodyDoc;
        serializeJson(docToSend, bodyDoc);
        Serial.println(bodyDoc);
        int httpResCode = http.POST(bodyDoc);
        if (httpResCode > 200) {
          Serial.printf("[POST] Code: %d\n", httpResCode);
          String httpPayload = http.getString();
          Serial.println(httpPayload);

          // if (httpPayload.startsWith("{") || httpPayload.startsWith("[")) {
          //   // parse JSON response
          //   DeserializationError parseError = deserializeJson(httpResData1, httpPayload);
          //   if (parseError) {
          //     Serial.print("Failed to parse JSON: ");
          //     Serial.println(parseError.c_str());
          //   }
          // } else {
          //   Serial.println("⚠️ Response isn't JSON.");
          // }
        } else {
          Serial.print("HTTP POST error: ");
          Serial.println(httpResCode);
        }

        http.end();  // WAJIB: selalu dipanggil
      }

      if (http.begin(client, serverNameB)) {
        http.useHTTP10(true);  // disable keep-alive, aman untuk ESP32
        http.addHeader("Content-Type", "application/json");

        // JSON yang dikirim
        int httpResCode = http.GET();
        if (httpResCode > 0) {
          Serial.printf("[POST] Code: %d\n", httpResCode);
          String httpPayload = http.getString();
          Serial.println(httpPayload);

          // parse JSON response
          if (httpPayload.startsWith("{") || httpPayload.startsWith("[")) {
            DeserializationError parseError = deserializeJson(httpResData2, httpPayload);
            if (parseError) {
              Serial.print("Failed to parse JSON: ");
              Serial.println(parseError.c_str());
            } else {
              int relayValue = httpResData2["status"] | false;
              setRelay(relayValue);
            }
          } else {
            Serial.println("⚠️ Response isn't JSON.");
          }
        } else {
          Serial.print("HTTP GET error: ");
          Serial.println(httpResCode);
        }

        http.end();  // WAJIB: selalu dipanggil
      }
    }
  }
}
